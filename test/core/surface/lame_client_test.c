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

#include <grpc/grpc.h>

#include "test/core/end2end/cq_verifier.h"
#include "test/core/util/test_config.h"
#include <grpc/support/log.h>

static void *tag(gpr_intptr x) { return (void *)x; }

int main(int argc, char **argv) {
  grpc_channel *chan;
  grpc_call *call;
  grpc_completion_queue *cq;
  cq_verifier *cqv;
  grpc_op ops[6];
  grpc_op *op;
  grpc_metadata_array trailing_metadata_recv;
  grpc_status_code status;
  char *details = NULL;
  size_t details_capacity = 0;

  grpc_test_init(argc, argv);
  grpc_init();

  grpc_metadata_array_init(&trailing_metadata_recv);

  chan = grpc_lame_client_channel_create();
  GPR_ASSERT(chan);
  cq = grpc_completion_queue_create();
  call = grpc_channel_create_call(chan, cq, "/Foo", "anywhere",
                                  GRPC_TIMEOUT_SECONDS_TO_DEADLINE(100));
  GPR_ASSERT(call);
  cqv = cq_verifier_create(cq);

  op = ops;
  op->op = GRPC_OP_SEND_INITIAL_METADATA;
  op->data.send_initial_metadata.count = 0;
  op++;
  op->op = GRPC_OP_RECV_STATUS_ON_CLIENT;
  op->data.recv_status_on_client.trailing_metadata = &trailing_metadata_recv;
  op->data.recv_status_on_client.status = &status;
  op->data.recv_status_on_client.status_details = &details;
  op->data.recv_status_on_client.status_details_capacity = &details_capacity;
  op++;
  GPR_ASSERT(GRPC_CALL_OK ==
             grpc_call_start_batch(call, ops, op - ops, tag(1)));

  /* the call should immediately fail */
  cq_expect_completion(cqv, tag(1), GRPC_OP_OK);
  cq_verify(cqv);

  grpc_call_destroy(call);
  grpc_channel_destroy(chan);
  cq_verifier_destroy(cqv);
  grpc_completion_queue_destroy(cq);

  grpc_shutdown();

  return 0;
}
