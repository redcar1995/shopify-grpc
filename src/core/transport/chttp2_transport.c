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

#include "src/core/transport/chttp2_transport.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "src/core/profiling/timers.h"
#include "src/core/support/string.h"
#include "src/core/transport/chttp2/http2_errors.h"
#include "src/core/transport/chttp2/status_conversion.h"
#include "src/core/transport/chttp2/timeout_encoding.h"
#include "src/core/transport/chttp2/internal.h"
#include "src/core/transport/transport_impl.h"
#include <grpc/support/alloc.h>
#include <grpc/support/log.h>
#include <grpc/support/slice_buffer.h>
#include <grpc/support/useful.h>

#define DEFAULT_WINDOW 65535
#define DEFAULT_CONNECTION_WINDOW_TARGET (1024 * 1024)
#define MAX_WINDOW 0x7fffffffu

#define MAX_CLIENT_STREAM_ID 0x7fffffffu

int grpc_http_trace = 0;
int grpc_flowctl_trace = 0;

#define TRANSPORT_FROM_WRITING(tw)                                        \
  ((grpc_chttp2_transport *)((char *)(tw)-offsetof(grpc_chttp2_transport, \
                                                   writing)))

#define TRANSPORT_FROM_PARSING(tw)                                        \
  ((grpc_chttp2_transport *)((char *)(tw)-offsetof(grpc_chttp2_transport, \
                                                   parsing)))

#define TRANSPORT_FROM_GLOBAL(tg)                                         \
  ((grpc_chttp2_transport *)((char *)(tg)-offsetof(grpc_chttp2_transport, \
                                                   global)))

#define STREAM_FROM_GLOBAL(sg) \
  ((grpc_chttp2_stream *)((char *)(sg)-offsetof(grpc_chttp2_stream, global)))

static const grpc_transport_vtable vtable;

static void lock(grpc_chttp2_transport *t);
static void unlock(grpc_chttp2_transport *t);

static void unlock_check_cancellations(grpc_chttp2_transport *t);
static void unlock_check_channel_callbacks(grpc_chttp2_transport *t);
static void unlock_check_reads(grpc_chttp2_transport *t);

/* forward declarations of various callbacks that we'll build closures around */
static void writing_action(void *t, int iomgr_success_ignored);
static void notify_closed(void *t, int iomgr_success_ignored);

/** Set a transport level setting, and push it to our peer */
static void push_setting(grpc_chttp2_transport *t, grpc_chttp2_setting_id id,
                         gpr_uint32 value);

/** Endpoint callback to process incoming data */
static void recv_data(void *tp, gpr_slice *slices, size_t nslices,
                      grpc_endpoint_cb_status error);

/** Start disconnection chain */
static void drop_connection(grpc_chttp2_transport *t);

/** Perform a transport_op */
static void perform_op_locked(grpc_chttp2_transport_global *transport_global,
                              grpc_chttp2_stream_global *stream_global,
                              grpc_transport_op *op);

/** Cancel a stream: coming from the transport API */
static void cancel_from_api(grpc_chttp2_transport_global *transport_global,
                            grpc_chttp2_stream_global *stream_global,
                            grpc_status_code status);

/** Add endpoint from this transport to pollset */
static void add_to_pollset_locked(grpc_chttp2_transport *t,
                                  grpc_pollset *pollset);

/*
 * CONSTRUCTION/DESTRUCTION/REFCOUNTING
 */

static void destruct_transport(grpc_chttp2_transport *t) {
  size_t i;

  gpr_mu_lock(&t->mu);

  GPR_ASSERT(t->ep == NULL);

  gpr_slice_buffer_destroy(&t->global.qbuf);

  gpr_slice_buffer_destroy(&t->writing.outbuf);
  grpc_chttp2_hpack_compressor_destroy(&t->writing.hpack_compressor);

  gpr_slice_buffer_destroy(&t->parsing.qbuf);
  grpc_chttp2_hpack_parser_destroy(&t->parsing.hpack_parser);
  grpc_chttp2_goaway_parser_destroy(&t->parsing.goaway_parser);

  grpc_mdstr_unref(t->parsing.str_grpc_timeout);

  for (i = 0; i < STREAM_LIST_COUNT; i++) {
    GPR_ASSERT(t->lists[i].head == NULL);
    GPR_ASSERT(t->lists[i].tail == NULL);
  }

  GPR_ASSERT(grpc_chttp2_stream_map_size(&t->parsing_stream_map) == 0);
  GPR_ASSERT(grpc_chttp2_stream_map_size(&t->new_stream_map) == 0);

  grpc_chttp2_stream_map_destroy(&t->parsing_stream_map);
  grpc_chttp2_stream_map_destroy(&t->new_stream_map);

  gpr_mu_unlock(&t->mu);
  gpr_mu_destroy(&t->mu);

  /* callback remaining pings: they're not allowed to call into the transpot,
     and maybe they hold resources that need to be freed */
  while (t->global.pings.next != &t->global.pings) {
    grpc_chttp2_outstanding_ping *ping = t->global.pings.next;
    grpc_iomgr_add_delayed_callback(ping->on_recv, 0);
    ping->next->prev = ping->prev;
    ping->prev->next = ping->next;
    gpr_free(ping);
  }

  grpc_mdctx_unref(t->metadata_context);

  gpr_free(t);
}

static void unref_transport(grpc_chttp2_transport *t) {
  if (!gpr_unref(&t->refs)) return;
  destruct_transport(t);
}

