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

#include "test/core/end2end/end2end_tests.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <grpc/byte_buffer.h>
#include <grpc/support/alloc.h>
#include <grpc/support/log.h>
#include <grpc/support/time.h>
#include <grpc/support/useful.h>
#include "test/core/end2end/cq_verifier.h"

enum { TIMEOUT = 200000 };

static void *tag(gpr_intptr t) { return (void *)t; }

static grpc_end2end_test_fixture begin_test(grpc_end2end_test_config config,
                                            const char *test_name,
                                            grpc_channel_args *client_args,
                                            grpc_channel_args *server_args) {
  grpc_end2end_test_fixture f;
  gpr_log(GPR_INFO, "%s/%s", test_name, config.name);
  f = config.create_fixture(client_args, server_args);
  config.init_client(&f, client_args);
  config.init_server(&f, server_args);
  return f;
}

static gpr_timespec n_seconds_time(int n) {
  return GRPC_TIMEOUT_SECONDS_TO_DEADLINE(n);
}

static gpr_timespec five_seconds_time(void) { return n_seconds_time(5); }

static void drain_cq(grpc_completion_queue *cq) {
  grpc_event *ev;
  grpc_completion_type type;
  do {
    ev = grpc_completion_queue_next(cq, five_seconds_time());
    GPR_ASSERT(ev);
    type = ev->type;
    grpc_event_finish(ev);
  } while (type != GRPC_QUEUE_SHUTDOWN);
}

static void shutdown_server(grpc_end2end_test_fixture *f) {
  if (!f->server) return;
  grpc_server_shutdown(f->server);
  grpc_server_destroy(f->server);
  f->server = NULL;
}

static void shutdown_client(grpc_end2end_test_fixture *f) {
  if (!f->client) return;
  grpc_channel_destroy(f->client);
  f->client = NULL;
}

static void end_test(grpc_end2end_test_fixture *f) {
  shutdown_server(f);
  shutdown_client(f);

  grpc_completion_queue_shutdown(f->server_cq);
  drain_cq(f->server_cq);
  grpc_completion_queue_destroy(f->server_cq);
  grpc_completion_queue_shutdown(f->client_cq);
  drain_cq(f->client_cq);
  grpc_completion_queue_destroy(f->client_cq);
}

