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
 *
 */

/* Benchmark gRPC end2end in various configurations */

#include <sstream>

#include <grpc++/channel.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
#include <grpc++/security/server_credentials.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc/support/log.h>

#include "src/proto/grpc/testing/echo.grpc.pb.h"
#include "test/core/util/port.h"
#include "third_party/google_benchmark/include/benchmark/benchmark.h"

namespace grpc {
namespace testing {

/*******************************************************************************
 * FIXTURES
 */

class TCPFixture {
 public:
  TCPFixture(EchoTestService::AsyncService* service) {
    ServerBuilder b;
    int port = grpc_pick_unused_port_or_die();
    std::stringstream addr;
    addr << "localhost:" << port;
    b.AddListeningPort(addr.str(), InsecureServerCredentials());
    cq_ = b.AddCompletionQueue(true);
    b.RegisterService(service);
    server_ = b.BuildAndStart();
    channel_ = CreateChannel(addr.str(), InsecureChannelCredentials());
  }

  ServerCompletionQueue* cq() { return cq_.get(); }

  std::shared_ptr<Channel> channel() { return channel_; }

 private:
  std::unique_ptr<Server> server_;
  std::unique_ptr<ServerCompletionQueue> cq_;
  std::shared_ptr<Channel> channel_;
};

/*******************************************************************************
 * BENCHMARKING KERNELS
 */

static void* tag(intptr_t x) { return reinterpret_cast<void*>(x); }

template <class Fixture>
static void BM_UnaryPingPong(benchmark::State& state) {
  EchoTestService::AsyncService service;
  Fixture fixture(&service);
  EchoRequest send_request;
  EchoResponse send_response;
  EchoResponse recv_response;
  Status recv_status;
  struct ServerEnv {
    ServerContext ctx;
    EchoRequest recv_request;
    grpc::ServerAsyncResponseWriter<EchoResponse> response_writer;
    ServerEnv() : response_writer(&ctx) {}
  };
  uint8_t server_env_buffer[2 * sizeof(ServerEnv)];
  ServerEnv* server_env[2] = {
      reinterpret_cast<ServerEnv*>(server_env_buffer),
      reinterpret_cast<ServerEnv*>(server_env_buffer + sizeof(ServerEnv))};
  new (server_env[0]) ServerEnv;
  new (server_env[1]) ServerEnv;
  service.RequestEcho(&server_env[0]->ctx, &server_env[0]->recv_request,
                      &server_env[0]->response_writer, fixture.cq(),
                      fixture.cq(), tag(0));
  service.RequestEcho(&server_env[1]->ctx, &server_env[1]->recv_request,
                      &server_env[1]->response_writer, fixture.cq(),
                      fixture.cq(), tag(1));
  std::unique_ptr<EchoTestService::Stub> stub(
      EchoTestService::NewStub(fixture.channel()));
  while (state.KeepRunning()) {
    ClientContext cli_ctx;
    std::unique_ptr<ClientAsyncResponseReader<EchoResponse>> response_reader(
        stub->AsyncEcho(&cli_ctx, send_request, fixture.cq()));
    void* t;
    bool ok;
    GPR_ASSERT(fixture.cq()->Next(&t, &ok));
    GPR_ASSERT(ok);
    GPR_ASSERT(t == tag(0) || t == tag(1));
    intptr_t slot = reinterpret_cast<intptr_t>(t);
    ServerEnv* senv = server_env[slot];
    senv->response_writer.Finish(send_response, Status::OK, tag(3));
    response_reader->Finish(&recv_response, &recv_status, tag(4));
    for (int i = (1 << 3) | (1 << 4); i != 0;) {
      GPR_ASSERT(fixture.cq()->Next(&t, &ok));
      GPR_ASSERT(ok);
      int tagnum = (int)reinterpret_cast<intptr_t>(t);
      GPR_ASSERT(i & (1 << tagnum));
      i -= 1 << tagnum;
    }
    GPR_ASSERT(recv_status.ok());

    senv->~ServerEnv();
    senv = new (senv) ServerEnv();
    service.RequestEcho(&senv->ctx, &senv->recv_request, &senv->response_writer,
                        fixture.cq(), fixture.cq(), tag(slot));
  }
}

/*******************************************************************************
 * CONFIGURATIONS
 */

BENCHMARK_TEMPLATE(BM_UnaryPingPong, TCPFixture);

}  // namespace testing
}  // namespace grpc

BENCHMARK_MAIN();