static void ref_transport(grpc_chttp2_transport *t) { gpr_ref(&t->refs); }

static void init_transport(grpc_chttp2_transport *t,
                           grpc_transport_setup_callback setup, void *arg,
                           const grpc_channel_args *channel_args,
                           grpc_endpoint *ep, gpr_slice *slices, size_t nslices,
                           grpc_mdctx *mdctx, int is_client) {
  size_t i;
  int j;
  grpc_transport_setup_result sr;

  GPR_ASSERT(strlen(GRPC_CHTTP2_CLIENT_CONNECT_STRING) ==
             GRPC_CHTTP2_CLIENT_CONNECT_STRLEN);

  memset(t, 0, sizeof(*t));

  t->base.vtable = &vtable;
  t->ep = ep;
  /* one ref is for destroy, the other for when ep becomes NULL */
  gpr_ref_init(&t->refs, 2);
  gpr_mu_init(&t->mu);
  grpc_mdctx_ref(mdctx);
  t->metadata_context = mdctx;
  t->endpoint_reading = 1;
  t->global.error_state = GRPC_CHTTP2_ERROR_STATE_NONE;
  t->global.next_stream_id = is_client ? 1 : 2;
  t->global.is_client = is_client;
  t->global.outgoing_window = DEFAULT_WINDOW;
  t->global.incoming_window = DEFAULT_WINDOW;
  t->global.connection_window_target = DEFAULT_CONNECTION_WINDOW_TARGET;
  t->global.ping_counter = 1;
  t->parsing.is_client = is_client;
  t->parsing.str_grpc_timeout =
      grpc_mdstr_from_string(t->metadata_context, "grpc-timeout");
  t->parsing.deframe_state = is_client ? DTS_FH_0 : DTS_CLIENT_PREFIX_0;

  gpr_slice_buffer_init(&t->global.qbuf);

  gpr_slice_buffer_init(&t->writing.outbuf);
  grpc_chttp2_hpack_compressor_init(&t->writing.hpack_compressor, mdctx);
  grpc_iomgr_closure_init(&t->writing_action, writing_action, t);

  gpr_slice_buffer_init(&t->parsing.qbuf);
  grpc_chttp2_goaway_parser_init(&t->parsing.goaway_parser);
  grpc_chttp2_hpack_parser_init(&t->parsing.hpack_parser, t->metadata_context);

  grpc_iomgr_closure_init(&t->channel_callback.notify_closed, notify_closed, t);
  if (is_client) {
    gpr_slice_buffer_add(
        &t->global.qbuf,
        gpr_slice_from_copied_string(GRPC_CHTTP2_CLIENT_CONNECT_STRING));
  }
  /* 8 is a random stab in the dark as to a good initial size: it's small enough
     that it shouldn't waste memory for infrequently used connections, yet
     large enough that the exponential growth should happen nicely when it's
     needed.
     TODO(ctiller): tune this */
  grpc_chttp2_stream_map_init(&t->parsing_stream_map, 8);
  grpc_chttp2_stream_map_init(&t->new_stream_map, 8);

  /* copy in initial settings to all setting sets */
  for (i = 0; i < GRPC_CHTTP2_NUM_SETTINGS; i++) {
    t->parsing.settings[i] = grpc_chttp2_settings_parameters[i].default_value;
    for (j = 0; j < NUM_SETTING_SETS; j++) {
      t->global.settings[j][i] =
          grpc_chttp2_settings_parameters[i].default_value;
    }
  }
  t->global.dirtied_local_settings = 1;
  /* Hack: it's common for implementations to assume 65536 bytes initial send
     window -- this should by rights be 0 */
  t->global.force_send_settings = 1 << GRPC_CHTTP2_SETTINGS_INITIAL_WINDOW_SIZE;
  t->global.sent_local_settings = 0;

  /* configure http2 the way we like it */
  if (is_client) {
    push_setting(t, GRPC_CHTTP2_SETTINGS_ENABLE_PUSH, 0);
    push_setting(t, GRPC_CHTTP2_SETTINGS_MAX_CONCURRENT_STREAMS, 0);
  }
  push_setting(t, GRPC_CHTTP2_SETTINGS_INITIAL_WINDOW_SIZE, DEFAULT_WINDOW);

  if (channel_args) {
    for (i = 0; i < channel_args->num_args; i++) {
      if (0 ==
          strcmp(channel_args->args[i].key, GRPC_ARG_MAX_CONCURRENT_STREAMS)) {
        if (is_client) {
          gpr_log(GPR_ERROR, "%s: is ignored on the client",
                  GRPC_ARG_MAX_CONCURRENT_STREAMS);
        } else if (channel_args->args[i].type != GRPC_ARG_INTEGER) {
          gpr_log(GPR_ERROR, "%s: must be an integer",
                  GRPC_ARG_MAX_CONCURRENT_STREAMS);
        } else {
          push_setting(t, GRPC_CHTTP2_SETTINGS_MAX_CONCURRENT_STREAMS,
                       channel_args->args[i].value.integer);
        }
      } else if (0 == strcmp(channel_args->args[i].key,
                             GRPC_ARG_HTTP2_INITIAL_SEQUENCE_NUMBER)) {
        if (channel_args->args[i].type != GRPC_ARG_INTEGER) {
          gpr_log(GPR_ERROR, "%s: must be an integer",
                  GRPC_ARG_HTTP2_INITIAL_SEQUENCE_NUMBER);
        } else if ((t->global.next_stream_id & 1) !=
                   (channel_args->args[i].value.integer & 1)) {
          gpr_log(GPR_ERROR, "%s: low bit must be %d on %s",
                  GRPC_ARG_HTTP2_INITIAL_SEQUENCE_NUMBER,
                  t->global.next_stream_id & 1,
                  is_client ? "client" : "server");
        } else {
          t->global.next_stream_id = channel_args->args[i].value.integer;
        }
      }
    }
  }

  gpr_mu_lock(&t->mu);
  t->channel_callback.executing = 1;
  ref_transport(t); /* matches unref at end of this function */
  gpr_mu_unlock(&t->mu);

  sr = setup(arg, &t->base, t->metadata_context);

  lock(t);
  t->channel_callback.cb = sr.callbacks;
  t->channel_callback.cb_user_data = sr.user_data;
  t->channel_callback.executing = 0;
  unlock(t);

  ref_transport(t); /* matches unref inside recv_data */
  recv_data(t, slices, nslices, GRPC_ENDPOINT_CB_OK);

  unref_transport(t);
}

