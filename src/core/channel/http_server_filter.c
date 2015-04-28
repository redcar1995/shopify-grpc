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

#include "src/core/channel/http_server_filter.h"

#include <string.h>
#include <grpc/grpc_http.h>
#include <grpc/support/alloc.h>
#include <grpc/support/log.h>

typedef struct {
  grpc_mdelem *path;
  grpc_mdelem *content_type;
  grpc_byte_buffer *content;
} gettable;

typedef struct call_data {
  gpr_uint8 got_initial_metadata;
  gpr_uint8 seen_path;
  gpr_uint8 seen_post;
  gpr_uint8 sent_status;
  gpr_uint8 seen_scheme;
  gpr_uint8 seen_te_trailers;
  grpc_linked_mdelem status;
} call_data;

typedef struct channel_data {
  grpc_mdelem *te_trailers;
  grpc_mdelem *method_post;
  grpc_mdelem *http_scheme;
  grpc_mdelem *https_scheme;
  /* TODO(klempner): Remove this once we stop using it */
  grpc_mdelem *grpc_scheme;
  grpc_mdelem *content_type;
  grpc_mdelem *status_ok;
  grpc_mdelem *status_not_found;
  grpc_mdstr *path_key;
  grpc_mdstr *authority_key;
  grpc_mdstr *host_key;

  grpc_mdctx *mdctx;

  size_t gettable_count;
  gettable *gettables;
} channel_data;

/* used to silence 'variable not used' warnings */
static void ignore_unused(void *ignored) {}

static grpc_mdelem *server_filter(void *user_data, grpc_mdelem *md) {
  grpc_call_element *elem = user_data;
  channel_data *channeld = elem->channel_data;
  call_data *calld = elem->call_data;

  /* Check if it is one of the headers we care about. */
  if (md == channeld->te_trailers || md == channeld->method_post ||
      md == channeld->http_scheme || md == channeld->https_scheme ||
      md == channeld->grpc_scheme || md == channeld->content_type) {
    /* swallow it */
    if (md == channeld->method_post) {
      calld->seen_post = 1;
    } else if (md->key == channeld->http_scheme->key) {
      calld->seen_scheme = 1;
    } else if (md == channeld->te_trailers) {
      calld->seen_te_trailers = 1;
    }
    /* TODO(klempner): Track that we've seen all the headers we should
       require */
    return NULL;
  } else if (md->key == channeld->content_type->key) {
    if (strncmp(grpc_mdstr_as_c_string(md->value), "application/grpc+", 17) ==
        0) {
      /* Although the C implementation doesn't (currently) generate them,
         any custom +-suffix is explicitly valid. */
      /* TODO(klempner): We should consider preallocating common values such
         as +proto or +json, or at least stashing them if we see them. */
      /* TODO(klempner): Should we be surfacing this to application code? */
    } else {
      /* TODO(klempner): We're currently allowing this, but we shouldn't
         see it without a proxy so log for now. */
      gpr_log(GPR_INFO, "Unexpected content-type %s",
              channeld->content_type->key);
    }
    return NULL;
  } else if (md->key == channeld->te_trailers->key ||
             md->key == channeld->method_post->key ||
             md->key == channeld->http_scheme->key ||
             md->key == channeld->content_type->key) {
    gpr_log(GPR_ERROR, "Invalid %s: header: '%s'",
            grpc_mdstr_as_c_string(md->key), grpc_mdstr_as_c_string(md->value));
    /* swallow it and error everything out. */
    /* TODO(klempner): We ought to generate more descriptive error messages
       on the wire here. */
    grpc_call_element_send_cancel(elem);
    return NULL;
  } else if (md->key == channeld->path_key) {
    if (calld->seen_path) {
      gpr_log(GPR_ERROR, "Received :path twice");
      return NULL;
    }
    calld->seen_path = 1;
    return md;
  } else if (md->key == channeld->host_key) {
    /* translate host to :authority since :authority may be
       omitted */
    grpc_mdelem *authority = grpc_mdelem_from_metadata_strings(
        channeld->mdctx, grpc_mdstr_ref(channeld->authority_key),
        grpc_mdstr_ref(md->value));
    grpc_mdelem_unref(md);
    return authority;
  } else {
    return md;
  }
}