/* Client pings and server pongs. Repeat messages rounds before finishing. */
static void test_pingpong_streaming(grpc_end2end_test_config config,
                                    int messages) {
  grpc_end2end_test_fixture f = begin_test(config, __FUNCTION__, NULL, NULL);
  grpc_call *c;
  grpc_call *s;
  gpr_timespec deadline = five_seconds_time();
  cq_verifier *v_client = cq_verifier_create(f.client_cq);
  cq_verifier *v_server = cq_verifier_create(f.server_cq);
  grpc_op ops[6];
  grpc_op *op;
  grpc_metadata_array initial_metadata_recv;
  grpc_metadata_array trailing_metadata_recv;
  grpc_metadata_array request_metadata_recv;
  grpc_call_details call_details;
  grpc_status_code status;
  char *details = NULL;
  size_t details_capacity = 0;
  int was_cancelled = 2;
  grpc_byte_buffer *request_payload;
  grpc_byte_buffer *request_payload_recv;
  grpc_byte_buffer *response_payload;
  grpc_byte_buffer *response_payload_recv;
  int i;
  gpr_slice request_payload_slice = gpr_slice_from_copied_string("hello world");
  gpr_slice response_payload_slice = gpr_slice_from_copied_string("hello you");

  c = grpc_channel_create_call(f.client, f.client_cq, "/foo",
                               "foo.test.google.fr:1234", deadline);
  GPR_ASSERT(c);

  grpc_metadata_array_init(&initial_metadata_recv);
  grpc_metadata_array_init(&trailing_metadata_recv);
  grpc_metadata_array_init(&request_metadata_recv);
  grpc_call_details_init(&call_details);

  op = ops;
  op->op = GRPC_OP_SEND_INITIAL_METADATA;
  op->data.send_initial_metadata.count = 0;
  op++;
  op->op = GRPC_OP_RECV_INITIAL_METADATA;
  op->data.recv_initial_metadata = &initial_metadata_recv;
  op++;
  op->op = GRPC_OP_RECV_STATUS_ON_CLIENT;
  op->data.recv_status_on_client.trailing_metadata = &trailing_metadata_recv;
  op->data.recv_status_on_client.status = &status;
  op->data.recv_status_on_client.status_details = &details;
  op->data.recv_status_on_client.status_details_capacity = &details_capacity;
  op++;
  GPR_ASSERT(GRPC_CALL_OK == grpc_call_start_batch(c, ops, op - ops, tag(1)));

  GPR_ASSERT(GRPC_CALL_OK == grpc_server_request_call(f.server, &s,
                                                      &call_details,
                                                      &request_metadata_recv,
                                                      f.server_cq, tag(100)));
  cq_expect_completion(v_server, tag(100), GRPC_OP_OK);
  cq_verify(v_server);

  op = ops;
  op->op = GRPC_OP_SEND_INITIAL_METADATA;
  op->data.send_initial_metadata.count = 0;
  op++;
  op->op = GRPC_OP_RECV_CLOSE_ON_SERVER;
  op->data.recv_close_on_server.cancelled = &was_cancelled;
  op++;
  GPR_ASSERT(GRPC_CALL_OK == grpc_call_start_batch(s, ops, op - ops, tag(101)));

  for (i = 0; i < messages; i++) {
    request_payload = grpc_byte_buffer_create(&request_payload_slice, 1);
    response_payload = grpc_byte_buffer_create(&response_payload_slice, 1);

    op = ops;
    op->op = GRPC_OP_SEND_MESSAGE;
    op->data.send_message = request_payload;
    op++;
    op->op = GRPC_OP_RECV_MESSAGE;
    op->data.recv_message = &response_payload_recv;
    op++;
    GPR_ASSERT(GRPC_CALL_OK == grpc_call_start_batch(c, ops, op - ops, tag(2)));

    op = ops;
    op->op = GRPC_OP_RECV_MESSAGE;
    op->data.recv_message = &request_payload_recv;
    op++;
    GPR_ASSERT(GRPC_CALL_OK == grpc_call_start_batch(s, ops, op - ops, tag(102)));
    cq_expect_completion(v_server, tag(102), GRPC_OP_OK);
    cq_verify(v_server);

    op = ops;
    op->op = GRPC_OP_SEND_MESSAGE;
    op->data.send_message = response_payload;
    op++;
    GPR_ASSERT(GRPC_CALL_OK == grpc_call_start_batch(s, ops, op - ops, tag(103)));
    cq_expect_completion(v_server, tag(103), GRPC_OP_OK);
    cq_verify(v_server);

    cq_expect_completion(v_client, tag(2), GRPC_OP_OK);
    cq_verify(v_client);

    grpc_byte_buffer_destroy(request_payload);
    grpc_byte_buffer_destroy(response_payload);
    grpc_byte_buffer_destroy(request_payload_recv);
    grpc_byte_buffer_destroy(response_payload_recv);
  }

  gpr_slice_unref(request_payload_slice);
  gpr_slice_unref(response_payload_slice);

  op = ops;
  op->op = GRPC_OP_SEND_CLOSE_FROM_CLIENT;
  op++;
  GPR_ASSERT(GRPC_CALL_OK == grpc_call_start_batch(c, ops, op - ops, tag(3)));

  op = ops;
  op->op = GRPC_OP_RECV_CLOSE_ON_SERVER;
  op->data.recv_close_on_server.cancelled = &was_cancelled;
  op++;
  op->op = GRPC_OP_SEND_STATUS_FROM_SERVER;
  op->data.send_status_from_server.trailing_metadata_count = 0;
  op->data.send_status_from_server.status = GRPC_STATUS_UNIMPLEMENTED;
  op->data.send_status_from_server.status_details = "xyz";
  op++;
  GPR_ASSERT(GRPC_CALL_OK == grpc_call_start_batch(s, ops, op - ops, tag(104)));

  cq_expect_completion(v_client, tag(1), GRPC_OP_OK);
  cq_expect_completion(v_client, tag(3), GRPC_OP_OK);
  cq_verify(v_client);

  cq_expect_completion(v_server, tag(101), GRPC_OP_OK);
  cq_expect_completion(v_server, tag(104), GRPC_OP_OK);
  cq_verify(v_server);

  grpc_call_destroy(c);
  grpc_call_destroy(s);

  end_test(&f);
  config.tear_down_data(&f);

  cq_verifier_destroy(v_client);
  cq_verifier_destroy(v_server);

  end_test(&f);
  config.tear_down_data(&f);
}

void grpc_end2end_tests(grpc_end2end_test_config config) {
  int i;

  for (i = 1; i < 10; i++) {
    test_pingpong_streaming(config, i);
  }
}