static void destroy_transport(grpc_transport *gt) {
  grpc_chttp2_transport *t = (grpc_chttp2_transport *)gt;

  lock(t);
  t->destroying = 1;
  drop_connection(t);
  unlock(t);

  unref_transport(t);
}

static void close_transport_locked(grpc_chttp2_transport *t) {
  if (!t->closed) {
    t->closed = 1;
    if (t->ep) {
      grpc_endpoint_shutdown(t->ep);
    }
  }
}

static void close_transport(grpc_transport *gt) {
  grpc_chttp2_transport *t = (grpc_chttp2_transport *)gt;
  gpr_mu_lock(&t->mu);
  close_transport_locked(t);
  gpr_mu_unlock(&t->mu);
}

static void goaway(grpc_transport *gt, grpc_status_code status,
                   gpr_slice debug_data) {
  grpc_chttp2_transport *t = (grpc_chttp2_transport *)gt;
  lock(t);
  grpc_chttp2_goaway_append(t->global.last_incoming_stream_id,
                            grpc_chttp2_grpc_status_to_http2_error(status),
                            debug_data, &t->global.qbuf);
  unlock(t);
}

static int init_stream(grpc_transport *gt, grpc_stream *gs,
                       const void *server_data, grpc_transport_op *initial_op) {
  grpc_chttp2_transport *t = (grpc_chttp2_transport *)gt;
  grpc_chttp2_stream *s = (grpc_chttp2_stream *)gs;

  memset(s, 0, sizeof(*s));

  grpc_chttp2_incoming_metadata_buffer_init(&s->parsing.incoming_metadata);
  grpc_chttp2_incoming_metadata_buffer_init(&s->global.incoming_metadata);
  grpc_sopb_init(&s->writing.sopb);
  grpc_sopb_init(&s->global.incoming_sopb);
  grpc_chttp2_data_parser_init(&s->parsing.data_parser);

  ref_transport(t);

  lock(t);
  grpc_chttp2_register_stream(t, s);
  if (server_data) {
    GPR_ASSERT(t->parsing_active);
    s->global.id = (gpr_uint32)(gpr_uintptr)server_data;
    s->global.outgoing_window =
        t->global
            .settings[PEER_SETTINGS][GRPC_CHTTP2_SETTINGS_INITIAL_WINDOW_SIZE];
    s->global.incoming_window =
        t->global
            .settings[SENT_SETTINGS][GRPC_CHTTP2_SETTINGS_INITIAL_WINDOW_SIZE];
    *t->accepting_stream = s;
    grpc_chttp2_list_add_incoming_window_updated(&t->global, &s->global);
    grpc_chttp2_stream_map_add(&t->new_stream_map, s->global.id, s);
    s->global.in_stream_map = 1;
  }

  if (initial_op) perform_op_locked(&t->global, &s->global, initial_op);
  unlock(t);

  return 0;
}

static void destroy_stream(grpc_transport *gt, grpc_stream *gs) {
  grpc_chttp2_transport *t = (grpc_chttp2_transport *)gt;
  grpc_chttp2_stream *s = (grpc_chttp2_stream *)gs;

  gpr_mu_lock(&t->mu);

  GPR_ASSERT(s->global.published_state == GRPC_STREAM_CLOSED ||
             s->global.id == 0);
  grpc_chttp2_unregister_stream(t, s);

  gpr_mu_unlock(&t->mu);

  GPR_ASSERT(s->global.outgoing_sopb == NULL);
  GPR_ASSERT(s->global.publish_sopb == NULL);
  grpc_sopb_destroy(&s->writing.sopb);
  grpc_chttp2_data_parser_destroy(&s->parsing.data_parser);
  grpc_chttp2_incoming_metadata_buffer_destroy(&s->parsing.incoming_metadata);
  grpc_chttp2_incoming_metadata_buffer_destroy(&s->global.incoming_metadata);

  unref_transport(t);
}

grpc_chttp2_stream_parsing *grpc_chttp2_parsing_lookup_stream(
    grpc_chttp2_transport_parsing *transport_parsing, gpr_uint32 id) {
  grpc_chttp2_transport *t = TRANSPORT_FROM_PARSING(transport_parsing);
  grpc_chttp2_stream *s =
      grpc_chttp2_stream_map_find(&t->parsing_stream_map, id);
  return s ? &s->parsing : NULL;
}

