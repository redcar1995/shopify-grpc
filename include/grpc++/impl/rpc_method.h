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

#ifndef __GRPCPP_IMPL_RPC_METHOD_H__
#define __GRPCPP_IMPL_RPC_METHOD_H__

namespace google {
namespace protobuf {
class Message;
}  // namespace protobuf
}  // namespace google

namespace grpc {

class RpcMethod {
 public:
  enum RpcType {
    NORMAL_RPC = 0,
    CLIENT_STREAMING,  // request streaming
    SERVER_STREAMING,  // response streaming
    BIDI_STREAMING
  };

  explicit RpcMethod(const char* name)
      : name_(name), method_type_(NORMAL_RPC) {}
  RpcMethod(const char* name, RpcType type) : name_(name), method_type_(type) {}

  const char* name() const { return name_; }

  RpcType method_type() const { return method_type_; }

 private:
  const char* name_;
  const RpcType method_type_;
};

}  // namespace grpc

#endif  // __GRPCPP_IMPL_RPC_METHOD_H__
