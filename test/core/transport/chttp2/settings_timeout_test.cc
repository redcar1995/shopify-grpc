/*
 *
 * Copyright 2017 gRPC authors.
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

#include <grpc/grpc.h>
#include <grpc/support/alloc.h>
#include <grpc/support/log.h>
#include <grpc/support/string_util.h>

#include <memory>
#include <thread>

#include <gtest/gtest.h>

extern "C" {
#include "src/core/lib/iomgr/endpoint.h"
#include "src/core/lib/iomgr/error.h"
#include "src/core/lib/iomgr/pollset.h"
#include "src/core/lib/iomgr/pollset_set.h"
#include "src/core/lib/iomgr/resolve_address.h"
#include "src/core/lib/iomgr/tcp_client.h"
#include "src/core/lib/slice/slice_internal.h"
}

#include "test/core/util/port.h"
#include "test/core/util/test_config.h"

namespace grpc_core {
namespace test {
namespace {

// A gRPC server, running in its own thread.
class ServerThread {
 public:
  explicit ServerThread(const char* address) : address_(address) {}

  void Start() {
    // Start server with 1-second handshake timeout.
    grpc_arg arg;
    arg.type = GRPC_ARG_INTEGER;
    arg.key = const_cast<char*>(GRPC_ARG_SERVER_HANDSHAKE_TIMEOUT_MS);
    arg.value.integer = 1000;
    grpc_channel_args args = {1, &arg};
    server_ = grpc_server_create(&args, nullptr);
    ASSERT_TRUE(grpc_server_add_insecure_http2_port(server_, address_));
    cq_ = grpc_completion_queue_create_for_next(nullptr);
    grpc_server_register_completion_queue(server_, cq_, nullptr);
    grpc_server_start(server_);
    thread_.reset(new std::thread(std::bind(&ServerThread::Serve, this)));
  }

  void Shutdown() {
    grpc_completion_queue* shutdown_cq =
        grpc_completion_queue_create_for_pluck(nullptr);
    grpc_server_shutdown_and_notify(server_, shutdown_cq, nullptr);
    GPR_ASSERT(grpc_completion_queue_pluck(shutdown_cq, nullptr,
                                           grpc_timeout_seconds_to_deadline(1),
                                           nullptr)
                   .type == GRPC_OP_COMPLETE);
    grpc_completion_queue_destroy(shutdown_cq);
    grpc_server_destroy(server_);
    grpc_completion_queue_destroy(cq_);
    thread_->join();
  }

 private:
  void Serve() {
    // The completion queue should not return anything other than shutdown.
    grpc_event ev = grpc_completion_queue_next(
        cq_, gpr_inf_future(GPR_CLOCK_MONOTONIC), nullptr);
    ASSERT_EQ(GRPC_QUEUE_SHUTDOWN, ev.type);
  }

  const char* address_;  // Do not own.
  grpc_server* server_ = nullptr;
  grpc_completion_queue* cq_ = nullptr;
  std::unique_ptr<std::thread> thread_;
};

// A TCP client that connects to the server, reads data until the server
// closes, and then terminates.
class Client {
 public:
  explicit Client(const char* server_address)
      : server_address_(server_address) {}

  void Connect() {
    grpc_exec_ctx exec_ctx = GRPC_EXEC_CTX_INIT;
    grpc_resolved_addresses* server_addresses = nullptr;
    grpc_error* error =
        grpc_blocking_resolve_address(server_address_, "80", &server_addresses);
    ASSERT_EQ(GRPC_ERROR_NONE, error) << grpc_error_string(error);
    ASSERT_GE(server_addresses->naddrs, 1UL);
    pollset_ = (grpc_pollset*)gpr_zalloc(grpc_pollset_size());
    grpc_pollset_init(pollset_, &mu_);
    grpc_pollset_set* pollset_set = grpc_pollset_set_create();
    grpc_pollset_set_add_pollset(&exec_ctx, pollset_set, pollset_);
    EventState state;
    grpc_tcp_client_connect(
        &exec_ctx, state.closure(), &endpoint_, pollset_set,
        nullptr /* channel_args */, server_addresses->addrs,
        gpr_time_add(gpr_now(GPR_CLOCK_MONOTONIC),
                     gpr_time_from_seconds(1, GPR_TIMESPAN)));
    ASSERT_TRUE(
        PollUntilDone(&exec_ctx, &state, gpr_inf_future(GPR_CLOCK_MONOTONIC)));
    ASSERT_EQ(GRPC_ERROR_NONE, state.error());
    grpc_pollset_set_destroy(&exec_ctx, pollset_set);
    grpc_endpoint_add_to_pollset(&exec_ctx, endpoint_, pollset_);
    grpc_resolved_addresses_destroy(server_addresses);
    grpc_exec_ctx_finish(&exec_ctx);
  }

  // Reads until an error is returned.
  // Returns true if an error was encountered before the deadline.
  bool ReadUntilError() {
    grpc_exec_ctx exec_ctx = GRPC_EXEC_CTX_INIT;
    grpc_slice_buffer read_buffer;
    grpc_slice_buffer_init(&read_buffer);
    bool retval = true;
    // Use a deadline of 3 seconds, which is a lot more than we should
    // need for a 1-second timeout, but this helps avoid flakes.
    gpr_timespec deadline = gpr_time_add(
        gpr_now(GPR_CLOCK_MONOTONIC), gpr_time_from_seconds(3, GPR_TIMESPAN));
    while (true) {
      EventState state;
      grpc_endpoint_read(&exec_ctx, endpoint_, &read_buffer, state.closure());
      if (!PollUntilDone(&exec_ctx, &state, deadline)) {
        retval = false;
        break;
      }
      if (state.error() != GRPC_ERROR_NONE) break;
      gpr_log(GPR_INFO, "client read %" PRIuPTR " bytes", read_buffer.length);
      grpc_slice_buffer_reset_and_unref_internal(&exec_ctx, &read_buffer);
    }
    grpc_endpoint_shutdown(&exec_ctx, endpoint_,
                           GRPC_ERROR_CREATE_FROM_STATIC_STRING("shutdown"));
    grpc_slice_buffer_destroy_internal(&exec_ctx, &read_buffer);
    grpc_exec_ctx_finish(&exec_ctx);
    return retval;
  }

  void Shutdown() {
    grpc_exec_ctx exec_ctx = GRPC_EXEC_CTX_INIT;
    grpc_endpoint_destroy(&exec_ctx, endpoint_);
    grpc_pollset_shutdown(&exec_ctx, pollset_,
                          GRPC_CLOSURE_CREATE(&Client::PollsetDestroy, pollset_,
                                              grpc_schedule_on_exec_ctx));
    grpc_exec_ctx_finish(&exec_ctx);
  }

 private:
  // State used to wait for an I/O event.
  class EventState {
   public:
    EventState() {
      GRPC_CLOSURE_INIT(&closure_, &EventState::OnEventDone, this,
                        grpc_schedule_on_exec_ctx);
    }

    ~EventState() { GRPC_ERROR_UNREF(error_); }

    grpc_closure* closure() { return &closure_; }

    bool done() const { return done_; }

    // Caller does NOT take ownership of the error.
    grpc_error* error() const { return error_; }

   private:
    static void OnEventDone(grpc_exec_ctx* exec_ctx, void* arg,
                            grpc_error* error) {
      gpr_log(GPR_INFO, "OnEventDone(): %s", grpc_error_string(error));
      EventState* state = (EventState*)arg;
      state->error_ = GRPC_ERROR_REF(error);
      state->done_ = true;
    }

    grpc_closure closure_;
    bool done_ = false;
    grpc_error* error_ = GRPC_ERROR_NONE;
  };

  // Returns true if done, or false if deadline exceeded.
  bool PollUntilDone(grpc_exec_ctx* exec_ctx, EventState* state,
                     gpr_timespec deadline) {
    while (true) {
      grpc_pollset_worker* worker = nullptr;
      gpr_mu_lock(mu_);
      gpr_timespec now = gpr_now(GPR_CLOCK_MONOTONIC);
      GRPC_LOG_IF_ERROR(
          "grpc_pollset_work",
          grpc_pollset_work(
              exec_ctx, pollset_, &worker, now,
              gpr_time_add(now, gpr_time_from_seconds(1, GPR_TIMESPAN))));
      gpr_mu_unlock(mu_);
      if (state != nullptr && state->done()) return true;
      if (gpr_time_cmp(gpr_now(GPR_CLOCK_MONOTONIC), deadline) > 0) {
        return false;
      }
    }
  }

  static void PollsetDestroy(grpc_exec_ctx* exec_ctx, void* arg,
                             grpc_error* error) {
    grpc_pollset* pollset = (grpc_pollset*)arg;
    grpc_pollset_destroy(exec_ctx, pollset);
    gpr_free(pollset);
  }

  const char* server_address_;  // Do not own.
  grpc_endpoint* endpoint_;
  gpr_mu* mu_;
  grpc_pollset* pollset_;
};

TEST(SettingsTimeout, Basic) {
  // Construct server address string.
  const int server_port = grpc_pick_unused_port_or_die();
  char* server_address_string;
  gpr_asprintf(&server_address_string, "localhost:%d", server_port);
  // Start server.
  gpr_log(GPR_INFO, "starting server on %s", server_address_string);
  ServerThread server_thread(server_address_string);
  server_thread.Start();
  // Create client and connect to server.
  gpr_log(GPR_INFO, "starting client connect");
  Client client(server_address_string);
  client.Connect();
  // Client read.  Should fail due to server dropping connection.
  gpr_log(GPR_INFO, "starting client read");
  EXPECT_TRUE(client.ReadUntilError());
  // Shut down client.
  gpr_log(GPR_INFO, "shutting down client");
  client.Shutdown();
  // Shut down server.
  gpr_log(GPR_INFO, "shutting down server");
  server_thread.Shutdown();
  // Clean up.
  gpr_free(server_address_string);
}

}  // namespace
}  // namespace test
}  // namespace grpc_core

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  grpc_test_init(argc, argv);
  grpc_init();
  int result = RUN_ALL_TESTS();
  grpc_shutdown();
  return result;
}