grpc_chttp2_stream_parsing *grpc_chttp2_parsing_accept_stream(
    grpc_chttp2_transport_parsing *transport_parsing, gpr_uint32 id) {
  grpc_chttp2_stream *accepting;
  grpc_chttp2_transport *t = TRANSPORT_FROM_PARSING(transport_parsing);
  GPR_ASSERT(t->accepting_stream == NULL);
  t->accepting_stream = &accepting;
  t->channel_callback.cb->accept_stream(t->channel_callback.cb_user_data,
                                        &t->base, (void *)(gpr_uintptr)id);
  t->accepting_stream = NULL;
  return &accepting->parsing;
}

#if 0
static void remove_from_stream_map(grpc_chttp2_transport *t, grpc_chttp2_stream *s) {
  if (s->global.id == 0) return;
  IF_TRACING(gpr_log(GPR_DEBUG, "HTTP:%s: Removing grpc_chttp2_stream %d",
                     t->global.is_client ? "CLI" : "SVR", s->global.id));
  if (grpc_chttp2_stream_map_delete(&t->stream_map, s->global.id)) {
    maybe_start_some_streams(t);
  }
}
#endif

/*
 * LOCK MANAGEMENT
 */

/* We take a grpc_chttp2_transport-global lock in response to calls coming in
   from above,
   and in response to data being received from below. New data to be written
   is always queued, as are callbacks to process data. During unlock() we
   check our todo lists and initiate callbacks and flush writes. */

static void lock(grpc_chttp2_transport *t) { gpr_mu_lock(&t->mu); }

static void unlock(grpc_chttp2_transport *t) {
  grpc_iomgr_closure *run_closures;

  if (!t->writing_active &&
      grpc_chttp2_unlocking_check_writes(&t->global, &t->writing)) {
    t->writing_active = 1;
    ref_transport(t);
    grpc_chttp2_schedule_closure(&t->global, &t->writing_action, 1);
  }
  unlock_check_cancellations(t);
  unlock_check_reads(t);
  /* unlock_check_parser(t); */
  unlock_check_channel_callbacks(t);

  run_closures = t->global.pending_closures;
  t->global.pending_closures = NULL;

  gpr_mu_unlock(&t->mu);

  while (run_closures) {
    grpc_iomgr_closure *next = run_closures->next;
    run_closures->cb(run_closures->cb_arg, run_closures->success);
    run_closures = next;
  }
}

/*
 * OUTPUT PROCESSING
 */

static void push_setting(grpc_chttp2_transport *t, grpc_chttp2_setting_id id,
                         gpr_uint32 value) {
  const grpc_chttp2_setting_parameters *sp =
      &grpc_chttp2_settings_parameters[id];
  gpr_uint32 use_value = GPR_CLAMP(value, sp->min_value, sp->max_value);
  if (use_value != value) {
    gpr_log(GPR_INFO, "Requested parameter %s clamped from %d to %d", sp->name,
            value, use_value);
  }
  if (use_value != t->global.settings[LOCAL_SETTINGS][id]) {
    t->global.settings[LOCAL_SETTINGS][id] = use_value;
    t->global.dirtied_local_settings = 1;
  }
}

void grpc_chttp2_terminate_writing(
    grpc_chttp2_transport_writing *transport_writing, int success) {
  grpc_chttp2_transport *t = TRANSPORT_FROM_WRITING(transport_writing);

  lock(t);

  if (!success) {
    drop_connection(t);
  }

  /* cleanup writing related jazz */
  grpc_chttp2_cleanup_writing(&t->global, &t->writing);

  /* leave the writing flag up on shutdown to prevent further writes in unlock()
     from starting */
  t->writing_active = 0;
  if (!t->endpoint_reading) {
    grpc_endpoint_destroy(t->ep);
    t->ep = NULL;
    unref_transport(t); /* safe because we'll still have the ref for write */
  }

  unlock(t);

  unref_transport(t);
}

static void writing_action(void *gt, int iomgr_success_ignored) {
  grpc_chttp2_transport *t = gt;
  grpc_chttp2_perform_writes(&t->writing, t->ep);
}

void grpc_chttp2_add_incoming_goaway(
    grpc_chttp2_transport_global *transport_global, gpr_uint32 goaway_error,
    gpr_slice goaway_text) {
  if (transport_global->goaway_state == GRPC_CHTTP2_ERROR_STATE_NONE) {
    transport_global->goaway_state = GRPC_CHTTP2_ERROR_STATE_NOTIFIED;
    transport_global->goaway_text = goaway_text;
    transport_global->goaway_error = goaway_error;
  } else {
    gpr_slice_unref(goaway_text);
  }
}

