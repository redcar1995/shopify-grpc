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

#import "ProtoRPC.h"

#if GPB_USE_PROTOBUF_FRAMEWORK_IMPORTS
#import <Protobuf/GPBProtocolBuffers.h>
#else
#import <GPBProtocolBuffers.h>
#endif
#import <GRPCClient/GRPCCall.h>
#import <RxLibrary/GRXWriteable.h>
#import <RxLibrary/GRXWriter+Transformations.h>

@implementation GRPCUnaryProtoCall {
  GRPCStreamingProtoCall *_call;
}

- (instancetype)initWithRequestOptions:(GRPCRequestOptions *)requestOptions
                               message:(GPBMessage *)message
                       responseHandler:(id<GRPCResponseHandler>)handler
                           callOptions:(GRPCCallOptions *)callOptions
                         responseClass:(Class)responseClass {
  if ((self = [super init])) {
    _call = [[GRPCStreamingProtoCall alloc] initWithRequestOptions:requestOptions
                                                   responseHandler:handler
                                                       callOptions:callOptions
                                                     responseClass:responseClass];
    [_call writeWithMessage:message];
    [_call finish];
  }
  return self;
}

- (void)cancel {
  [_call cancel];
  _call = nil;
}

@end

@interface GRPCStreamingProtoCall ()<GRPCResponseHandler>

@end

@implementation GRPCStreamingProtoCall {
  GRPCRequestOptions *_requestOptions;
  id<GRPCResponseHandler> _handler;
  GRPCCallOptions *_callOptions;
  Class _responseClass;

  GRPCCall2 *_call;
  dispatch_queue_t _dispatchQueue;
}

- (instancetype)initWithRequestOptions:(GRPCRequestOptions *)requestOptions
                       responseHandler:(id<GRPCResponseHandler>)handler
                           callOptions:(GRPCCallOptions *)callOptions
                         responseClass:(Class)responseClass {
  if ((self = [super init])) {
    _requestOptions = [requestOptions copy];
    _handler = handler;
    _callOptions = [callOptions copy];
    _responseClass = responseClass;
    _dispatchQueue = dispatch_queue_create(nil, DISPATCH_QUEUE_SERIAL);

    [self start];
  }
  return self;
}

- (void)start {
  _call = [[GRPCCall2 alloc] initWithRequestOptions:_requestOptions
                                            handler:self
                                        callOptions:_callOptions];
  [_call start];
}

- (void)cancel {
  dispatch_async(_dispatchQueue, ^{
    if (_call) {
      [_call cancel];
      _call = nil;
    }
    if (_handler) {
      id<GRPCResponseHandler> handler = _handler;
      dispatch_async(handler.dispatchQueue, ^{
        [handler closedWithTrailingMetadata:nil
                                      error:[NSError errorWithDomain:kGRPCErrorDomain
                                                                code:GRPCErrorCodeCancelled
                                                            userInfo:@{
                                                              NSLocalizedDescriptionKey :
                                                                  @"Canceled by app"
                                                            }]];
      });
      _handler = nil;
    }
  });
}

- (void)writeWithMessage:(GPBMessage *)message {
  if (![message isKindOfClass:[GPBMessage class]]) {
    [NSException raise:NSInvalidArgumentException format:@"Data must be a valid protobuf type."];
  }

  dispatch_async(_dispatchQueue, ^{
    if (_call) {
      [_call writeWithData:[message data]];
    }
  });
}

- (void)finish {
  dispatch_async(_dispatchQueue, ^{
    if (_call) {
      [_call finish];
      _call = nil;
    }
  });
}

- (void)receivedInitialMetadata:(NSDictionary *)initialMetadata {
  if (_handler) {
    id<GRPCResponseHandler> handler = _handler;
    dispatch_async(handler.dispatchQueue, ^{
      [handler receivedInitialMetadata:initialMetadata];
    });
  }
}

