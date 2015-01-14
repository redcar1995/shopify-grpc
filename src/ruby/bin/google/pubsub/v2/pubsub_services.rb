# Generated by the protocol buffer compiler.  DO NOT EDIT!
# Source: google/pubsub/v2/pubsub.proto for package 'google.pubsub.v2'

require 'grpc'
require 'google/pubsub/v2/pubsub'

module Google
  module Pubsub
    module V2
      module PublisherService

        # TODO: add proto service documentation here
        class Service

          include GRPC::GenericService

          self.marshal_class_method = :encode
          self.unmarshal_class_method = :decode
          self.service_name = 'google.pubsub.v2.PublisherService'

          rpc :CreateTopic, Topic, Topic
          rpc :Publish, PublishRequest, PublishResponse
          rpc :GetTopic, GetTopicRequest, Topic
          rpc :ListTopics, ListTopicsRequest, ListTopicsResponse
          rpc :DeleteTopic, DeleteTopicRequest, Empty
        end

        Stub = Service.rpc_stub_class
      end
      module SubscriberService

        # TODO: add proto service documentation here
        class Service

          include GRPC::GenericService

          self.marshal_class_method = :encode
          self.unmarshal_class_method = :decode
          self.service_name = 'google.pubsub.v2.SubscriberService'

          rpc :CreateSubscription, Subscription, Subscription
          rpc :GetSubscription, GetSubscriptionRequest, Subscription
          rpc :ListSubscriptions, ListSubscriptionsRequest, ListSubscriptionsResponse
          rpc :DeleteSubscription, DeleteSubscriptionRequest, Empty
          rpc :TruncateSubscription, TruncateSubscriptionRequest, Empty
          rpc :DetachSubscription, DetachSubscriptionRequest, Empty
          rpc :ModifyAckDeadline, ModifyAckDeadlineRequest, Empty
          rpc :Acknowledge, AcknowledgeRequest, Empty
          rpc :Pull, PullRequest, PullResponse
          rpc :ModifyPushConfig, ModifyPushConfigRequest, Empty
        end

        Stub = Service.rpc_stub_class
      end
      module PushEndpointService

        # TODO: add proto service documentation here
        class Service

          include GRPC::GenericService

          self.marshal_class_method = :encode
          self.unmarshal_class_method = :decode
          self.service_name = 'google.pubsub.v2.PushEndpointService'

          rpc :HandlePubsubEvent, PubsubEvent, HandlePubsubEventResponse
        end

        Stub = Service.rpc_stub_class
      end
    end
  end
end