static void maybe_start_some_streams(
    grpc_chttp2_transport_global *transport_global) {
  grpc_chttp2_stream_global *stream_global;
  /* start streams where we have free grpc_chttp2_stream ids and free
   * concurrency */
  while (transport_global->next_stream_id <= MAX_CLIENT_STREAM_ID &&
         transport_global->concurrent_stream_count <
             transport_global->settings
                 [PEER_SETTINGS][GRPC_CHTTP2_SETTINGS_MAX_CONCURRENT_STREAMS] &&
         grpc_chttp2_list_pop_waiting_for_concurrency(transport_global,
                                                      &stream_global)) {
    IF_TRACING(gpr_log(GPR_DEBUG,
                       "HTTP:%s: Allocating new grpc_chttp2_stream %p to id %d",
                       transport_global->is_client ? "CLI" : "SVR",
                       stream_global, transport_global->next_stream_id));

    if (transport_global->next_stream_id == MAX_CLIENT_STREAM_ID) {
      grpc_chttp2_add_incoming_goaway(
          transport_global, GRPC_CHTTP2_NO_ERROR,
          gpr_slice_from_copied_string("Exceeded sequence number limit"));
    }

    GPR_ASSERT(stream_global->id == 0);
    stream_global->id = transport_global->next_stream_id;
    transport_global->next_stream_id += 2;
    stream_global->outgoing_window =
        transport_global
            ->settings[PEER_SETTINGS][GRPC_CHTTP2_SETTINGS_INITIAL_WINDOW_SIZE];
    stream_global->incoming_window =
        transport_global
            ->settings[SENT_SETTINGS][GRPC_CHTTP2_SETTINGS_INITIAL_WINDOW_SIZE];
    grpc_chttp2_stream_map_add(
        &TRANSPORT_FROM_GLOBAL(transport_global)->new_stream_map,
        stream_global->id, STREAM_FROM_GLOBAL(stream_global));
    stream_global->in_stream_map = 1;
    transport_global->concurrent_stream_count++;
    grpc_chttp2_list_add_incoming_window_updated(transport_global, stream_global);
    grpc_chttp2_list_add_writable_stream(transport_global, stream_global);
  }
  /* cancel out streams that will never be started */
  while (transport_global->next_stream_id > MAX_CLIENT_STREAM_ID &&
         grpc_chttp2_list_pop_waiting_for_concurrency(transport_global,
                                                      &stream_global)) {
    cancel_from_api(transport_global, stream_global, GRPC_STATUS_UNAVAILABLE);
  }
}

static void perform_op_locked(grpc_chttp2_transport_global *transport_global,
                              grpc_chttp2_stream_global *stream_global,
                              grpc_transport_op *op) {
  if (op->cancel_with_status != GRPC_STATUS_OK) {
    cancel_from_api(transport_global, stream_global, op->cancel_with_status);
  }

  if (op->send_ops) {
    GPR_ASSERT(stream_global->outgoing_sopb == NULL);
    stream_global->send_done_closure = op->on_done_send;
    if (!stream_global->cancelled) {
      stream_global->outgoing_sopb = op->send_ops;
      if (op->is_last_send && stream_global->write_state == WRITE_STATE_OPEN) {
        stream_global->write_state = WRITE_STATE_QUEUED_CLOSE;
      }
      if (stream_global->id == 0) {
        IF_TRACING(gpr_log(
            GPR_DEBUG,
            "HTTP:%s: New grpc_chttp2_stream %p waiting for concurrency",
            transport_global->is_client ? "CLI" : "SVR", stream_global));
        grpc_chttp2_list_add_waiting_for_concurrency(transport_global,
                                                     stream_global);
        maybe_start_some_streams(transport_global);
      } else if (stream_global->outgoing_window > 0) {
        grpc_chttp2_list_add_writable_stream(transport_global, stream_global);
      }
    } else {
      grpc_sopb_reset(op->send_ops);
      grpc_chttp2_schedule_closure(transport_global,
                                   stream_global->send_done_closure, 0);
    }
  }

  if (op->recv_ops) {
    GPR_ASSERT(stream_global->publish_sopb == NULL);
    GPR_ASSERT(stream_global->published_state != GRPC_STREAM_CLOSED);
    stream_global->recv_done_closure = op->on_done_recv;
    stream_global->publish_sopb = op->recv_ops;
    stream_global->publish_sopb->nops = 0;
    stream_global->publish_state = op->recv_state;
    grpc_chttp2_incoming_metadata_live_op_buffer_end(
        &stream_global->outstanding_metadata);
    grpc_chttp2_list_add_read_write_state_changed(transport_global,
                                                  stream_global);
    grpc_chttp2_list_add_incoming_window_state_changed(transport_global,
                                                       stream_global);
  }

  if (op->bind_pollset) {
    add_to_pollset_locked(TRANSPORT_FROM_GLOBAL(transport_global),
                          op->bind_pollset);
  }

  if (op->on_consumed) {
    grpc_chttp2_schedule_closure(transport_global, op->on_consumed, 1);
  }
}

static void perform_op(grpc_transport *gt, grpc_stream *gs,
                       grpc_transport_op *op) {
  grpc_chttp2_transport *t = (grpc_chttp2_transport *)gt;
  grpc_chttp2_stream *s = (grpc_chttp2_stream *)gs;

  lock(t);
  perform_op_locked(&t->global, &s->global, op);
  unlock(t);
}

static void send_ping(grpc_transport *gt, grpc_iomgr_closure *on_recv) {
  grpc_chttp2_transport *t = (grpc_chttp2_transport *)gt;
  grpc_chttp2_outstanding_ping *p = gpr_malloc(sizeof(*p));

  lock(t);
  p->next = &t->global.pings;
  p->prev = p->next->prev;
  p->prev->next = p->next->prev = p;
  p->id[0] = (t->global.ping_counter >> 56) & 0xff;
  p->id[1] = (t->global.ping_counter >> 48) & 0xff;
  p->id[2] = (t->global.ping_counter >> 40) & 0xff;
  p->id[3] = (t->global.ping_counter >> 32) & 0xff;
  p->id[4] = (t->global.ping_counter >> 24) & 0xff;
  p->id[5] = (t->global.ping_counter >> 16) & 0xff;
  p->id[6] = (t->global.ping_counter >> 8) & 0xff;
  p->id[7] = t->global.ping_counter & 0xff;
  p->on_recv = on_recv;
  gpr_slice_buffer_add(&t->global.qbuf, grpc_chttp2_ping_create(0, p->id));
  unlock(t);
}

