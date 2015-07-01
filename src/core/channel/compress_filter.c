/*
 *
 * Copyright 2015, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <assert.h>
#include <string.h>

#include "src/core/channel/compress_filter.h"
#include "src/core/channel/channel_args.h"
#include "src/core/compression/message_compress.h"
#include <grpc/compression.h>
#include <grpc/support/log.h>
#include <grpc/support/slice_buffer.h>

typedef struct call_data {
  gpr_slice_buffer slices;
  grpc_linked_mdelem compression_algorithm_storage;
  int remaining_slice_bytes;
  grpc_compression_algorithm compression_algorithm;
  gpr_uint8 has_compression_algorithm;
} call_data;

typedef struct channel_data {
  grpc_mdstr *mdstr_request_compression_algorithm_key;
  grpc_mdstr *mdstr_outgoing_compression_algorithm_key;
  grpc_mdelem *mdelem_compression_algorithms[GRPC_COMPRESS_ALGORITHMS_COUNT];
  grpc_compression_algorithm default_compression_algorithm;
} channel_data;

/** Compress \a slices in place using \a algorithm. Returns 1 if compression did
 * actually happen, 0 otherwise (for example if the compressed output size was
 * larger than the raw input).
 *
 * Returns 1 if the data was actually compress and 0 otherwise. */
static int compress_send_sb(grpc_compression_algorithm algorithm,
                             gpr_slice_buffer *slices) {
  int did_compress;
  gpr_slice_buffer tmp;
  gpr_slice_buffer_init(&tmp);
  did_compress = grpc_msg_compress(algorithm, slices, &tmp);
  gpr_slice_buffer_swap(slices, &tmp);
  gpr_slice_buffer_destroy(&tmp);
  return did_compress;
}

/** For each \a md element from the incoming metadata, filter out the entry for
 * "grpc-encoding", using its value to populate the call data's
 * compression_algorithm field. */
static grpc_mdelem* compression_md_filter(void *user_data, grpc_mdelem *md) {
  grpc_call_element *elem = user_data;
  call_data *calld = elem->call_data;
  channel_data *channeld = elem->channel_data;

  if (md->key == channeld->mdstr_request_compression_algorithm_key) {
    const char *md_c_str = grpc_mdstr_as_c_string(md->value);
    if (!grpc_compression_algorithm_parse(md_c_str,
                                          &calld->compression_algorithm)) {
      gpr_log(GPR_ERROR, "Invalid compression algorithm: '%s'. Ignoring.",
              md_c_str);
      calld->compression_algorithm = GRPC_COMPRESS_NONE;
    }
    calld->has_compression_algorithm = 1;
    return NULL;
  }

  return md;
}

static int skip_compression(channel_data *channeld, call_data *calld) {
  if (calld->has_compression_algorithm) {
     if (calld->compression_algorithm == GRPC_COMPRESS_NONE) {
       return 1;
     }
     return 0;  /* we have an actual call-specific algorithm */
  }
  /* no per-call compression override */
  return channeld->default_compression_algorithm == GRPC_COMPRESS_NONE;
}

static void process_send_ops(grpc_call_element *elem,
                             grpc_stream_op_buffer *send_ops) {
  call_data *calld = elem->call_data;
  channel_data *channeld = elem->channel_data;
  size_t i, j;
  int did_compress = 0;

  /* buffer up slices until we've processed all the expected ones (as given by
   * GRPC_OP_BEGIN_MESSAGE) */
  for (i = 0; i < send_ops->nops; ++i) {
    grpc_stream_op *sop = &send_ops->ops[i];
    switch (sop->type) {
      case GRPC_OP_BEGIN_MESSAGE:
        calld->remaining_slice_bytes = sop->data.begin_message.length;
        if (sop->data.begin_message.flags & GRPC_WRITE_NO_COMPRESS) {
          calld->has_compression_algorithm = 1;  /* GPR_TRUE */
          calld->compression_algorithm = GRPC_COMPRESS_NONE;
        }
        break;
      case GRPC_OP_METADATA:
        /* Parse incoming request for compression. If any, it'll be available at
         * calld->compression_algorithm */
        grpc_metadata_batch_filter(&(sop->data.metadata), compression_md_filter,
                                   elem);
        if (!calld->has_compression_algorithm) {
          /* If no algorithm was found in the metadata and we aren't
           * exceptionally skipping compression, fall back to the channel
           * default */
          calld->compression_algorithm =
              channeld->default_compression_algorithm;
          calld->has_compression_algorithm = 1; /* GPR_TRUE */
        }
        break;
      case GRPC_OP_SLICE:
        if (skip_compression(channeld, calld)) continue;
        GPR_ASSERT(calld->remaining_slice_bytes > 0);
        /* add to calld->slices */
        gpr_slice_buffer_add(&calld->slices, sop->data.slice);
        calld->remaining_slice_bytes -= GPR_SLICE_LENGTH(sop->data.slice);
        if (calld->remaining_slice_bytes == 0) {
          /* compress */
          did_compress =
              compress_send_sb(calld->compression_algorithm, &calld->slices);
        }
        break;
      case GRPC_NO_OP:
        ;  /* fallthrough */
    }
  }

  /* We need to:
   * - (OP_SLICE) If compression happened, replace the input slices with the
   *   compressed ones.
   * - (BEGIN_MESSAGE) Update the message info (size, flags).
   * - (OP_METADATA) Convey the compression configuration */
  for (i = 0, j = 0; i < send_ops->nops; ++i) {
    grpc_stream_op *sop = &send_ops->ops[i];
    switch (sop->type) {
      case GRPC_OP_BEGIN_MESSAGE:
        if (did_compress) {
          sop->data.begin_message.length = calld->slices.length;
          sop->data.begin_message.flags |= GRPC_WRITE_INTERNAL_COMPRESS;
        } else {
          /* either because the user requested the exception or because
           * compressing would have resulted in a larger output */
          calld->compression_algorithm = GRPC_COMPRESS_NONE;
          /* reset the flag compression bit */
          sop->data.begin_message.flags &= ~GRPC_WRITE_INTERNAL_COMPRESS;
        }
        break;
      case GRPC_OP_METADATA:
        grpc_metadata_batch_add_head(
            &(sop->data.metadata), &calld->compression_algorithm_storage,
            grpc_mdelem_ref(channeld->mdelem_compression_algorithms
                                [did_compress ? calld->compression_algorithm
                                              : GRPC_COMPRESS_NONE]));
        break;
      case GRPC_OP_SLICE:
        if (did_compress) {
          if (j < calld->slices.count) {
            sop->data.slice = gpr_slice_ref(calld->slices.slices[j++]);
          }
        }
        break;
      case GRPC_NO_OP:
        ;  /* fallthrough */
    }
  }
}

