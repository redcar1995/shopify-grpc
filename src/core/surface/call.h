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

#ifndef GRPC_INTERNAL_CORE_SURFACE_CALL_H
#define GRPC_INTERNAL_CORE_SURFACE_CALL_H

#include "src/core/channel/channel_stack.h"
#include <grpc/grpc.h>

/* Primitive operation types - grpc_op's get rewritten into these */
typedef enum {
  GRPC_IOREQ_RECV_INITIAL_METADATA,
  GRPC_IOREQ_RECV_MESSAGE,
  GRPC_IOREQ_RECV_TRAILING_METADATA,
  GRPC_IOREQ_RECV_STATUS,
  GRPC_IOREQ_RECV_STATUS_DETAILS,
  GRPC_IOREQ_RECV_CLOSE,
  GRPC_IOREQ_SEND_INITIAL_METADATA,
  GRPC_IOREQ_SEND_MESSAGE,
  GRPC_IOREQ_SEND_TRAILING_METADATA,
  GRPC_IOREQ_SEND_STATUS,
  GRPC_IOREQ_SEND_CLOSE,
  GRPC_IOREQ_OP_COUNT
} grpc_ioreq_op;

typedef union {
  grpc_metadata_array *recv_metadata;
  grpc_byte_buffer **recv_message;
  struct {
    void (*set_value)(grpc_status_code status, void *user_data);
    void *user_data;
  } recv_status;
  struct {
    char **details;
    size_t *details_capacity;
  } recv_status_details;
  struct {
    size_t count;
    grpc_metadata *metadata;
  } send_metadata;
  grpc_byte_buffer *send_message;
  struct {
    grpc_status_code code;
    const char *details;
  } send_status;
} grpc_ioreq_data;

typedef struct {
  grpc_ioreq_op op;
  grpc_ioreq_data data;
} grpc_ioreq;

typedef void (*grpc_ioreq_completion_func)(grpc_call *call, int success,
                                           void *user_data);

grpc_call *grpc_call_create(grpc_channel *channel, grpc_completion_queue *cq,
                            const void *server_transport_data,
                            grpc_mdelem **add_initial_metadata,
                            size_t add_initial_metadata_count,
                            gpr_timespec send_deadline);

void grpc_call_set_completion_queue(grpc_call *call, grpc_completion_queue *cq);
grpc_completion_queue *grpc_call_get_completion_queue(grpc_call *call);

#ifdef GRPC_CALL_REF_COUNT_DEBUG
void grpc_call_internal_ref(grpc_call *call, const char *reason);
void grpc_call_internal_unref(grpc_call *call, const char *reason, int allow_immediate_deletion);
#define GRPC_CALL_INTERNAL_REF(call, reason) grpc_call_internal_ref(call, reason)
#define GRPC_CALL_INTERNAL_UNREF(call, reason, allow_immediate_deletion) grpc_call_internal_unref(call, reason, allow_immediate_deletion)
#else
void grpc_call_internal_ref(grpc_call *call);
void grpc_call_internal_unref(grpc_call *call, int allow_immediate_deletion);
#define GRPC_CALL_INTERNAL_REF(call, reason) grpc_call_internal_ref(call)
#define GRPC_CALL_INTERNAL_UNREF(call, reason, allow_immediate_deletion) grpc_call_internal_unref(call, allow_immediate_deletion)
#endif

grpc_call_error grpc_call_start_ioreq_and_call_back(
    grpc_call *call, const grpc_ioreq *reqs, size_t nreqs,
    grpc_ioreq_completion_func on_complete, void *user_data);

grpc_call_stack *grpc_call_get_call_stack(grpc_call *call);

/* Given the top call_element, get the call object. */
grpc_call *grpc_call_from_top_element(grpc_call_element *surface_element);

extern int grpc_trace_batch;

void grpc_call_log_batch(char *file, int line, gpr_log_severity severity,
                         grpc_call *call, const grpc_op *ops, size_t nops,
                         void *tag);

#define GRPC_CALL_LOG_BATCH(sev, call, ops, nops, tag) \
  if (grpc_trace_batch) grpc_call_log_batch(sev, call, ops, nops, tag)

#endif /* GRPC_INTERNAL_CORE_SURFACE_CALL_H */