/*
 * INPUT PROCESSING
 */

static void unlock_check_cancellations(grpc_chttp2_transport *t) {
  grpc_chttp2_transport_global *transport_global = &t->global;
  grpc_chttp2_stream_global *stream_global;

  /* if a stream is in the stream map, and gets cancelled, we need to ensure
     we are not parsing before continuing the cancellation to keep things in
     a sane state */
  if (!t->parsing_active) {
    while (grpc_chttp2_list_pop_cancelled_waiting_for_parsing(transport_global,
                                                              &stream_global)) {
      GPR_ASSERT(stream_global->in_stream_map);
      grpc_chttp2_stream_map_delete(&t->parsing_stream_map, stream_global->id);
      stream_global->in_stream_map = 0;
      grpc_chttp2_list_add_read_write_state_changed(transport_global,
                                                    stream_global);
    }
  }

#if 0  
  grpc_chttp2_stream *s;

  if (t->writing_active) {
    return;
  }

  while ((s = stream_list_remove_head(t, CANCELLED))) {
    s->global.read_closed = 1;
    s->global.write_state = WRITE_STATE_SENT_CLOSE;
    grpc_chttp2_list_add_read_write_state_changed(&t->global, &s->global);
  }
#endif
}

static grpc_stream_state compute_state(gpr_uint8 write_closed,
                                       gpr_uint8 read_closed) {
  if (write_closed && read_closed) return GRPC_STREAM_CLOSED;
  if (write_closed) return GRPC_STREAM_SEND_CLOSED;
  if (read_closed) return GRPC_STREAM_RECV_CLOSED;
  return GRPC_STREAM_OPEN;
}

static void unlock_check_reads(grpc_chttp2_transport *t) {
  grpc_chttp2_stream_global *stream_global;
  grpc_stream_state state;

  while (grpc_chttp2_list_pop_read_write_state_changed(&t->global, &stream_global)) {
    if (!stream_global->publish_sopb) {
      continue;
    }
    state = compute_state(stream_global->write_state == WRITE_STATE_SENT_CLOSE, stream_global->read_closed && !stream_global->in_stream_map);
    gpr_log(GPR_DEBUG, "ws:%d rc:%d ism:%d => st:%d", stream_global->write_state, stream_global->read_closed, stream_global->in_stream_map, state);
    if (stream_global->incoming_sopb.nops == 0 && state == stream_global->published_state) {
      continue;
    }
    grpc_chttp2_incoming_metadata_buffer_postprocess_sopb_and_begin_live_op(&stream_global->incoming_metadata, &stream_global->incoming_sopb, &stream_global->outstanding_metadata);
    grpc_sopb_swap(stream_global->publish_sopb, &stream_global->incoming_sopb);
    stream_global->published_state = *stream_global->publish_state = state;
    grpc_chttp2_schedule_closure(&t->global, stream_global->recv_done_closure, 1);
    stream_global->recv_done_closure = NULL;
    stream_global->publish_sopb = NULL;
    stream_global->publish_state = NULL;
  }
}

static void cancel_from_api(grpc_chttp2_transport_global *transport_global,
                            grpc_chttp2_stream_global *stream_global,
                            grpc_status_code status) {
  stream_global->cancelled = 1;
  if (stream_global->in_stream_map) {
    gpr_slice_buffer_add(&transport_global->qbuf,
                         grpc_chttp2_rst_stream_create(
                             stream_global->id,
                             grpc_chttp2_grpc_status_to_http2_status(status)));
  } else {
    grpc_chttp2_list_add_read_write_state_changed(transport_global,
                                                  stream_global);
  }
}

#if 0
static void cancel_stream_inner(grpc_chttp2_transport *t, grpc_chttp2_stream *s, gpr_uint32 id,
                                grpc_status_code local_status,
                                grpc_chttp2_error_code error_code,
                                grpc_mdstr *optional_message, int send_rst,
                                int is_parser) {
  int had_outgoing;
  char buffer[GPR_LTOA_MIN_BUFSIZE];

  if (s) {
    /* clear out any unreported input & output: nobody cares anymore */
    had_outgoing = s->global.outgoing_sopb && s->global.outgoing_sopb->nops != 0;
    if (error_code != GRPC_CHTTP2_NO_ERROR) {
      schedule_nuke_sopb(t, &s->parser.incoming_sopb);
      if (s->global.outgoing_sopb) {
        schedule_nuke_sopb(t, s->global.outgoing_sopb);
        s->global.outgoing_sopb = NULL;
        stream_list_remove(t, s, WRITABLE);
        grpc_chttp2_schedule_closure(t, s->global.send_done_closure, 0);
      }
    }
    if (s->cancelled) {
      send_rst = 0;
    } else if (!s->read_closed || s->write_state != WRITE_STATE_SENT_CLOSE ||
               had_outgoing) {
      s->cancelled = 1;
      stream_list_join(t, s, CANCELLED);

      if (error_code != GRPC_CHTTP2_NO_ERROR) {
        /* synthesize a status if we don't believe we'll get one */
        gpr_ltoa(local_status, buffer);
        add_incoming_metadata(
            t, s, grpc_mdelem_from_strings(t->metadata_context, "grpc-status",
                                           buffer));
        if (!optional_message) {
          switch (local_status) {
            case GRPC_STATUS_CANCELLED:
              add_incoming_metadata(
                  t, s, grpc_mdelem_from_strings(t->metadata_context,
                                                 "grpc-message", "Cancelled"));
              break;
            default:
              break;
          }
        } else {
          add_incoming_metadata(
              t, s,
              grpc_mdelem_from_metadata_strings(
                  t->metadata_context,
                  grpc_mdstr_from_string(t->metadata_context, "grpc-message"),
                  grpc_mdstr_ref(optional_message)));
        }
        add_metadata_batch(t, s);
      }
    }
    maybe_finish_read(t, s, is_parser);
  }
  if (!id) send_rst = 0;
  if (send_rst) {
    gpr_slice_buffer_add(&t->global.qbuf,
                         grpc_chttp2_rst_stream_create(id, error_code));
  }
  if (optional_message) {
    grpc_mdstr_unref(optional_message);
  }
}

