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

#include "src/core/channel/census_filter.h"

#include <stdio.h>
#include <string.h>

#include "include/grpc/census.h"
#include "src/core/census/rpc_stat_id.h"
#include "src/core/channel/channel_stack.h"
#include "src/core/channel/noop_filter.h"
#include "src/core/statistics/census_interface.h"
#include "src/core/statistics/census_rpc_stats.h"
#include <grpc/support/alloc.h>
#include <grpc/support/log.h>
#include <grpc/support/slice.h>
#include <grpc/support/time.h>

typedef struct call_data {
  census_op_id op_id;
  census_context* ctxt;
  gpr_timespec start_ts;
  int error;

  /* recv callback */
  grpc_stream_op_buffer* recv_ops;
  grpc_iomgr_closure* on_done_recv;
} call_data;

typedef struct channel_data {
  grpc_mdstr* path_str; /* pointer to meta data str with key == ":path" */
} channel_data;

static void extract_and_annotate_method_tag(grpc_stream_op_buffer* sopb,
                                            call_data* calld,
                                            channel_data* chand) {
  grpc_linked_mdelem* m;
  size_t i;
  for (i = 0; i < sopb->nops; i++) {
    grpc_stream_op* op = &sopb->ops[i];
    if (op->type != GRPC_OP_METADATA) continue;
    for (m = op->data.metadata.list.head; m != NULL; m = m->next) {
      if (m->md->key == chand->path_str) {
        gpr_log(GPR_DEBUG, "%s",
                (const char*)GPR_SLICE_START_PTR(m->md->value->slice));
        /* Add method tag here */
      }
    }
  }
}

static void client_mutate_op(grpc_call_element* elem,
                             grpc_transport_stream_op* op) {
  call_data* calld = elem->call_data;
  channel_data* chand = elem->channel_data;
  if (op->send_ops) {
    extract_and_annotate_method_tag(op->send_ops, calld, chand);
  }
}

static void client_start_transport_op(grpc_call_element* elem,
                                      grpc_transport_stream_op* op) {
  client_mutate_op(elem, op);
  grpc_call_next_op(elem, op);
}

static void server_on_done_recv(void* ptr, int success) {
  grpc_call_element* elem = ptr;
  call_data* calld = elem->call_data;
  channel_data* chand = elem->channel_data;
  if (success) {
    extract_and_annotate_method_tag(calld->recv_ops, calld, chand);
  }
  calld->on_done_recv->cb(calld->on_done_recv->cb_arg, success);
}

static void server_mutate_op(grpc_call_element* elem,
                             grpc_transport_stream_op* op) {
  call_data* calld = elem->call_data;
  if (op->recv_ops) {
    /* substitute our callback for the op callback */
    calld->recv_ops = op->recv_ops;
    calld->on_done_recv = op->on_done_recv;
    op->on_done_recv = calld->on_done_recv;
  }
}

static void server_start_transport_op(grpc_call_element* elem,
                                      grpc_transport_stream_op* op) {
  call_data* calld = elem->call_data;
  GPR_ASSERT((calld->op_id.upper != 0) || (calld->op_id.lower != 0));
  server_mutate_op(elem, op);
  grpc_call_next_op(elem, op);
}

static void client_init_call_elem(grpc_call_element* elem,
                                  const void* server_transport_data,
                                  grpc_transport_stream_op* initial_op) {
  call_data* d = elem->call_data;
  GPR_ASSERT(d != NULL);
  d->start_ts = gpr_now(GPR_CLOCK_REALTIME);
  if (initial_op) client_mutate_op(elem, initial_op);
}

static void client_destroy_call_elem(grpc_call_element* elem) {
  call_data* d = elem->call_data;
  GPR_ASSERT(d != NULL);
  /* TODO(hongyu): record rpc client stats and census_rpc_end_op here */
}

static void server_init_call_elem(grpc_call_element* elem,
                                  const void* server_transport_data,
                                  grpc_transport_stream_op* initial_op) {
  call_data* d = elem->call_data;
  GPR_ASSERT(d != NULL);
  d->start_ts = gpr_now(GPR_CLOCK_REALTIME);
  /* TODO(hongyu): call census_tracing_start_op here. */
  grpc_iomgr_closure_init(d->on_done_recv, server_on_done_recv, elem);
  if (initial_op) server_mutate_op(elem, initial_op);
}

static void server_destroy_call_elem(grpc_call_element* elem) {
  call_data* d = elem->call_data;
  GPR_ASSERT(d != NULL);
  /* TODO(hongyu): record rpc server stats and census_tracing_end_op here */
}

static void init_channel_elem(grpc_channel_element* elem, grpc_channel* master,
                              const grpc_channel_args* args, grpc_mdctx* mdctx,
                              int is_first, int is_last) {
  channel_data* chand = elem->channel_data;
  GPR_ASSERT(chand != NULL);
  chand->path_str = grpc_mdstr_from_string(mdctx, ":path", 0);
}

static void destroy_channel_elem(grpc_channel_element* elem) {
  channel_data* chand = elem->channel_data;
  GPR_ASSERT(chand != NULL);
  if (chand->path_str != NULL) {
    GRPC_MDSTR_UNREF(chand->path_str);
  }
}

const grpc_channel_filter grpc_client_census_filter = {
    client_start_transport_op,
    grpc_channel_next_op,
    sizeof(call_data),
    client_init_call_elem,
    client_destroy_call_elem,
    sizeof(channel_data),
    init_channel_elem,
    destroy_channel_elem,
    grpc_call_next_get_peer,
    "census-client"};

const grpc_channel_filter grpc_server_census_filter = {
    server_start_transport_op,
    grpc_channel_next_op,
    sizeof(call_data),
    server_init_call_elem,
    server_destroy_call_elem,
    sizeof(channel_data),
    init_channel_elem,
    destroy_channel_elem,
    grpc_call_next_get_peer,
    "census-server"};