/* Called either:
     - in response to an API call (or similar) from above, to send something
     - a network event (or similar) from below, to receive something
   op contains type and call direction information, in addition to the data
   that is being sent or received. */
static void call_op(grpc_call_element *elem, grpc_call_element *from_elem,
                    grpc_call_op *op) {
  /* grab pointers to our data from the call element */
  call_data *calld = elem->call_data;
  channel_data *channeld = elem->channel_data;
  GRPC_CALL_LOG_OP(GPR_INFO, elem, op);

  switch (op->type) {
    case GRPC_RECV_METADATA:
      grpc_metadata_batch_filter(&op->data.metadata, server_filter, elem);
      if (!calld->got_initial_metadata) {
        calld->got_initial_metadata = 1;
        /* Have we seen the required http2 transport headers?
           (:method, :scheme, content-type, with :path and :authority covered
           at the channel level right now) */
        if (calld->seen_post && calld->seen_scheme && calld->seen_te_trailers &&
            calld->seen_path) {
          grpc_call_next_op(elem, op);
        } else {
          if (!calld->seen_path) {
            gpr_log(GPR_ERROR, "Missing :path header");
          }
          if (!calld->seen_post) {
            gpr_log(GPR_ERROR, "Missing :method header");
          }
          if (!calld->seen_scheme) {
            gpr_log(GPR_ERROR, "Missing :scheme header");
          }
          if (!calld->seen_te_trailers) {
            gpr_log(GPR_ERROR, "Missing te trailers header");
          }
          /* Error this call out */
          grpc_metadata_batch_destroy(&op->data.metadata);
          op->done_cb(op->user_data, GRPC_OP_OK);
          grpc_call_element_send_cancel(elem);
        }
      } else {
        grpc_call_next_op(elem, op);
      }
      break;
    case GRPC_SEND_METADATA:
      /* If we haven't sent status 200 yet, we need to so so because it needs to
         come before any non : prefixed metadata. */
      if (!calld->sent_status) {
        calld->sent_status = 1;
        grpc_metadata_batch_add_head(&op->data.metadata, &calld->status,
                                     grpc_mdelem_ref(channeld->status_ok));
      }
      grpc_call_next_op(elem, op);
      break;
    default:
      /* pass control up or down the stack depending on op->dir */
      grpc_call_next_op(elem, op);
      break;
  }
}

/* Called on special channel events, such as disconnection or new incoming
   calls on the server */
static void channel_op(grpc_channel_element *elem,
                       grpc_channel_element *from_elem, grpc_channel_op *op) {
  /* grab pointers to our data from the channel element */
  channel_data *channeld = elem->channel_data;

  ignore_unused(channeld);

  switch (op->type) {
    default:
      /* pass control up or down the stack depending on op->dir */
      grpc_channel_next_op(elem, op);
      break;
  }
}

/* Constructor for call_data */
static void init_call_elem(grpc_call_element *elem,
                           const void *server_transport_data) {
  /* grab pointers to our data from the call element */
  call_data *calld = elem->call_data;
  channel_data *channeld = elem->channel_data;

  ignore_unused(channeld);

  /* initialize members */
  memset(calld, 0, sizeof(*calld));
}

/* Destructor for call_data */
static void destroy_call_elem(grpc_call_element *elem) {}

