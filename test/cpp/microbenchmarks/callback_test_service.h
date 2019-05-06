/*
 *
 * Copyright 2019 gRPC authors.
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

#ifndef TEST_CPP_MICROBENCHMARKS_CALLBACK_TEST_SERVICE_H
#define TEST_CPP_MICROBENCHMARKS_CALLBACK_TEST_SERVICE_H

#include <benchmark/benchmark.h>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <sstream>
#include "src/proto/grpc/testing/echo.grpc.pb.h"
#include "test/cpp/util/string_ref_helper.h"

namespace grpc {
namespace testing {

const char* const kServerFinishAfterNReads = "server_finish_after_n_reads";
const char* const kServerMessageSize = "server_message_size";

class CallbackStreamingTestService
    : public EchoTestService::ExperimentalCallbackService {
 public:
  CallbackStreamingTestService() {}
  void Echo(ServerContext* context, const EchoRequest* request,
            EchoResponse* response,
            experimental::ServerCallbackRpcController* controller) override;

  experimental::ServerBidiReactor<EchoRequest, EchoResponse>* BidiStream()
      override;
};

class BidiClient
    : public grpc::experimental::ClientBidiReactor<EchoRequest, EchoResponse> {
 public:
  BidiClient(benchmark::State* state, EchoTestService::Stub* stub,
             EchoRequest* request, EchoResponse* response, std::mutex* mu,
             std::condition_variable* cv, bool* done)
      : state_{state},
        stub_{stub},
        request_{request},
        response_{response},
        mu_{mu},
        cv_{cv},
        done_(done) {
    msgs_size_ = state->range(0);
    msgs_to_send_ = state->range(1);
  }

  void OnReadDone(bool ok) override {
    if (!ok) {
      return;
    }
    if (writes_complete_ < msgs_to_send_) {
      MaybeWrite();
    }
  }

  void OnWriteDone(bool ok) override {
    if (!ok) {
      return;
    }
    writes_complete_++;
    StartRead(response_);
  }

  void OnDone(const Status& s) override {
    GPR_ASSERT(s.ok());
    if (state_->KeepRunning()) {
      BidiClient* test =
          new BidiClient(state_, stub_, request_, response_, mu_, cv_, done_);
      test->StartNewRpc();
    } else {
      std::unique_lock<std::mutex> l(*mu_);
      *done_ = true;
      cv_->notify_one();
    }
    delete cli_ctx_;
  }

  void StartNewRpc() {
    cli_ctx_ = new ClientContext();
    cli_ctx_->AddMetadata(kServerFinishAfterNReads,
                          grpc::to_string(msgs_to_send_));
    cli_ctx_->AddMetadata(kServerMessageSize, grpc::to_string(msgs_size_));
    stub_->experimental_async()->BidiStream(cli_ctx_, this);
    MaybeWrite();
    StartCall();
  }

 private:
  void MaybeWrite() {
    if (writes_complete_ < msgs_to_send_) {
      StartWrite(request_);
    } else {
      StartWritesDone();
    }
  }

  ClientContext* cli_ctx_;
  benchmark::State* state_;
  EchoTestService::Stub* stub_;
  EchoRequest* request_;
  EchoResponse* response_;
  int writes_complete_{0};
  int msgs_to_send_;
  int msgs_size_;
  std::mutex* mu_;
  std::condition_variable* cv_;
  bool* done_;
};

}  // namespace testing
}  // namespace grpc
#endif  // TEST_CPP_MICROBENCHMARKS_CALLBACK_TEST_SERVICE_H