/* Called either:
     - in response to an API call (or similar) from above, to send something
     - a network event (or similar) from below, to receive something
   op contains type and call direction information, in addition to the data
   that is being sent or received. */
static void compress_start_transport_op(grpc_call_element *elem,
                                    grpc_transport_op *op) {
  if (op->send_ops && op->send_ops->nops > 0) {
    process_send_ops(elem, op->send_ops);
  }

  /* pass control down the stack */
  grpc_call_next_op(elem, op);
}

/* Called on special channel events, such as disconnection or new incoming
   calls on the server */
static void channel_op(grpc_channel_element *elem,
                       grpc_channel_element *from_elem, grpc_channel_op *op) {
  switch (op->type) {
    default:
      grpc_channel_next_op(elem, op);
      break;
  }
}

/* Constructor for call_data */
static void init_call_elem(grpc_call_element *elem,
                           const void *server_transport_data,
                           grpc_transport_op *initial_op) {
  /* grab pointers to our data from the call element */
  call_data *calld = elem->call_data;

  /* initialize members */
  gpr_slice_buffer_init(&calld->slices);
  calld->has_compression_algorithm = 0;

  if (initial_op) {
    if (initial_op->send_ops && initial_op->send_ops->nops > 0) {
      process_send_ops(elem, initial_op->send_ops);
    }
  }
}

/* Destructor for call_data */
static void destroy_call_elem(grpc_call_element *elem) {
  /* grab pointers to our data from the call element */
  call_data *calld = elem->call_data;
  gpr_slice_buffer_destroy(&calld->slices);
}

/* Constructor for channel_data */
static void init_channel_elem(grpc_channel_element *elem,
                              const grpc_channel_args *args, grpc_mdctx *mdctx,
                              int is_first, int is_last) {
  channel_data *channeld = elem->channel_data;
  grpc_compression_algorithm algo_idx;
  const grpc_compression_level clevel =
      grpc_channel_args_get_compression_level(args);

  channeld->default_compression_algorithm =
      grpc_compression_algorithm_for_level(clevel);

  channeld->mdstr_request_compression_algorithm_key =
      grpc_mdstr_from_string(mdctx, GRPC_COMPRESS_REQUEST_ALGORITHM_KEY);

  channeld->mdstr_outgoing_compression_algorithm_key =
      grpc_mdstr_from_string(mdctx, "grpc-encoding");

  for (algo_idx = 0; algo_idx < GRPC_COMPRESS_ALGORITHMS_COUNT; ++algo_idx) {
    char *algorith_name;
    GPR_ASSERT(grpc_compression_algorithm_name(algo_idx, &algorith_name) != 0);
    channeld->mdelem_compression_algorithms[algo_idx] =
        grpc_mdelem_from_metadata_strings(
            mdctx,
            grpc_mdstr_ref(channeld->mdstr_outgoing_compression_algorithm_key),
            grpc_mdstr_from_string(mdctx, algorith_name));
  }

  /* The first and the last filters tend to be implemented differently to
     handle the case that there's no 'next' filter to call on the up or down
     path */
  GPR_ASSERT(!is_first);
  GPR_ASSERT(!is_last);
}

/* Destructor for channel data */
static void destroy_channel_elem(grpc_channel_element *elem) {
  channel_data *channeld = elem->channel_data;
  grpc_compression_algorithm algo_idx;

  grpc_mdstr_unref(channeld->mdstr_request_compression_algorithm_key);
  grpc_mdstr_unref(channeld->mdstr_outgoing_compression_algorithm_key);
  for (algo_idx = 0; algo_idx < GRPC_COMPRESS_ALGORITHMS_COUNT;
       ++algo_idx) {
    grpc_mdelem_unref(channeld->mdelem_compression_algorithms[algo_idx]);
  }
}

const grpc_channel_filter grpc_compress_filter = {
    compress_start_transport_op, channel_op, sizeof(call_data), init_call_elem,
    destroy_call_elem, sizeof(channel_data), init_channel_elem,
    destroy_channel_elem, "compress"};
