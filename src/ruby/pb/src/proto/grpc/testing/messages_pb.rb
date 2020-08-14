# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: src/proto/grpc/testing/messages.proto

require 'google/protobuf'

Google::Protobuf::DescriptorPool.generated_pool.build do
  add_file("src/proto/grpc/testing/messages.proto", :syntax => :proto3) do
    add_message "grpc.testing.BoolValue" do
      optional :value, :bool, 1
    end
    add_message "grpc.testing.Payload" do
      optional :type, :enum, 1, "grpc.testing.PayloadType"
      optional :body, :bytes, 2
    end
    add_message "grpc.testing.EchoStatus" do
      optional :code, :int32, 1
      optional :message, :string, 2
    end
    add_message "grpc.testing.SimpleRequest" do
      optional :response_type, :enum, 1, "grpc.testing.PayloadType"
      optional :response_size, :int32, 2
      optional :payload, :message, 3, "grpc.testing.Payload"
      optional :fill_username, :bool, 4
      optional :fill_oauth_scope, :bool, 5
      optional :response_compressed, :message, 6, "grpc.testing.BoolValue"
      optional :response_status, :message, 7, "grpc.testing.EchoStatus"
      optional :expect_compressed, :message, 8, "grpc.testing.BoolValue"
      optional :fill_server_id, :bool, 9
      optional :fill_grpclb_route_type, :bool, 10
    end
    add_message "grpc.testing.SimpleResponse" do
      optional :payload, :message, 1, "grpc.testing.Payload"
      optional :username, :string, 2
      optional :oauth_scope, :string, 3
      optional :server_id, :string, 4
      optional :grpclb_route_type, :enum, 5, "grpc.testing.GrpclbRouteType"
      optional :hostname, :string, 6
    end
    add_message "grpc.testing.StreamingInputCallRequest" do
      optional :payload, :message, 1, "grpc.testing.Payload"
      optional :expect_compressed, :message, 2, "grpc.testing.BoolValue"
    end
    add_message "grpc.testing.StreamingInputCallResponse" do
      optional :aggregated_payload_size, :int32, 1
    end
    add_message "grpc.testing.ResponseParameters" do
      optional :size, :int32, 1
      optional :interval_us, :int32, 2
      optional :compressed, :message, 3, "grpc.testing.BoolValue"
    end
    add_message "grpc.testing.StreamingOutputCallRequest" do
      optional :response_type, :enum, 1, "grpc.testing.PayloadType"
      repeated :response_parameters, :message, 2, "grpc.testing.ResponseParameters"
      optional :payload, :message, 3, "grpc.testing.Payload"
      optional :response_status, :message, 7, "grpc.testing.EchoStatus"
    end
    add_message "grpc.testing.StreamingOutputCallResponse" do
      optional :payload, :message, 1, "grpc.testing.Payload"
    end
    add_message "grpc.testing.ReconnectParams" do
      optional :max_reconnect_backoff_ms, :int32, 1
    end
    add_message "grpc.testing.ReconnectInfo" do
      optional :passed, :bool, 1
      repeated :backoff_ms, :int32, 2
    end
    add_message "grpc.testing.LoadBalancerStatsRequest" do
      optional :num_rpcs, :int32, 1
      optional :timeout_sec, :int32, 2
    end
    add_message "grpc.testing.LoadBalancerStatsResponse" do
      map :rpcs_by_peer, :string, :int32, 1
      optional :num_failures, :int32, 2
      map :rpcs_by_method, :string, :message, 3, "grpc.testing.LoadBalancerStatsResponse.RpcsByPeer"
    end
    add_message "grpc.testing.LoadBalancerStatsResponse.RpcsByPeer" do
      map :rpcs_by_peer, :string, :int32, 1
    end
    add_enum "grpc.testing.PayloadType" do
      value :COMPRESSABLE, 0
    end
    add_enum "grpc.testing.GrpclbRouteType" do
      value :GRPCLB_ROUTE_TYPE_UNKNOWN, 0
      value :GRPCLB_ROUTE_TYPE_FALLBACK, 1
      value :GRPCLB_ROUTE_TYPE_BACKEND, 2
    end
  end
end

module Grpc
  module Testing
    BoolValue = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("grpc.testing.BoolValue").msgclass
    Payload = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("grpc.testing.Payload").msgclass
    EchoStatus = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("grpc.testing.EchoStatus").msgclass
    SimpleRequest = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("grpc.testing.SimpleRequest").msgclass
    SimpleResponse = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("grpc.testing.SimpleResponse").msgclass
    StreamingInputCallRequest = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("grpc.testing.StreamingInputCallRequest").msgclass
    StreamingInputCallResponse = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("grpc.testing.StreamingInputCallResponse").msgclass
    ResponseParameters = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("grpc.testing.ResponseParameters").msgclass
    StreamingOutputCallRequest = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("grpc.testing.StreamingOutputCallRequest").msgclass
    StreamingOutputCallResponse = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("grpc.testing.StreamingOutputCallResponse").msgclass
    ReconnectParams = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("grpc.testing.ReconnectParams").msgclass
    ReconnectInfo = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("grpc.testing.ReconnectInfo").msgclass
    LoadBalancerStatsRequest = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("grpc.testing.LoadBalancerStatsRequest").msgclass
    LoadBalancerStatsResponse = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("grpc.testing.LoadBalancerStatsResponse").msgclass
    LoadBalancerStatsResponse::RpcsByPeer = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("grpc.testing.LoadBalancerStatsResponse.RpcsByPeer").msgclass
    PayloadType = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("grpc.testing.PayloadType").enummodule
    GrpclbRouteType = ::Google::Protobuf::DescriptorPool.generated_pool.lookup("grpc.testing.GrpclbRouteType").enummodule
  end
end