/* Constructor for channel_data */
static void init_channel_elem(grpc_channel_element *elem,
                              const grpc_channel_args *args, grpc_mdctx *mdctx,
                              int is_first, int is_last) {
  size_t i;
  size_t gettable_capacity = 0;

  /* grab pointers to our data from the channel element */
  channel_data *channeld = elem->channel_data;

  /* The first and the last filters tend to be implemented differently to
     handle the case that there's no 'next' filter to call on the up or down
     path */
  GPR_ASSERT(!is_first);
  GPR_ASSERT(!is_last);

  /* initialize members */
  channeld->te_trailers = grpc_mdelem_from_strings(mdctx, "te", "trailers");
  channeld->status_ok = grpc_mdelem_from_strings(mdctx, ":status", "200");
  channeld->status_not_found =
      grpc_mdelem_from_strings(mdctx, ":status", "404");
  channeld->method_post = grpc_mdelem_from_strings(mdctx, ":method", "POST");
  channeld->http_scheme = grpc_mdelem_from_strings(mdctx, ":scheme", "http");
  channeld->https_scheme = grpc_mdelem_from_strings(mdctx, ":scheme", "https");
  channeld->grpc_scheme = grpc_mdelem_from_strings(mdctx, ":scheme", "grpc");
  channeld->path_key = grpc_mdstr_from_string(mdctx, ":path");
  channeld->authority_key = grpc_mdstr_from_string(mdctx, ":authority");
  channeld->host_key = grpc_mdstr_from_string(mdctx, "host");
  channeld->content_type =
      grpc_mdelem_from_strings(mdctx, "content-type", "application/grpc");

  channeld->mdctx = mdctx;

  /* initialize http download support */
  channeld->gettable_count = 0;
  channeld->gettables = NULL;
  for (i = 0; i < args->num_args; i++) {
    if (0 == strcmp(args->args[i].key, GRPC_ARG_SERVE_OVER_HTTP)) {
      gettable *g;
      gpr_slice slice;
      grpc_http_server_page *p = args->args[i].value.pointer.p;
      if (channeld->gettable_count == gettable_capacity) {
        gettable_capacity =
            GPR_MAX(gettable_capacity * 3 / 2, gettable_capacity + 1);
        channeld->gettables = gpr_realloc(channeld->gettables,
                                          gettable_capacity * sizeof(gettable));
      }
      g = &channeld->gettables[channeld->gettable_count++];
      g->path = grpc_mdelem_from_strings(mdctx, ":path", p->path);
      g->content_type =
          grpc_mdelem_from_strings(mdctx, "content-type", p->content_type);
      slice = gpr_slice_from_copied_string(p->content);
      g->content = grpc_byte_buffer_create(&slice, 1);
      gpr_slice_unref(slice);
    }
  }
}

/* Destructor for channel data */
static void destroy_channel_elem(grpc_channel_element *elem) {
  size_t i;

  /* grab pointers to our data from the channel element */
  channel_data *channeld = elem->channel_data;

  for (i = 0; i < channeld->gettable_count; i++) {
    grpc_mdelem_unref(channeld->gettables[i].path);
    grpc_mdelem_unref(channeld->gettables[i].content_type);
    grpc_byte_buffer_destroy(channeld->gettables[i].content);
  }
  gpr_free(channeld->gettables);

  grpc_mdelem_unref(channeld->te_trailers);
  grpc_mdelem_unref(channeld->status_ok);
  grpc_mdelem_unref(channeld->status_not_found);
  grpc_mdelem_unref(channeld->method_post);
  grpc_mdelem_unref(channeld->http_scheme);
  grpc_mdelem_unref(channeld->https_scheme);
  grpc_mdelem_unref(channeld->grpc_scheme);
  grpc_mdelem_unref(channeld->content_type);
  grpc_mdstr_unref(channeld->path_key);
  grpc_mdstr_unref(channeld->authority_key);
  grpc_mdstr_unref(channeld->host_key);
}

const grpc_channel_filter grpc_http_server_filter = {
    call_op, channel_op, sizeof(call_data), init_call_elem, destroy_call_elem,
    sizeof(channel_data), init_channel_elem, destroy_channel_elem,
    "http-server"};
