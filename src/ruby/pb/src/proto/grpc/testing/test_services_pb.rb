# Generated by the protocol buffer compiler.  DO NOT EDIT!
# Source: src/proto/grpc/testing/test.proto for package 'grpc.testing'
# Original file comments:
# Copyright 2015-2016 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# An integration test service that covers all the method signature permutations
# of unary/streaming requests/responses.
#

require 'grpc'
require 'src/proto/grpc/testing/test_pb'

module Grpc
  module Testing
    module TestService
      # A simple service to test the various types of RPCs and experiment with
      # performance with various types of payload.
      class Service

        include GRPC::GenericService

        self.marshal_class_method = :encode
        self.unmarshal_class_method = :decode
        self.service_name = 'grpc.testing.TestService'

        # One empty request followed by one empty response.
        rpc :EmptyCall, ::Grpc::Testing::Empty, ::Grpc::Testing::Empty
        # One request followed by one response.
        rpc :UnaryCall, ::Grpc::Testing::SimpleRequest, ::Grpc::Testing::SimpleResponse
        # One request followed by one response. Response has cache control
        # headers set such that a caching HTTP proxy (such as GFE) can
        # satisfy subsequent requests.
        rpc :CacheableUnaryCall, ::Grpc::Testing::SimpleRequest, ::Grpc::Testing::SimpleResponse
        # One request followed by a sequence of responses (streamed download).
        # The server returns the payload with client desired type and sizes.
        rpc :StreamingOutputCall, ::Grpc::Testing::StreamingOutputCallRequest, stream(::Grpc::Testing::StreamingOutputCallResponse)
        # A sequence of requests followed by one response (streamed upload).
        # The server returns the aggregated size of client payload as the result.
        rpc :StreamingInputCall, stream(::Grpc::Testing::StreamingInputCallRequest), ::Grpc::Testing::StreamingInputCallResponse
        # A sequence of requests with each request served by the server immediately.
        # As one request could lead to multiple responses, this interface
        # demonstrates the idea of full duplexing.
        rpc :FullDuplexCall, stream(::Grpc::Testing::StreamingOutputCallRequest), stream(::Grpc::Testing::StreamingOutputCallResponse)
        # A sequence of requests followed by a sequence of responses.
        # The server buffers all the client requests and then serves them in order. A
        # stream of responses are returned to the client when the server starts with
        # first request.
        rpc :HalfDuplexCall, stream(::Grpc::Testing::StreamingOutputCallRequest), stream(::Grpc::Testing::StreamingOutputCallResponse)
        # The test server will not implement this method. It will be used
        # to test the behavior when clients call unimplemented methods.
        rpc :UnimplementedCall, ::Grpc::Testing::Empty, ::Grpc::Testing::Empty
      end

      Stub = Service.rpc_stub_class
    end
    module UnimplementedService
      # A simple service NOT implemented at servers so clients can test for
      # that case.
      class Service

        include GRPC::GenericService

        self.marshal_class_method = :encode
        self.unmarshal_class_method = :decode
        self.service_name = 'grpc.testing.UnimplementedService'

        # A call that no server should implement
        rpc :UnimplementedCall, ::Grpc::Testing::Empty, ::Grpc::Testing::Empty
      end

      Stub = Service.rpc_stub_class
    end
    module ReconnectService
      # A service used to control reconnect server.
      class Service

        include GRPC::GenericService

        self.marshal_class_method = :encode
        self.unmarshal_class_method = :decode
        self.service_name = 'grpc.testing.ReconnectService'

        rpc :Start, ::Grpc::Testing::ReconnectParams, ::Grpc::Testing::Empty
        rpc :Stop, ::Grpc::Testing::Empty, ::Grpc::Testing::ReconnectInfo
      end

      Stub = Service.rpc_stub_class
    end
    module LoadBalancerStatsService
      # A service used to obtain stats for verifying LB behavior.
      class Service

        include GRPC::GenericService

        self.marshal_class_method = :encode
        self.unmarshal_class_method = :decode
        self.service_name = 'grpc.testing.LoadBalancerStatsService'

        # Gets the backend distribution for RPCs sent by a test client.
        rpc :GetClientStats, ::Grpc::Testing::LoadBalancerStatsRequest, ::Grpc::Testing::LoadBalancerStatsResponse
        # Gets the accumulated stats for RPCs sent by a test client.
        rpc :GetClientAccumulatedStats, ::Grpc::Testing::LoadBalancerAccumulatedStatsRequest, ::Grpc::Testing::LoadBalancerAccumulatedStatsResponse
      end

      Stub = Service.rpc_stub_class
    end
    module XdsUpdateHealthService
      # A service to remotely control health status of an xDS test server.
      class Service

        include GRPC::GenericService

        self.marshal_class_method = :encode
        self.unmarshal_class_method = :decode
        self.service_name = 'grpc.testing.XdsUpdateHealthService'

        rpc :SetServing, ::Grpc::Testing::Empty, ::Grpc::Testing::Empty
        rpc :SetNotServing, ::Grpc::Testing::Empty, ::Grpc::Testing::Empty
      end

      Stub = Service.rpc_stub_class
    end
    module XdsUpdateClientConfigureService
      # A service to dynamically update the configuration of an xDS test client.
      class Service

        include GRPC::GenericService

        self.marshal_class_method = :encode
        self.unmarshal_class_method = :decode
        self.service_name = 'grpc.testing.XdsUpdateClientConfigureService'

        # Update the tes client's configuration.
        rpc :Configure, ::Grpc::Testing::ClientConfigureRequest, ::Grpc::Testing::ClientConfigureResponse
      end

      Stub = Service.rpc_stub_class
    end
  end
end
