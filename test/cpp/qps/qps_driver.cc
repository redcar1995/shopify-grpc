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

#include <memory>
#include <set>

#include <gflags/gflags.h>
#include <grpc/support/log.h>

#include "test/cpp/qps/driver.h"
#include "test/cpp/qps/report.h"
#include "test/cpp/util/benchmark_config.h"

DEFINE_int32(num_clients, 1, "Number of client binaries");
DEFINE_int32(num_servers, 1, "Number of server binaries");

DEFINE_int32(warmup_seconds, 5, "Warmup time (in seconds)");
DEFINE_int32(benchmark_seconds, 30, "Benchmark time (in seconds)");
DEFINE_int32(local_workers, 0, "Number of local workers to start");

// Server config
DEFINE_int32(async_server_threads, 1, "Number of threads for async servers");
DEFINE_string(server_type, "SYNC_SERVER", "Server type");
DEFINE_int32(server_core_limit, -1, "Limit on server cores to use");

// Client config
DEFINE_string(rpc_type, "UNARY", "Type of RPC: UNARY or STREAMING");
DEFINE_int32(outstanding_rpcs_per_channel, 1,
             "Number of outstanding rpcs per channel");
DEFINE_int32(client_channels, 1, "Number of client channels");

DEFINE_int32(simple_req_size, -1, "Simple proto request payload size");
DEFINE_int32(simple_resp_size, -1, "Simple proto response payload size");
DEFINE_int32(bbuf_req_size, -1, "Byte-buffer request payload size");
DEFINE_int32(bbuf_resp_size, -1, "Byte-buffer response payload size");

DEFINE_string(client_type, "SYNC_CLIENT", "Client type");
DEFINE_int32(async_client_threads, 1, "Async client threads");

DEFINE_double(poisson_load, -1.0, "Poisson offered load (qps)");

DEFINE_int32(client_core_limit, -1, "Limit on client cores to use");

DEFINE_bool(secure_test, false, "Run a secure test");

DEFINE_bool(quit, false, "Quit the workers");

using grpc::testing::ClientConfig;
using grpc::testing::ServerConfig;
using grpc::testing::ClientType;
using grpc::testing::ServerType;
using grpc::testing::RpcType;
using grpc::testing::SecurityParams;

namespace grpc {
namespace testing {

static void QpsDriver() {
  if (FLAGS_quit) {
    RunQuit();
    return;
  }

  RpcType rpc_type;
  GPR_ASSERT(RpcType_Parse(FLAGS_rpc_type, &rpc_type));

  ClientType client_type;
  ServerType server_type;
  GPR_ASSERT(ClientType_Parse(FLAGS_client_type, &client_type));
  GPR_ASSERT(ServerType_Parse(FLAGS_server_type, &server_type));

  ClientConfig client_config;
  client_config.set_client_type(client_type);
  client_config.set_outstanding_rpcs_per_channel(
      FLAGS_outstanding_rpcs_per_channel);
  client_config.set_client_channels(FLAGS_client_channels);

  // Decide which type to use based on the response type
  if (FLAGS_simple_resp_size >= 0) {
    auto params =
        client_config.mutable_payload_config()->mutable_simple_params();
    params->set_resp_size(FLAGS_simple_resp_size);
    if (FLAGS_simple_req_size >= 0) {
      params->set_req_size(FLAGS_simple_req_size);
    }
  } else if (FLAGS_bbuf_resp_size >= 0) {
    auto params =
        client_config.mutable_payload_config()->mutable_bytebuf_params();
    params->set_resp_size(FLAGS_bbuf_resp_size);
    if (FLAGS_bbuf_req_size >= 0) {
      params->set_req_size(FLAGS_bbuf_req_size);
    }
  } else {
    // set a reasonable default: proto but no payload
    client_config.mutable_payload_config()->mutable_simple_params();
  }

  client_config.set_async_client_threads(FLAGS_async_client_threads);
  client_config.set_rpc_type(rpc_type);

  // set up the load parameters
  if (FLAGS_poisson_load > 0.0) {
    auto poisson = client_config.mutable_load_params()->mutable_poisson();
    poisson->set_offered_load(FLAGS_poisson_load);
  } else {
    client_config.mutable_load_params()->mutable_closed_loop();
    // No further load parameters to set up for closed loop
  }

  client_config.mutable_histogram_params()->set_resolution(
      Histogram::default_resolution());
  client_config.mutable_histogram_params()->set_max_possible(
      Histogram::default_max_possible());

  if (FLAGS_client_core_limit > 0) {
    client_config.set_core_limit(FLAGS_client_core_limit);
  }

  ServerConfig server_config;
  server_config.set_server_type(server_type);
  server_config.set_async_server_threads(FLAGS_async_server_threads);

  if (FLAGS_server_core_limit > 0) {
    server_config.set_core_limit(FLAGS_server_core_limit);
  }

  if (FLAGS_bbuf_resp_size >= 0) {
    *server_config.mutable_payload_config() = client_config.payload_config();
  }

  if (FLAGS_secure_test) {
    // Set up security params
    SecurityParams security;
    security.set_use_test_ca(true);
    security.set_server_host_override("foo.test.google.fr");
    client_config.mutable_security_params()->CopyFrom(security);
    server_config.mutable_security_params()->CopyFrom(security);
  }

  // Make sure that if we are performing a generic (bytebuf) test
  // that we are also using async streaming
  GPR_ASSERT(!client_config.payload_config().has_bytebuf_params() ||
             (client_config.client_type() == ASYNC_CLIENT &&
              client_config.rpc_type() == STREAMING &&
              server_config.server_type() == ASYNC_GENERIC_SERVER));

  const auto result = RunScenario(
      client_config, FLAGS_num_clients, server_config, FLAGS_num_servers,
      FLAGS_warmup_seconds, FLAGS_benchmark_seconds, FLAGS_local_workers);

  GetReporter()->ReportQPS(*result);
  GetReporter()->ReportQPSPerCore(*result);
  GetReporter()->ReportLatency(*result);
  GetReporter()->ReportTimes(*result);
}

}  // namespace testing
}  // namespace grpc

int main(int argc, char** argv) {
  grpc::testing::InitBenchmark(&argc, &argv, true);

  grpc::testing::QpsDriver();

  return 0;
}