- (void)receivedMessage:(NSData *)message {
  if (_handler) {
    id<GRPCResponseHandler> handler = _handler;
    NSError *error = nil;
    id parsed = [_responseClass parseFromData:message error:&error];
    if (parsed) {
      dispatch_async(handler.dispatchQueue, ^{
        [handler receivedMessage:parsed];
      });
    } else {
      dispatch_async(handler.dispatchQueue, ^{
        [handler closedWithTrailingMetadata:nil error:error];
      });
      handler = nil;
      [_call cancel];
      _call = nil;
    }
  }
}

- (void)closedWithTrailingMetadata:(NSDictionary *)trailingMetadata error:(NSError *)error {
  if (_handler) {
    id<GRPCResponseHandler> handler = _handler;
    dispatch_async(handler.dispatchQueue, ^{
      [handler closedWithTrailingMetadata:trailingMetadata error:error];
    });
    _handler = nil;
  }
  if (_call) {
    [_call cancel];
    _call = nil;
  }
}

- (dispatch_queue_t)dispatchQueue {
  return _dispatchQueue;
}

@end

static NSError *ErrorForBadProto(id proto, Class expectedClass, NSError *parsingError) {
  NSDictionary *info = @{
    NSLocalizedDescriptionKey : @"Unable to parse response from the server",
    NSLocalizedRecoverySuggestionErrorKey :
        @"If this RPC is idempotent, retry "
        @"with exponential backoff. Otherwise, query the server status before "
        @"retrying.",
    NSUnderlyingErrorKey : parsingError,
    @"Expected class" : expectedClass,
    @"Received value" : proto,
  };
  // TODO(jcanizales): Use kGRPCErrorDomain and GRPCErrorCodeInternal when they're public.
  return [NSError errorWithDomain:@"io.grpc" code:13 userInfo:info];
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-implementations"
@implementation ProtoRPC {
#pragma clang diagnostic pop
  id<GRXWriteable> _responseWriteable;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wobjc-designated-initializers"
- (instancetype)initWithHost:(NSString *)host
                        path:(NSString *)path
              requestsWriter:(GRXWriter *)requestsWriter {
  [NSException raise:NSInvalidArgumentException
              format:@"Please use ProtoRPC's designated initializer instead."];
  return nil;
}
#pragma clang diagnostic pop

// Designated initializer
- (instancetype)initWithHost:(NSString *)host
                      method:(GRPCProtoMethod *)method
              requestsWriter:(GRXWriter *)requestsWriter
               responseClass:(Class)responseClass
          responsesWriteable:(id<GRXWriteable>)responsesWriteable {
  // Because we can't tell the type system to constrain the class, we need to check at runtime:
  if (![responseClass respondsToSelector:@selector(parseFromData:error:)]) {
    [NSException raise:NSInvalidArgumentException
                format:@"A protobuf class to parse the responses must be provided."];
  }
  // A writer that serializes the proto messages to send.
  GRXWriter *bytesWriter = [requestsWriter map:^id(GPBMessage *proto) {
    if (![proto isKindOfClass:GPBMessage.class]) {
      [NSException raise:NSInvalidArgumentException
                  format:@"Request must be a proto message: %@", proto];
    }
    return [proto data];
  }];
  if ((self = [super initWithHost:host path:method.HTTPPath requestsWriter:bytesWriter])) {
    __weak ProtoRPC *weakSelf = self;

    // A writeable that parses the proto messages received.
    _responseWriteable = [[GRXWriteable alloc] initWithValueHandler:^(NSData *value) {
      // TODO(jcanizales): This is done in the main thread, and needs to happen in another thread.
      NSError *error = nil;
      id parsed = [responseClass parseFromData:value error:&error];
      if (parsed) {
        [responsesWriteable writeValue:parsed];
      } else {
        [weakSelf finishWithError:ErrorForBadProto(value, responseClass, error)];
      }
    }
        completionHandler:^(NSError *errorOrNil) {
          [responsesWriteable writesFinishedWithError:errorOrNil];
        }];
  }
  return self;
}

- (void)start {
  [self startWithWriteable:_responseWriteable];
}

- (void)startWithWriteable:(id<GRXWriteable>)writeable {
  [super startWithWriteable:writeable];
  // Break retain cycles.
  _responseWriteable = nil;
}
@end

@implementation GRPCProtoCall

@end
