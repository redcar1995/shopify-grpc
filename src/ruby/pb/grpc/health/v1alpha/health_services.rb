# Generated by the protocol buffer compiler.  DO NOT EDIT!
# Source: grpc/health/v1alpha/health.proto for package 'grpc.health.v1alpha'

require 'grpc'
require 'grpc/health/v1alpha/health'

module Grpc
  module Health
    module V1alpha
      module Health

        # TODO: add proto service documentation here
        class Service

          include GRPC::GenericService

          self.marshal_class_method = :encode
          self.unmarshal_class_method = :decode
          self.service_name = 'grpc.health.v1alpha.Health'

          rpc :Check, HealthCheckRequest, HealthCheckResponse
        end

        Stub = Service.rpc_stub_class
      end
    end
  end
end
