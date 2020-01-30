/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "test/core/end2end/end2end_tests.h"

#include <stdio.h>
#include <string.h>

#include "src/core/lib/surface/channel.h"
#include "src/core/lib/surface/server.h"

#include <grpc/byte_buffer.h>
#include <grpc/grpc.h>
#include <grpc/support/alloc.h>
#include <grpc/support/log.h>
#include <grpc/support/time.h>
#include "src/core/lib/channel/channelz_registry.h"
#include "src/core/lib/gpr/string.h"
#include "test/core/end2end/cq_verifier.h"

static void* tag(intptr_t t) { return (void*)t; }

static grpc_end2end_test_fixture begin_test(grpc_end2end_test_config config,
                                            const char* test_name,
                                            grpc_channel_args* client_args,
                                            grpc_channel_args* server_args) {
  grpc_end2end_test_fixture f;
  gpr_log(GPR_INFO, "Running test: %s/%s", test_name, config.name);
  f = config.create_fixture(client_args, server_args);
  config.init_server(&f, server_args);
  config.init_client(&f, client_args);
  return f;
}

static gpr_timespec n_seconds_from_now(int n) {
  return grpc_timeout_seconds_to_deadline(n);
}

static gpr_timespec five_seconds_from_now(void) {
  return n_seconds_from_now(5);
}

static void drain_cq(grpc_completion_queue* cq) {
  grpc_event ev;
  do {
    ev = grpc_completion_queue_next(cq, five_seconds_from_now(), nullptr);
  } while (ev.type != GRPC_QUEUE_SHUTDOWN);
}

static void shutdown_server(grpc_end2end_test_fixture* f) {
  if (!f->server) return;
  grpc_server_shutdown_and_notify(f->server, f->shutdown_cq, tag(1000));
  GPR_ASSERT(grpc_completion_queue_pluck(f->shutdown_cq, tag(1000),
                                         grpc_timeout_seconds_to_deadline(5),
                                         nullptr)
                 .type == GRPC_OP_COMPLETE);
  grpc_server_destroy(f->server);
  f->server = nullptr;
}

static void shutdown_client(grpc_end2end_test_fixture* f) {
  if (!f->client) return;
  grpc_channel_destroy(f->client);
  f->client = nullptr;
}

static void end_test(grpc_end2end_test_fixture* f) {
  shutdown_server(f);
  shutdown_client(f);

  grpc_completion_queue_shutdown(f->cq);
  drain_cq(f->cq);
  grpc_completion_queue_destroy(f->cq);
  grpc_completion_queue_destroy(f->shutdown_cq);
}