static void cancel_stream_id(grpc_chttp2_transport *t, gpr_uint32 id,
                             grpc_status_code local_status,
                             grpc_chttp2_error_code error_code, int send_rst) {
  lock(t);
  cancel_stream_inner(t, lookup_stream(t, id), id, local_status, error_code,
                      NULL, send_rst, 1);
  unlock(t);
}

static void cancel_stream(grpc_chttp2_transport *t, grpc_chttp2_stream *s,
                          grpc_status_code local_status,
                          grpc_chttp2_error_code error_code,
                          grpc_mdstr *optional_message, int send_rst) {
  cancel_stream_inner(t, s, s->global.id, local_status, error_code, optional_message,
                      send_rst, 0);
}
#endif

static void cancel_stream_cb(grpc_chttp2_transport_global *transport_global,
                             void *user_data,
                             grpc_chttp2_stream_global *stream_global) {
  cancel_from_api(transport_global, stream_global, GRPC_STATUS_UNAVAILABLE);
}

static void end_all_the_calls(grpc_chttp2_transport *t) {
  grpc_chttp2_for_all_streams(&t->global, NULL, cancel_stream_cb);
}

static void drop_connection(grpc_chttp2_transport *t) {
  if (t->global.error_state == GRPC_CHTTP2_ERROR_STATE_NONE) {
    t->global.error_state = GRPC_CHTTP2_ERROR_STATE_SEEN;
  }
  close_transport_locked(t);
  end_all_the_calls(t);
}

#if 0
static void maybe_finish_read(grpc_chttp2_transport *t, grpc_chttp2_stream *s, int is_parser) {
  if (is_parser) {
    stream_list_join(t, s, MAYBE_FINISH_READ_AFTER_PARSE);
  } else if (s->incoming_sopb) {
    stream_list_join(t, s, FINISHED_READ_OP);
  }
}

static void maybe_join_window_updates(grpc_chttp2_transport *t,
                                      grpc_chttp2_stream *s) {
  if (t->parsing.executing) {
    stream_list_join(t, s, OTHER_CHECK_WINDOW_UPDATES_AFTER_PARSE);
    return;
  }
  if (s->incoming_sopb != NULL &&
      s->global.incoming_window <
          t->global.settings[LOCAL_SETTINGS]
                            [GRPC_CHTTP2_SETTINGS_INITIAL_WINDOW_SIZE] *
              3 / 4) {
    stream_list_join(t, s, WINDOW_UPDATE);
  }
}

static grpc_chttp2_stream *lookup_stream(grpc_chttp2_transport *t, gpr_uint32 id) {
  return grpc_chttp2_stream_map_find(&t->stream_map, id);
}
#endif

/* tcp read callback */
static void recv_data(void *tp, gpr_slice *slices, size_t nslices,
                      grpc_endpoint_cb_status error) {
  grpc_chttp2_transport *t = tp;
  size_t i;

  switch (error) {
    case GRPC_ENDPOINT_CB_SHUTDOWN:
    case GRPC_ENDPOINT_CB_EOF:
    case GRPC_ENDPOINT_CB_ERROR:
      lock(t);
      drop_connection(t);
      t->endpoint_reading = 0;
      if (!t->writing_active && t->ep) {
        grpc_endpoint_destroy(t->ep);
        t->ep = NULL;
        unref_transport(t); /* safe as we still have a ref for read */
      }
      unlock(t);
      unref_transport(t);
      for (i = 0; i < nslices; i++) gpr_slice_unref(slices[i]);
      break;
    case GRPC_ENDPOINT_CB_OK:
      lock(t);
      i = 0;
      GPR_ASSERT(!t->parsing_active);
      if (t->global.error_state == GRPC_CHTTP2_ERROR_STATE_NONE) {
        t->parsing_active = 1;
        grpc_chttp2_prepare_to_read(&t->global, &t->parsing);
        gpr_mu_unlock(&t->mu);
        for (; i < nslices && grpc_chttp2_perform_read(&t->parsing, slices[i]);
             i++) {
          gpr_slice_unref(slices[i]);
        }
        gpr_mu_lock(&t->mu);
        if (i != nslices) {
          drop_connection(t);
        }
        /* merge stream lists */
        grpc_chttp2_stream_map_move_into(&t->new_stream_map,
                                         &t->parsing_stream_map);
        /* handle higher level things */
        grpc_chttp2_publish_reads(&t->global, &t->parsing);
        t->global.concurrent_stream_count =
            grpc_chttp2_stream_map_size(&t->parsing_stream_map);
        t->parsing_active = 0;
      }
#if 0
      while ((s = stream_list_remove_head(t, MAYBE_FINISH_READ_AFTER_PARSE))) {
        maybe_finish_read(t, s, 0);
      }
      while ((s = stream_list_remove_head(t, PARSER_CHECK_WINDOW_UPDATES_AFTER_PARSE))) {
        maybe_join_window_updates(t, s);
      }
      while ((s = stream_list_remove_head(t, OTHER_CHECK_WINDOW_UPDATES_AFTER_PARSE))) {
        maybe_join_window_updates(t, s);
      }
      while ((s = stream_list_remove_head(t, NEW_OUTGOING_WINDOW))) {
        int was_window_empty = s->global.outgoing_window <= 0;
        FLOWCTL_TRACE(t, s, outgoing, s->global.id, s->global.outgoing_window_update);
        s->global.outgoing_window += s->global.outgoing_window_update;
        s->global.outgoing_window_update = 0;
        /* if this window update makes outgoing ops writable again,
           flag that */
        if (was_window_empty && s->global.outgoing_sopb &&
            s->global.outgoing_sopb->nops > 0) {
          stream_list_join(t, s, WRITABLE);
        }
      }
      t->global.outgoing_window += t->global.outgoing_window_update;
      t->global.outgoing_window_update = 0;
      maybe_start_some_streams(t);
#endif
      if (i == nslices) {
        grpc_endpoint_notify_on_read(t->ep, recv_data, t);
      }
      unlock(t);
      for (; i < nslices; i++) gpr_slice_unref(slices[i]);
      break;
  }
}

