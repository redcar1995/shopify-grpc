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

#include <grpc/support/log.h>

#include "test/cpp/qps/driver.h"
#include "test/cpp/qps/stats.h"

namespace grpc {
namespace testing {

static void RunSynchronousUnaryPingPong() {
  gpr_log(GPR_INFO, "Running Synchronous Unary Ping Pong");

  ClientConfig client_config;
  client_config.set_client_type(SYNCHRONOUS_CLIENT);
  client_config.set_enable_ssl(false);
  client_config.set_outstanding_rpcs_per_channel(1);
  client_config.set_client_channels(1);
  client_config.set_payload_size(1);
  client_config.set_rpc_type(UNARY);

  ServerConfig server_config;
  server_config.set_server_type(SYNCHRONOUS_SERVER);
  server_config.set_enable_ssl(false);
  server_config.set_threads(1);

  auto result = RunScenario(client_config, 1, server_config, 1, 5, 15);

  gpr_log(GPR_INFO, "QPS: %.1f",
          result.latencies.Count() /
              average(result.client_resources,
                      [](ResourceUsage u) { return u.wall_time; }));
  gpr_log(GPR_INFO, "Latencies (50/95/99/99.9%%-ile): %.1f/%.1f/%.1f/%.1f us",
          result.latencies.Percentile(50) / 1000,
          result.latencies.Percentile(95) / 1000,
          result.latencies.Percentile(99) / 1000,
          result.latencies.Percentile(99.9) / 1000);
}

static void RunSynchronousStreamingPingPong() {
  gpr_log(GPR_INFO, "Running Synchronous Streaming Ping Pong");

  ClientConfig client_config;
  client_config.set_client_type(SYNCHRONOUS_CLIENT);
  client_config.set_enable_ssl(false);
  client_config.set_outstanding_rpcs_per_channel(1);
  client_config.set_client_channels(1);
  client_config.set_payload_size(1);
  client_config.set_rpc_type(STREAMING);

  ServerConfig server_config;
  server_config.set_server_type(SYNCHRONOUS_SERVER);
  server_config.set_enable_ssl(false);
  server_config.set_threads(1);

  auto result = RunScenario(client_config, 1, server_config, 1, 5, 15);

  gpr_log(GPR_INFO, "QPS: %.1f",
          result.latencies.Count() /
              average(result.client_resources,
                      [](ResourceUsage u) { return u.wall_time; }));
  gpr_log(GPR_INFO, "Latencies (50/95/99/99.9%%-ile): %.1f/%.1f/%.1f/%.1f us",
          result.latencies.Percentile(50) / 1000,
          result.latencies.Percentile(95) / 1000,
          result.latencies.Percentile(99) / 1000,
          result.latencies.Percentile(99.9) / 1000);
}

static void RunAsyncUnaryPingPong() {
  gpr_log(GPR_INFO, "Running Async Unary Ping Pong");

  ClientConfig client_config;
  client_config.set_client_type(ASYNC_CLIENT);
  client_config.set_enable_ssl(false);
  client_config.set_outstanding_rpcs_per_channel(1);
  client_config.set_client_channels(1);
  client_config.set_payload_size(1);
  client_config.set_async_client_threads(1);
  client_config.set_rpc_type(UNARY);

  ServerConfig server_config;
  server_config.set_server_type(ASYNC_SERVER);
  server_config.set_enable_ssl(false);
  server_config.set_threads(1);

  auto result = RunScenario(client_config, 1, server_config, 1, 5, 15);

  gpr_log(GPR_INFO, "QPS: %.1f",
          result.latencies.Count() /
              average(result.client_resources,
                      [](ResourceUsage u) { return u.wall_time; }));
  gpr_log(GPR_INFO, "Latencies (50/95/99/99.9%%-ile): %.1f/%.1f/%.1f/%.1f us",
          result.latencies.Percentile(50) / 1000,
          result.latencies.Percentile(95) / 1000,
          result.latencies.Percentile(99) / 1000,
          result.latencies.Percentile(99.9) / 1000);
}

static void RunQPS() {
  gpr_log(GPR_INFO, "Running QPS test");

  ClientConfig client_config;
  client_config.set_client_type(ASYNC_CLIENT);
  client_config.set_enable_ssl(false);
  client_config.set_outstanding_rpcs_per_channel(1000);
  client_config.set_client_channels(8);
  client_config.set_payload_size(1);
  client_config.set_async_client_threads(8);
  client_config.set_rpc_type(UNARY);

  ServerConfig server_config;
  server_config.set_server_type(ASYNC_SERVER);
  server_config.set_enable_ssl(false);
  server_config.set_threads(4);

  auto result = RunScenario(client_config, 1, server_config, 1, 10, 30);

  auto qps = 
      result.latencies.Count() /
      average(result.client_resources,
          [](ResourceUsage u) { return u.wall_time; });

  gpr_log(GPR_INFO, "QPS: %.1f (%.1f/core)", qps, qps/client_config.client_channels());
  gpr_log(GPR_INFO, "Latencies (50/95/99/99.9%%-ile): %.1f/%.1f/%.1f/%.1f us",
          result.latencies.Percentile(50) / 1000,
          result.latencies.Percentile(95) / 1000,
          result.latencies.Percentile(99) / 1000,
          result.latencies.Percentile(99.9) / 1000);
}

}  // namespace testing
}  // namespace grpc

int main(int argc, char** argv) {
  grpc_init();

  using namespace grpc::testing;
  RunSynchronousStreamingPingPong();
  RunSynchronousUnaryPingPong();
  RunAsyncUnaryPingPong();
  RunQPS();

  grpc_shutdown();
  return 0;
}