static void run_one_request(grpc_end2end_test_config /*config*/,
                            grpc_end2end_test_fixture f,
                            bool request_is_success) {
  grpc_call* c;
  grpc_call* s;
  cq_verifier* cqv = cq_verifier_create(f.cq);
  grpc_op ops[6];
  grpc_op* op;
  grpc_metadata_array initial_metadata_recv;
  grpc_metadata_array trailing_metadata_recv;
  grpc_metadata_array request_metadata_recv;
  grpc_call_details call_details;
  grpc_status_code status;
  grpc_call_error error;
  grpc_slice details;
  int was_cancelled = 2;

  gpr_timespec deadline = five_seconds_from_now();
  c = grpc_channel_create_call(f.client, nullptr, GRPC_PROPAGATE_DEFAULTS, f.cq,
                               grpc_slice_from_static_string("/foo"), nullptr,
                               deadline, nullptr);
  GPR_ASSERT(c);

  grpc_metadata_array_init(&initial_metadata_recv);
  grpc_metadata_array_init(&trailing_metadata_recv);
  grpc_metadata_array_init(&request_metadata_recv);
  grpc_call_details_init(&call_details);

  memset(ops, 0, sizeof(ops));
  op = ops;
  op->op = GRPC_OP_SEND_INITIAL_METADATA;
  op->data.send_initial_metadata.count = 0;
  op->flags = 0;
  op->reserved = nullptr;
  op++;
  op->op = GRPC_OP_SEND_CLOSE_FROM_CLIENT;
  op->flags = 0;
  op->reserved = nullptr;
  op++;
  op->op = GRPC_OP_RECV_INITIAL_METADATA;
  op->data.recv_initial_metadata.recv_initial_metadata = &initial_metadata_recv;
  op->flags = 0;
  op->reserved = nullptr;
  op++;
  op->op = GRPC_OP_RECV_STATUS_ON_CLIENT;
  op->data.recv_status_on_client.trailing_metadata = &trailing_metadata_recv;
  op->data.recv_status_on_client.status = &status;
  op->data.recv_status_on_client.status_details = &details;
  op->data.recv_status_on_client.error_string = nullptr;
  op->flags = 0;
  op->reserved = nullptr;
  op++;
  error = grpc_call_start_batch(c, ops, static_cast<size_t>(op - ops), tag(1),
                                nullptr);
  GPR_ASSERT(GRPC_CALL_OK == error);

  error =
      grpc_server_request_call(f.server, &s, &call_details,
                               &request_metadata_recv, f.cq, f.cq, tag(101));
  GPR_ASSERT(GRPC_CALL_OK == error);
  CQ_EXPECT_COMPLETION(cqv, tag(101), 1);
  cq_verify(cqv);

  memset(ops, 0, sizeof(ops));
  op = ops;
  op->op = GRPC_OP_SEND_INITIAL_METADATA;
  op->data.send_initial_metadata.count = 0;
  op->flags = 0;
  op->reserved = nullptr;
  op++;
  op->op = GRPC_OP_SEND_STATUS_FROM_SERVER;
  op->data.send_status_from_server.trailing_metadata_count = 0;
  op->data.send_status_from_server.status =
      request_is_success ? GRPC_STATUS_OK : GRPC_STATUS_UNIMPLEMENTED;
  grpc_slice status_details = grpc_slice_from_static_string("xyz");
  op->data.send_status_from_server.status_details = &status_details;
  op->flags = 0;
  op->reserved = nullptr;
  op++;
  op->op = GRPC_OP_RECV_CLOSE_ON_SERVER;
  op->data.recv_close_on_server.cancelled = &was_cancelled;
  op->flags = 0;
  op->reserved = nullptr;
  op++;
  error = grpc_call_start_batch(s, ops, static_cast<size_t>(op - ops), tag(102),
                                nullptr);
  GPR_ASSERT(GRPC_CALL_OK == error);

  CQ_EXPECT_COMPLETION(cqv, tag(102), 1);
  CQ_EXPECT_COMPLETION(cqv, tag(1), 1);
  cq_verify(cqv);

  GPR_ASSERT(0 == grpc_slice_str_cmp(details, "xyz"));
  GPR_ASSERT(0 == grpc_slice_str_cmp(call_details.method, "/foo"));
  GPR_ASSERT(0 == call_details.flags);

  grpc_slice_unref(details);
  grpc_metadata_array_destroy(&initial_metadata_recv);
  grpc_metadata_array_destroy(&trailing_metadata_recv);
  grpc_metadata_array_destroy(&request_metadata_recv);
  grpc_call_details_destroy(&call_details);

  grpc_call_unref(c);
  grpc_call_unref(s);

  cq_verifier_destroy(cqv);
}

static void test_channelz(grpc_end2end_test_config config) {
  grpc_end2end_test_fixture f;

  grpc_arg arg[] = {
      grpc_channel_arg_integer_create(
          const_cast<char*>(GRPC_ARG_MAX_CHANNEL_TRACE_EVENT_MEMORY_PER_NODE),
          0),
      grpc_channel_arg_integer_create(
          const_cast<char*>(GRPC_ARG_ENABLE_CHANNELZ), true)};
  grpc_channel_args args = {GPR_ARRAY_SIZE(arg), arg};

  f = begin_test(config, "test_channelz", &args, &args);
  grpc_core::channelz::ChannelNode* channelz_channel =
      grpc_channel_get_channelz_node(f.client);
  GPR_ASSERT(channelz_channel != nullptr);

  grpc_core::channelz::ServerNode* channelz_server =
      grpc_server_get_channelz_node(f.server);
  GPR_ASSERT(channelz_server != nullptr);

  std::string json = channelz_channel->RenderJsonString();
  // nothing is present yet
  GPR_ASSERT(json.find("\"callsStarted\"") == json.npos);
  GPR_ASSERT(json.find("\"callsFailed\"") == json.npos);
  GPR_ASSERT(json.find("\"callsSucceeded\"") == json.npos);

  // one successful request
  run_one_request(config, f, true);

  json = channelz_channel->RenderJsonString();
  GPR_ASSERT(json.find("\"callsStarted\":\"1\"") != json.npos);
  GPR_ASSERT(json.find("\"callsSucceeded\":\"1\"") != json.npos);

  // one failed request
  run_one_request(config, f, false);

  json = channelz_channel->RenderJsonString();
  GPR_ASSERT(json.find("\"callsStarted\":\"2\"") != json.npos);
  GPR_ASSERT(json.find("\"callsFailed\":\"1\"") != json.npos);
  GPR_ASSERT(json.find("\"callsSucceeded\":\"1\"") != json.npos);
  // channel tracing is not enabled, so these should not be preset.
  GPR_ASSERT(json.find("\"trace\"") == json.npos);
  GPR_ASSERT(json.find("\"description\":\"Channel created\"") == json.npos);
  GPR_ASSERT(json.find("\"severity\":\"CT_INFO\"") == json.npos);

  json = channelz_server->RenderJsonString();
  GPR_ASSERT(json.find("\"callsStarted\":\"2\"") != json.npos);
  GPR_ASSERT(json.find("\"callsFailed\":\"1\"") != json.npos);
  GPR_ASSERT(json.find("\"callsSucceeded\":\"1\"") != json.npos);
  // channel tracing is not enabled, so these should not be preset.
  GPR_ASSERT(json.find("\"trace\"") == json.npos);
  GPR_ASSERT(json.find("\"description\":\"Channel created\"") == json.npos);
  GPR_ASSERT(json.find("\"severity\":\"CT_INFO\"") == json.npos);

  json = channelz_server->RenderServerSockets(0, 100);
  GPR_ASSERT(json.find("\"end\":true") != json.npos);

  end_test(&f);
  config.tear_down_data(&f);
}

