/*
 *
 * Copyright 2016, Google Inc.
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
 *is % allowed in string
 */

#include <atomic>
#include <memory>
#include <string>

#include <gflags/gflags.h>
#include <grpc++/grpc++.h>
#include <grpc/support/log.h>

#include "test/cpp/util/test_config.h"

class ThreadManagerTest GRPC_FINAL : public grpc::ThreadManager {
 public:
  ThreadManagerTest()
      : ThreadManager(kMinPollers, kMaxPollers),
        num_do_work_(0),
        num_poll_for_work_(0),
        num_work_found_(0) {}

  grpc::ThreadManager::WorkStatus PollForWork(void **tag,
                                              bool *ok) GRPC_OVERRIDE;
  void DoWork(void *tag, bool ok) GRPC_OVERRIDE;
  void PerformTest();

 private:
  void SleepForMs(int sleep_time_ms);

  static const int kMinPollers = 2;
  static const int kMaxPollers = 10;

  static const int kPollingTimeoutMsec = 10;
  static const int kDoWorkDurationMsec = 1;

  // PollForWork will return SHUTDOWN after these many number of invocations
  static const int kMaxNumPollForWork = 50;

  std::atomic_int num_do_work_;        // Number of calls to DoWork
  std::atomic_int num_poll_for_work_;  // Number of calls to PollForWork
  std::atomic_int num_work_found_;  // Number of times WORK_FOUND was returned
};

void ThreadManagerTest::SleepForMs(int duration_ms) {
  gpr_timespec sleep_time =
      gpr_time_add(gpr_now(GPR_CLOCK_REALTIME),
                   gpr_time_from_millis(duration_ms, GPR_TIMESPAN));
  gpr_sleep_until(sleep_time);
}

grpc::ThreadManager::WorkStatus ThreadManagerTest::PollForWork(void **tag,
                                                               bool *ok) {
  int call_num = num_poll_for_work_.fetch_add(1);

  if (call_num >= kMaxNumPollForWork) {
    ThreadManager::Shutdown();
    return SHUTDOWN;
  }

  // Simulate "polling for work" by sleeping for sometime
  SleepForMs(kPollingTimeoutMsec);

  *tag = nullptr;
  *ok = true;

  // Return timeout roughly 1 out of every 3 calls
  if (call_num % 3 == 0) {
    return TIMEOUT;
  } else {
    num_work_found_++;
    return WORK_FOUND;
  }
}

void ThreadManagerTest::DoWork(void *tag, bool ok) {
  num_do_work_++;
  SleepForMs(kDoWorkDurationMsec); // Simulate doing work by sleeping
}

void ThreadManagerTest::PerformTest() {
  // Initialize() starts the ThreadManager
  ThreadManager::Initialize();

  // Wait for all the threads to gracefully terminate
  ThreadManager::Wait();

  // The number of times DoWork() was called is equal to the number of times
  // WORK_FOUND was returned
  gpr_log(GPR_DEBUG, "DoWork() called %d times", num_do_work_.load());
  GPR_ASSERT(num_do_work_ == num_work_found_);
}

int main(int argc, char **argv) {
  std::srand(std::time(NULL));

  grpc::testing::InitTest(&argc, &argv, true);
  ThreadManagerTest test_rpc_manager;
  test_rpc_manager.PerformTest();

  return 0;
}
