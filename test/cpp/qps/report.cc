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

#include "test/cpp/qps/report.h"

#include <grpc/support/log.h>
#include "test/cpp/qps/stats.h"
#include "user_data_client.h"

namespace grpc {
namespace testing {

void CompositeReporter::add(std::unique_ptr<Reporter> reporter) {
  reporters_.emplace_back(std::move(reporter));
}

void CompositeReporter::ReportQPS(const ScenarioResult& result) const {
  for (size_t i = 0; i < reporters_.size(); ++i) {
    reporters_[i]->ReportQPS(result);
  }
}

void CompositeReporter::ReportQPSPerCore(const ScenarioResult& result) const {
  for (size_t i = 0; i < reporters_.size(); ++i) {
    reporters_[i]->ReportQPSPerCore(result);
  }
}

void CompositeReporter::ReportLatency(const ScenarioResult& result) const {
  for (size_t i = 0; i < reporters_.size(); ++i) {
    reporters_[i]->ReportLatency(result);
  }
}

void CompositeReporter::ReportTimes(const ScenarioResult& result) const {
  for (size_t i = 0; i < reporters_.size(); ++i) {
    reporters_[i]->ReportTimes(result);
  }
}


void GprLogReporter::ReportQPS(const ScenarioResult& result) const {
  gpr_log(GPR_INFO, "QPS: %.1f",
          result.latencies.Count() /
              average(result.client_resources,
                      [](ResourceUsage u) { return u.wall_time; }));
}

void GprLogReporter::ReportQPSPerCore(const ScenarioResult& result)  const {
  auto qps =
      result.latencies.Count() /
      average(result.client_resources,
          [](ResourceUsage u) { return u.wall_time; });

  gpr_log(GPR_INFO, "QPS: %.1f (%.1f/server core)", qps,
          qps / result.server_config.threads());
}

void GprLogReporter::ReportLatency(const ScenarioResult& result) const {
  gpr_log(GPR_INFO,
          "Latencies (50/90/95/99/99.9%%-ile): %.1f/%.1f/%.1f/%.1f/%.1f us",
          result.latencies.Percentile(50) / 1000,
          result.latencies.Percentile(90) / 1000,
          result.latencies.Percentile(95) / 1000,
          result.latencies.Percentile(99) / 1000,
          result.latencies.Percentile(99.9) / 1000);
}

void GprLogReporter::ReportTimes(const ScenarioResult& result) const {
  gpr_log(GPR_INFO, "Server system time: %.2f%%",
          100.0 * sum(result.server_resources,
                      [](ResourceUsage u) { return u.system_time; }) /
              sum(result.server_resources,
                  [](ResourceUsage u) { return u.wall_time; }));
  gpr_log(GPR_INFO, "Server user time:   %.2f%%",
          100.0 * sum(result.server_resources,
                      [](ResourceUsage u) { return u.user_time; }) /
              sum(result.server_resources,
                  [](ResourceUsage u) { return u.wall_time; }));
  gpr_log(GPR_INFO, "Client system time: %.2f%%",
          100.0 * sum(result.client_resources,
                      [](ResourceUsage u) { return u.system_time; }) /
              sum(result.client_resources,
                  [](ResourceUsage u) { return u.wall_time; }));
  gpr_log(GPR_INFO, "Client user time:   %.2f%%",
          100.0 * sum(result.client_resources,
                      [](ResourceUsage u) { return u.user_time; }) /
              sum(result.client_resources,
                  [](ResourceUsage u) { return u.wall_time; }));
}

UserDataClient userDataClient(grpc::CreateChannel("localhost:50052", grpc::InsecureCredentials(),
                          ChannelArguments()));

//Leaderboard Reported implementation.
void UserDatabaseReporter::ReportQPS(const ScenarioResult& result) const {
  auto qps = result.latencies.Count() /
              average(result.client_resources,
                      [](ResourceUsage u) { return u.wall_time; });

  userDataClient.setQPS(qps);
  userDataClient.setConfigs(result.client_config, result.server_config);
}

void UserDatabaseReporter::ReportQPSPerCore(const ScenarioResult& result) const {
  auto qps = result.latencies.Count() /
            average(result.client_resources,
                    [](ResourceUsage u) { return u.wall_time; });

  auto qpsPerCore = qps / result.server_config.threads();

  userDataClient.setQPSPerCore(qpsPerCore);
  userDataClient.setConfigs(result.client_config, result.server_config);
}

void UserDatabaseReporter::ReportLatency(const ScenarioResult& result) const {
  userDataClient.setLatencies(result.latencies.Percentile(50) / 1000,
                              result.latencies.Percentile(90) / 1000,
                              result.latencies.Percentile(95) / 1000,
                              result.latencies.Percentile(99) / 1000,
                              result.latencies.Percentile(99.9) / 1000);
  userDataClient.setConfigs(result.client_config, result.server_config);
}

void UserDatabaseReporter::ReportTimes(const ScenarioResult& result) const {
  double serverSystemTime = 100.0 * sum(result.server_resources,
                  [](ResourceUsage u) { return u.system_time; }) /
                    sum(result.server_resources,
                  [](ResourceUsage u) { return u.wall_time; });
  double serverUserTime = 100.0 * sum(result.server_resources,
                  [](ResourceUsage u) { return u.user_time; }) /
                    sum(result.server_resources,
                  [](ResourceUsage u) { return u.wall_time; });
  double clientSystemTime = 100.0 * sum(result.client_resources,
                  [](ResourceUsage u) { return u.system_time; }) /
                    sum(result.client_resources,
                  [](ResourceUsage u) { return u.wall_time; });
  double clientUserTime = 100.0 * sum(result.client_resources,
                  [](ResourceUsage u) { return u.user_time; }) /
                    sum(result.client_resources,
                  [](ResourceUsage u) { return u.wall_time; });

  userDataClient.setTimes(serverSystemTime, serverUserTime, 
    clientSystemTime, clientUserTime);
  userDataClient.setConfigs(result.client_config, result.server_config);
}

void UserDatabaseReporter::SendData() const {
  //send data to performance database
  int userDataState = userDataClient.sendData(access_token_, test_name_, sys_info_);

  //check state of data sending
  switch(userDataState) {
    case 1:
      gpr_log(GPR_INFO, "Data sent to user database successfully");
      break;
    case -1:
      gpr_log(GPR_INFO, "Data could not be sent to user database");
      break;
  }
}

}  // namespace testing
}  // namespace grpc
