/*
 *
 * Copyright 2015 gRPC authors.
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

#import <Foundation/Foundation.h>
#import <GRPCClient/GRPCCall.h>

#import "ProtoMethod.h"

NS_ASSUME_NONNULL_BEGIN

@class GPBMessage;

/** An object can implement this protocol to receive responses from server from a call. */
@protocol GRPCProtoResponseHandler<NSObject>

@optional

/**
 * Issued when initial metadata is received from the server. The task must be scheduled onto the
 * dispatch queue in property \a dispatchQueue. */
- (void)receivedInitialMetadata:(NSDictionary *_Nullable)initialMetadata;

/**
 * Issued when a message is received from the server. The message is the deserialized proto object.
 * The task must be scheduled onto the dispatch queue in property \a dispatchQueue.
 */
- (void)receivedProtoMessage:(GPBMessage *_Nullable)message;

/**
 * Issued when a call finished. If the call finished successfully, \a error is nil and \a
 * trainingMetadata consists any trailing metadata received from the server. Otherwise, \a error
 * is non-nil and contains the corresponding error information, including gRPC error codes and
 * error descriptions. The task must be scheduled onto the dispatch queue in property
 * \a dispatchQueue.
 */
- (void)closedWithTrailingMetadata:(NSDictionary *_Nullable)trailingMetadata
                             error:(NSError *_Nullable)error;

@required

/**
 * All the responses must be issued to a user-provided dispatch queue. This property specifies the
 * dispatch queue to be used for issuing the notifications.
 */
@property(atomic, readonly) dispatch_queue_t dispatchQueue;

@end

/** A unary-request RPC call with Protobuf. */
@interface GRPCUnaryProtoCall : NSObject

- (instancetype)init NS_UNAVAILABLE;

+ (instancetype) new NS_UNAVAILABLE;

/**
 * Users should not use this initializer directly. Call objects will be created, initialized, and
 * returned to users by methods of the generated service.
 */
- (instancetype)initWithRequestOptions:(GRPCRequestOptions *)requestOptions
                               message:(GPBMessage *)message
                       responseHandler:(id<GRPCProtoResponseHandler>)handler
                           callOptions:(GRPCCallOptions *_Nullable)callOptions
                         responseClass:(Class)responseClass NS_DESIGNATED_INITIALIZER;

/**
 * Cancel the request of this call at best effort. It attempts to notify the server that the RPC
 * should be cancelled, and issue closedWithTrailingMetadata:error: callback with error code
 * CANCELED if no other error code has already been issued.
 */
- (void)cancel;

@end

/** A client-streaming RPC call with Protobuf. */
@interface GRPCStreamingProtoCall : NSObject

- (instancetype)init NS_UNAVAILABLE;

+ (instancetype) new NS_UNAVAILABLE;

/**
 * Users should not use this initializer directly. Call objects will be created, initialized, and
 * returned to users by methods of the generated service.
 */
- (instancetype)initWithRequestOptions:(GRPCRequestOptions *)requestOptions
                       responseHandler:(id<GRPCProtoResponseHandler>)handler
                           callOptions:(GRPCCallOptions *_Nullable)callOptions
                         responseClass:(Class)responseClass NS_DESIGNATED_INITIALIZER;

/**
 * Cancel the request of this call at best effort. It attempts to notify the server that the RPC
 * should be cancelled, and issue closedWithTrailingMetadata:error: callback with error code
 * CANCELED if no other error code has already been issued.
 */
- (void)cancel;

/**
 * Send a message to the server. The message should be a Protobuf message which will be serialized
 * internally.
 */
- (void)writeMessage:(GPBMessage *)message;

/**
 * Finish the RPC request and half-close the call. The server may still send messages and/or
 * trailers to the client.
 */
- (void)finish;

@end

NS_ASSUME_NONNULL_END

__attribute__((deprecated("Please use GRPCProtoCall."))) @interface ProtoRPC
    : GRPCCall

      /**
       * host parameter should not contain the scheme (http:// or https://), only the name or IP
       * addr and the port number, for example @"localhost:5050".
       */
      -
      (instancetype _Null_unspecified)initWithHost : (NSString *_Null_unspecified)host method
    : (GRPCProtoMethod *_Null_unspecified)method requestsWriter
    : (GRXWriter *_Null_unspecified)requestsWriter responseClass
    : (Class _Null_unspecified)responseClass responsesWriteable
    : (id<GRXWriteable> _Null_unspecified)responsesWriteable NS_DESIGNATED_INITIALIZER;

- (void)start;
@end

/**
 * This subclass is empty now. Eventually we'll remove ProtoRPC class
 * to avoid potential naming conflict
 */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    @interface GRPCProtoCall : ProtoRPC
#pragma clang diagnostic pop

                               @end