static void test_channelz_with_channel_trace(grpc_end2end_test_config config) {
  grpc_end2end_test_fixture f;

  grpc_arg arg[] = {
      grpc_channel_arg_integer_create(
          const_cast<char*>(GRPC_ARG_MAX_CHANNEL_TRACE_EVENT_MEMORY_PER_NODE),
          1024 * 1024),
      grpc_channel_arg_integer_create(
          const_cast<char*>(GRPC_ARG_ENABLE_CHANNELZ), true)};
  grpc_channel_args args = {GPR_ARRAY_SIZE(arg), arg};

  f = begin_test(config, "test_channelz_with_channel_trace", &args, &args);
  grpc_core::channelz::ChannelNode* channelz_channel =
      grpc_channel_get_channelz_node(f.client);
  GPR_ASSERT(channelz_channel != nullptr);

  grpc_core::channelz::ServerNode* channelz_server =
      grpc_server_get_channelz_node(f.server);
  GPR_ASSERT(channelz_server != nullptr);

  run_one_request(config, f, true);

  std::string json = channelz_channel->RenderJsonString();
  GPR_ASSERT(json.find("\"trace\"") != json.npos);
  GPR_ASSERT(json.find("\"description\":\"Channel created\"") != json.npos);
  GPR_ASSERT(json.find("\"severity\":\"CT_INFO\"") != json.npos);

  json = channelz_server->RenderJsonString();
  GPR_ASSERT(json.find("\"trace\"") != json.npos);
  GPR_ASSERT(json.find("\"description\":\"Server created\"") != json.npos);
  GPR_ASSERT(json.find("\"severity\":\"CT_INFO\"") != json.npos);

  end_test(&f);
  config.tear_down_data(&f);
}

static void test_channelz_disabled(grpc_end2end_test_config config) {
  grpc_end2end_test_fixture f;

  grpc_arg arg[] = {
      grpc_channel_arg_integer_create(
          const_cast<char*>(GRPC_ARG_MAX_CHANNEL_TRACE_EVENT_MEMORY_PER_NODE),
          0),
      grpc_channel_arg_integer_create(
          const_cast<char*>(GRPC_ARG_ENABLE_CHANNELZ), false)};
  grpc_channel_args args = {GPR_ARRAY_SIZE(arg), arg};

  f = begin_test(config, "test_channelz_disabled", &args, &args);
  grpc_core::channelz::ChannelNode* channelz_channel =
      grpc_channel_get_channelz_node(f.client);
  GPR_ASSERT(channelz_channel == nullptr);
  // one successful request
  run_one_request(config, f, true);
  GPR_ASSERT(channelz_channel == nullptr);
  end_test(&f);
  config.tear_down_data(&f);
}

void channelz(grpc_end2end_test_config config) {
  test_channelz(config);
  test_channelz_with_channel_trace(config);
  test_channelz_disabled(config);
}

void channelz_pre_init(void) {}
