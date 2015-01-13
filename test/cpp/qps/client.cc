/*
 *
 * Copyright 2014, Google Inc.
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

#include <cassert>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <sstream>

#include <grpc/grpc.h>
#include <grpc/support/histogram.h>
#include <grpc/support/log.h>
#include <google/gflags.h>
#include <grpc++/client_context.h>
#include <grpc++/status.h>
#include "test/cpp/util/create_test_channel.h"
#include "test/cpp/qps/qpstest.pb.h"

DEFINE_bool(enable_ssl, false, "Whether to use ssl/tls.");
DEFINE_int32(server_port, 0, "Server port.");
DEFINE_string(server_host, "127.0.0.1", "Server host.");
DEFINE_int32(client_threads, 4, "Number of client threads.");

// We have a configurable number of channels for sending RPCs.
// RPCs are sent round-robin on the available channels by the
// various threads. Interesting cases are 1 global channel or
// 1 per-thread channel, but we can support any number.
// The channels are assigned round-robin on an RPC by RPC basis
// rather than just at initialization time in order to also measure the
// impact of cache thrashing caused by channel changes. This is an issue
// if you are not in one of the above "interesting cases"
DEFINE_int32(client_channels, 4, "Number of client channels.");

DEFINE_int32(num_rpcs, 1000, "Number of RPCs per thread.");
DEFINE_int32(payload_size, 1, "Payload size in bytes");

// Alternatively, specify parameters for test as a workload so that multiple
// tests are initiated back-to-back. This is convenient for keeping a borg
// allocation consistent. This is a space-separated list of
// [threads channels num_rpcs payload_size ]*
DEFINE_string(workload, "", "Workload parameters");

using grpc::ChannelInterface;
using grpc::CreateTestChannel;
using grpc::testing::ServerStats;
using grpc::testing::SimpleRequest;
using grpc::testing::SimpleResponse;
using grpc::testing::StatsRequest;
using grpc::testing::TestService;

static double now() {
  gpr_timespec tv = gpr_now();
  return 1e9 * tv.tv_sec + tv.tv_nsec;
}

void RunTest(const int client_threads, const int client_channels,
             const int num_rpcs, const int payload_size) {
  gpr_log(GPR_INFO,
          "QPS test with parameters\n"
          "enable_ssl = %d\n"
          "client_channels = %d\n"
          "client_threads = %d\n"
          "num_rpcs = %d\n"
          "payload_size = %d\n"
          "server_host:server_port = %s:%d\n\n",
          FLAGS_enable_ssl, client_channels, client_threads, num_rpcs,
          payload_size, FLAGS_server_host.c_str(), FLAGS_server_port);

  std::ostringstream oss;
  oss << FLAGS_server_host << ":" << FLAGS_server_port;

  class ClientChannelInfo {
   public:
    explicit ClientChannelInfo(const grpc::string &server)
        : channel_(CreateTestChannel(server, FLAGS_enable_ssl)),
          stub_(TestService::NewStub(channel_)) {}
    ChannelInterface *get_channel() { return channel_.get(); }
    TestService::Stub *get_stub() { return stub_.get(); }

   private:
    std::shared_ptr<ChannelInterface> channel_;
    std::unique_ptr<TestService::Stub> stub_;
  };

  std::vector<ClientChannelInfo> channels;
  for (int i = 0; i < client_channels; i++) {
    channels.push_back(ClientChannelInfo(oss.str()));
  }

  std::vector<std::thread> threads;  // Will add threads when ready to execute
  std::vector<::gpr_histogram *> thread_stats(client_threads);

  TestService::Stub *stub_stats = channels[0].get_stub();
  grpc::ClientContext context_stats_begin;
  StatsRequest stats_request;
  ServerStats server_stats_begin;
  stats_request.set_test_num(0);
  grpc::Status status_beg = stub_stats->CollectServerStats(
      &context_stats_begin, stats_request, &server_stats_begin);

  for (int i = 0; i < client_threads; i++) {
    gpr_histogram *hist = gpr_histogram_create(0.01, 60e9);
    GPR_ASSERT(hist != NULL);
    thread_stats[i] = hist;

    threads.push_back(std::thread(
        [hist, client_threads, client_channels, num_rpcs, payload_size,
         &channels](int channel_num) {
          SimpleRequest request;
          SimpleResponse response;
          request.set_response_type(grpc::testing::PayloadType::COMPRESSABLE);
          request.set_response_size(payload_size);

          for (int j = 0; j < num_rpcs; j++) {
            TestService::Stub *stub = channels[channel_num].get_stub();
            double start = now();
            grpc::ClientContext context;
            grpc::Status s = stub->UnaryCall(&context, request, &response);
            gpr_histogram_add(hist, now() - start);

            GPR_ASSERT((s.code() == grpc::StatusCode::OK) &&
                       (response.payload().type() ==
                        grpc::testing::PayloadType::COMPRESSABLE) &&
                       (response.payload().body().length() ==
                        static_cast<size_t>(payload_size)));

            // Now do runtime round-robin assignment of the next channel number
            channel_num += client_threads;
            channel_num %= client_channels;
          }
        },
        i % client_channels));
  }

  gpr_histogram *hist = gpr_histogram_create(0.01, 60e9);
  GPR_ASSERT(hist != NULL);
  for (auto &t : threads) {
    t.join();
  }
  for (int i = 0; i < client_threads; i++) {
    gpr_histogram *h = thread_stats[i];
    gpr_log(GPR_INFO, "latency at thread %d (50/90/95/99/99.9): %f/%f/%f/%f/%f",
            i, gpr_histogram_percentile(h, 50), gpr_histogram_percentile(h, 90),
            gpr_histogram_percentile(h, 95), gpr_histogram_percentile(h, 99),
            gpr_histogram_percentile(h, 99.9));
    gpr_histogram_merge(hist, h);
    gpr_histogram_destroy(h);
  }

  gpr_log(
      GPR_INFO,
      "latency across %d threads with %d channels and %d payload "
      "(50/90/95/99/99.9): %f / %f / %f / %f / %f",
      client_threads, client_channels, payload_size,
      gpr_histogram_percentile(hist, 50), gpr_histogram_percentile(hist, 90),
      gpr_histogram_percentile(hist, 95), gpr_histogram_percentile(hist, 99),
      gpr_histogram_percentile(hist, 99.9));
  gpr_histogram_destroy(hist);

  grpc::ClientContext context_stats_end;
  ServerStats server_stats_end;
  grpc::Status status_end = stub_stats->CollectServerStats(
      &context_stats_end, stats_request, &server_stats_end);

  double elapsed = server_stats_end.time_now() - server_stats_begin.time_now();
  int total_rpcs = client_threads * num_rpcs;
  double utime = server_stats_end.time_user() - server_stats_begin.time_user();
  double stime =
      server_stats_end.time_system() - server_stats_begin.time_system();
  gpr_log(GPR_INFO,
          "Elapsed time: %.3f\n"
          "RPC Count: %d\n"
          "QPS: %.3f\n"
          "System time: %.3f\n"
          "User time: %.3f\n"
          "Resource usage: %.1f%%\n",
          elapsed, total_rpcs, total_rpcs / elapsed, stime, utime,
          (stime + utime) / elapsed * 100.0);
}

int main(int argc, char **argv) {
  grpc_init();
  google::ParseCommandLineFlags(&argc, &argv, true);

  GPR_ASSERT(FLAGS_server_port);

  if (FLAGS_workload.length() == 0) {
    RunTest(FLAGS_client_threads, FLAGS_client_channels, FLAGS_num_rpcs,
            FLAGS_payload_size);
  } else {
    std::istringstream workload(FLAGS_workload);
    int client_threads, client_channels, num_rpcs, payload_size;
    workload >> client_threads;
    while (!workload.eof()) {
      workload >> client_channels >> num_rpcs >> payload_size;
      RunTest(client_threads, client_channels, num_rpcs, payload_size);
      workload >> client_threads;
    }
    gpr_log(GPR_INFO, "Done with specified workload.");
  }

  grpc_shutdown();
  return 0;
}
