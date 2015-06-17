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

#include "test/cpp/util/cli_call.h"

#include <iostream>

#include <grpc++/byte_buffer.h>
#include <grpc++/channel_interface.h>
#include <grpc++/client_context.h>
#include <grpc++/generic_stub.h>
#include <grpc++/status.h>
#include <grpc++/stream.h>

#include <grpc/grpc.h>
#include <grpc/support/log.h>
#include <grpc/support/slice.h>

namespace grpc {
namespace testing {
namespace {
void* tag(int i) { return (void*)(gpr_intptr) i; }
}  // namespace

void CliCall::Call(std::shared_ptr<grpc::ChannelInterface> channel,
                   const grpc::string& method, const grpc::string& request,
                   grpc::string* response) {
  std::unique_ptr<grpc::GenericStub> stub(new grpc::GenericStub(channel));
  grpc::ClientContext ctx;
  grpc::CompletionQueue cq;
  std::unique_ptr<grpc::GenericClientAsyncReaderWriter> call(
      stub->Call(&ctx, method, &cq, tag(1)));
  void* got_tag;
  bool ok;
  cq.Next(&got_tag, &ok);
  GPR_ASSERT(ok);

  gpr_slice s = gpr_slice_from_copied_string(request.c_str());
  grpc::Slice req_slice(s, grpc::Slice::STEAL_REF);
  grpc::ByteBuffer send_buffer(&req_slice, 1);
  call->Write(send_buffer, tag(2));
  cq.Next(&got_tag, &ok);
  GPR_ASSERT(ok);
  call->WritesDone(tag(3));
  cq.Next(&got_tag, &ok);
  GPR_ASSERT(ok);
  grpc::ByteBuffer recv_buffer;
  call->Read(&recv_buffer, tag(4));
  cq.Next(&got_tag, &ok);
  if (!ok) {
    std::cout << "Failed to read response." << std::endl;
    return;
  }
  grpc::Status status;
  call->Finish(&status, tag(5));
  cq.Next(&got_tag, &ok);
  GPR_ASSERT(ok);

  if (status.ok()) {
    std::cout << "RPC finished with OK status." << std::endl;
    std::vector<grpc::Slice> slices;
    recv_buffer.Dump(&slices);

    response->clear();
    for (size_t i = 0; i < slices.size(); i++) {
      response->append(reinterpret_cast<const char*>(slices[i].begin()),
                       slices[i].size());
    }
  } else {
    std::cout << "RPC finished with status code " << status.error_code()
              << " details: " << status.error_message() << std::endl;
  }
}

}  // namespace testing
}  // namespace grpc
