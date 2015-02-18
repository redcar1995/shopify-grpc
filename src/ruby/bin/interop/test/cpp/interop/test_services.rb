# Copyright 2015, Google Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Generated by the protocol buffer compiler.  DO NOT EDIT!
# Source: test/cpp/interop/test.proto for package 'grpc.testing'

require 'grpc'
require 'test/cpp/interop/test'

module Grpc
  module Testing
    module TestService

      # TODO: add proto service documentation here
      class Service

        include GRPC::GenericService

        self.marshal_class_method = :encode
        self.unmarshal_class_method = :decode
        self.service_name = 'grpc.testing.TestService'

        rpc :EmptyCall, Empty, Empty
        rpc :UnaryCall, SimpleRequest, SimpleResponse
        rpc :StreamingOutputCall, StreamingOutputCallRequest, stream(StreamingOutputCallResponse)
        rpc :StreamingInputCall, stream(StreamingInputCallRequest), StreamingInputCallResponse
        rpc :FullDuplexCall, stream(StreamingOutputCallRequest), stream(StreamingOutputCallResponse)
        rpc :HalfDuplexCall, stream(StreamingOutputCallRequest), stream(StreamingOutputCallResponse)
      end

      Stub = Service.rpc_stub_class
    end
  end
end
