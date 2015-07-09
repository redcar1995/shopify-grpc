/*
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

#include "src/core/channel/http_client_filter.h"
#include <string.h>
#include <grpc/support/log.h>

typedef struct call_data {
  grpc_linked_mdelem method;
  grpc_linked_mdelem scheme;
  grpc_linked_mdelem te_trailers;
  grpc_linked_mdelem content_type;
  int sent_initial_metadata;

  int got_initial_metadata;
  grpc_stream_op_buffer *recv_ops;

  /** Closure to call when finished with the hc_on_recv hook */
  grpc_iomgr_closure *on_done_recv;
  /** Receive closures are chained: we inject this closure as the on_done_recv
      up-call on transport_op, and remember to call our on_done_recv member
      after handling it. */
  grpc_iomgr_closure hc_on_recv;
} call_data;

typedef struct channel_data {
  grpc_mdelem *te_trailers;
  grpc_mdelem *method;
  grpc_mdelem *scheme;
  grpc_mdelem *content_type;
  grpc_mdelem *status;
} channel_data;

/* used to silence 'variable not used' warnings */
static void ignore_unused(void *ignored) {}

static grpc_mdelem *client_filter(void *user_data, grpc_mdelem *md) {
  grpc_call_element *elem = user_data;
  channel_data *channeld = elem->channel_data;
  if (md == channeld->status) {
    return NULL;
  } else if (md->key == channeld->status->key) {
    grpc_call_element_send_cancel(elem);
    return NULL;
  }
  return md;
}

static void hc_on_recv(void *user_data, int success) {
  grpc_call_element *elem = user_data;
  call_data *calld = elem->call_data;
  if (success) {
    size_t i;
    size_t nops = calld->recv_ops->nops;
    grpc_stream_op *ops = calld->recv_ops->ops;
    for (i = 0; i < nops; i++) {
      grpc_stream_op *op = &ops[i];
      if (op->type != GRPC_OP_METADATA) continue;
      calld->got_initial_metadata = 1;
      grpc_metadata_batch_filter(&op->data.metadata, client_filter, elem);
    }
  }
  calld->on_done_recv->cb(calld->on_done_recv->cb_arg, success);
}

static void hc_mutate_op(grpc_call_element *elem,
                         grpc_transport_stream_op *op) {
  /* grab pointers to our data from the call element */
  call_data *calld = elem->call_data;
  channel_data *channeld = elem->channel_data;
  size_t i;
  if (op->send_ops && !calld->sent_initial_metadata) {
    size_t nops = op->send_ops->nops;
    grpc_stream_op *ops = op->send_ops->ops;
    for (i = 0; i < nops; i++) {
      grpc_stream_op *op = &ops[i];
      if (op->type != GRPC_OP_METADATA) continue;
      calld->sent_initial_metadata = 1;
      /* Send : prefixed headers, which have to be before any application
         layer headers. */
      grpc_metadata_batch_add_head(&op->data.metadata, &calld->method,
                                   grpc_mdelem_ref(channeld->method));
      grpc_metadata_batch_add_head(&op->data.metadata, &calld->scheme,
                                   grpc_mdelem_ref(channeld->scheme));
      grpc_metadata_batch_add_tail(&op->data.metadata, &calld->te_trailers,
                                   grpc_mdelem_ref(channeld->te_trailers));
      grpc_metadata_batch_add_tail(&op->data.metadata, &calld->content_type,
                                   grpc_mdelem_ref(channeld->content_type));
      break;
    }
  }

  if (op->recv_ops && !calld->got_initial_metadata) {
    /* substitute our callback for the higher callback */
    calld->recv_ops = op->recv_ops;
    calld->on_done_recv = op->on_done_recv;
    op->on_done_recv = &calld->hc_on_recv;
  }
}

static void hc_start_transport_op(grpc_call_element *elem,
                                  grpc_transport_stream_op *op) {
  GRPC_CALL_LOG_OP(GPR_INFO, elem, op);
  hc_mutate_op(elem, op);
  grpc_call_next_op(elem, op);
}

/* Constructor for call_data */
static void init_call_elem(grpc_call_element *elem,
                           const void *server_transport_data,
                           grpc_transport_stream_op *initial_op) {
  call_data *calld = elem->call_data;
  calld->sent_initial_metadata = 0;
  calld->got_initial_metadata = 0;
  calld->on_done_recv = NULL;
  grpc_iomgr_closure_init(&calld->hc_on_recv, hc_on_recv, elem);
  if (initial_op) hc_mutate_op(elem, initial_op);
}

/* Destructor for call_data */
static void destroy_call_elem(grpc_call_element *elem) {
  /* grab pointers to our data from the call element */
  call_data *calld = elem->call_data;
  channel_data *channeld = elem->channel_data;

  ignore_unused(calld);
  ignore_unused(channeld);
}

static const char *scheme_from_args(const grpc_channel_args *args) {
  unsigned i;
  if (args != NULL) {
    for (i = 0; i < args->num_args; ++i) {
      if (args->args[i].type == GRPC_ARG_STRING &&
          strcmp(args->args[i].key, GRPC_ARG_HTTP2_SCHEME) == 0) {
        return args->args[i].value.string;
      }
    }
  }
  return "http";
}

/* Constructor for channel_data */
static void init_channel_elem(grpc_channel_element *elem, grpc_channel *master,
                              const grpc_channel_args *args, grpc_mdctx *mdctx,
                              int is_first, int is_last) {
  /* grab pointers to our data from the channel element */
  channel_data *channeld = elem->channel_data;

  /* The first and the last filters tend to be implemented differently to
     handle the case that there's no 'next' filter to call on the up or down
     path */
  GPR_ASSERT(!is_last);

  /* initialize members */
  channeld->te_trailers = grpc_mdelem_from_strings(mdctx, "te", "trailers");
  channeld->method = grpc_mdelem_from_strings(mdctx, ":method", "POST");
  channeld->scheme =
      grpc_mdelem_from_strings(mdctx, ":scheme", scheme_from_args(args));
  channeld->content_type =
      grpc_mdelem_from_strings(mdctx, "content-type", "application/grpc");
  channeld->status = grpc_mdelem_from_strings(mdctx, ":status", "200");
}

/* Destructor for channel data */
static void destroy_channel_elem(grpc_channel_element *elem) {
  /* grab pointers to our data from the channel element */
  channel_data *channeld = elem->channel_data;

  grpc_mdelem_unref(channeld->te_trailers);
  grpc_mdelem_unref(channeld->method);
  grpc_mdelem_unref(channeld->scheme);
  grpc_mdelem_unref(channeld->content_type);
  grpc_mdelem_unref(channeld->status);
}

const grpc_channel_filter grpc_http_client_filter = {
    hc_start_transport_op, grpc_channel_next_op, sizeof(call_data),
    init_call_elem,        destroy_call_elem,    sizeof(channel_data),
    init_channel_elem,     destroy_channel_elem, "http-client"};