/*
 * CALLBACK LOOP
 */

typedef struct {
  grpc_chttp2_transport *t;
  gpr_uint32 error;
  gpr_slice text;
  grpc_iomgr_closure closure;
} notify_goaways_args;

static void notify_goaways(void *p, int iomgr_success_ignored) {
  notify_goaways_args *a = p;
  grpc_chttp2_transport *t = a->t;

  t->channel_callback.cb->goaway(t->channel_callback.cb_user_data, &t->base,
                                 a->error, a->text);

  gpr_free(a);

  lock(t);
  t->channel_callback.executing = 0;
  unlock(t);

  unref_transport(t);
}

static void notify_closed(void *gt, int iomgr_success_ignored) {
  grpc_chttp2_transport *t = gt;
  t->channel_callback.cb->closed(t->channel_callback.cb_user_data, &t->base);

  lock(t);
  t->channel_callback.executing = 0;
  unlock(t);

  unref_transport(t);
}

static void unlock_check_channel_callbacks(grpc_chttp2_transport *t) {
  if (t->channel_callback.executing) {
    return;
  }
  if (t->global.goaway_state != GRPC_CHTTP2_ERROR_STATE_NONE) {
    if (t->global.goaway_state == GRPC_CHTTP2_ERROR_STATE_SEEN &&
        t->global.error_state != GRPC_CHTTP2_ERROR_STATE_NOTIFIED) {
      notify_goaways_args *a = gpr_malloc(sizeof(*a));
      a->error = t->global.goaway_error;
      a->text = t->global.goaway_text;
      t->global.goaway_state = GRPC_CHTTP2_ERROR_STATE_NOTIFIED;
      t->channel_callback.executing = 1;
      grpc_iomgr_closure_init(&a->closure, notify_goaways, a);
      ref_transport(t);
      grpc_chttp2_schedule_closure(&t->global, &a->closure, 1);
      return;
    } else if (t->global.goaway_state != GRPC_CHTTP2_ERROR_STATE_NOTIFIED) {
      return;
    }
  }
  if (t->global.error_state == GRPC_CHTTP2_ERROR_STATE_SEEN) {
    t->global.error_state = GRPC_CHTTP2_ERROR_STATE_NOTIFIED;
    t->channel_callback.executing = 1;
    ref_transport(t);
    grpc_chttp2_schedule_closure(&t->global, &t->channel_callback.notify_closed,
                                 1);
  }
}

void grpc_chttp2_schedule_closure(
    grpc_chttp2_transport_global *transport_global, grpc_iomgr_closure *closure,
    int success) {
  closure->success = success;
  closure->next = transport_global->pending_closures;
  transport_global->pending_closures = closure;
}

/*
 * POLLSET STUFF
 */

static void add_to_pollset_locked(grpc_chttp2_transport *t,
                                  grpc_pollset *pollset) {
  if (t->ep) {
    grpc_endpoint_add_to_pollset(t->ep, pollset);
  }
}

static void add_to_pollset(grpc_transport *gt, grpc_pollset *pollset) {
  grpc_chttp2_transport *t = (grpc_chttp2_transport *)gt;
  lock(t);
  add_to_pollset_locked(t, pollset);
  unlock(t);
}

/*
 * INTEGRATION GLUE
 */

static const grpc_transport_vtable vtable = {sizeof(grpc_chttp2_stream),
                                             init_stream,
                                             perform_op,
                                             add_to_pollset,
                                             destroy_stream,
                                             goaway,
                                             close_transport,
                                             send_ping,
                                             destroy_transport};

void grpc_create_chttp2_transport(grpc_transport_setup_callback setup,
                                  void *arg,
                                  const grpc_channel_args *channel_args,
                                  grpc_endpoint *ep, gpr_slice *slices,
                                  size_t nslices, grpc_mdctx *mdctx,
                                  int is_client) {
  grpc_chttp2_transport *t = gpr_malloc(sizeof(grpc_chttp2_transport));
  init_transport(t, setup, arg, channel_args, ep, slices, nslices, mdctx,
                 is_client);
}
