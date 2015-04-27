// Generated by the protocol buffer compiler.  DO NOT EDIT!

#import "Test.pb.h"

#import <gRPC/GRXWriteable.h>
#import <gRPC/GRXWriter+Immediate.h>
#import <gRPC/ProtoRPC.h>
// @@protoc_insertion_point(imports)

@implementation RMTTestRoot
static PBExtensionRegistry* extensionRegistry = nil;
+ (PBExtensionRegistry*) extensionRegistry {
  return extensionRegistry;
}

+ (void) initialize {
  if (self == [RMTTestRoot class]) {
    PBMutableExtensionRegistry* registry = [PBMutableExtensionRegistry registry];
    [self registerAllExtensions:registry];
    [RMTEmptyRoot registerAllExtensions:registry];
    [RMTMessagesRoot registerAllExtensions:registry];
    [ObjectivecDescriptorRoot registerAllExtensions:registry];
    extensionRegistry = registry;
  }
}
+ (void) registerAllExtensions:(PBMutableExtensionRegistry*) registry {
}
@end


static NSString *const kPackageName = @"grpc.testing";
static NSString *const kServiceName = @"TestService";

@implementation RMTTestService

// Designated initializer
- (instancetype)initWithHost:(NSString *)host {
  return (self = [super initWithHost:host packageName:kPackageName serviceName:kServiceName]);
}

- (instancetype)initWithHost:(NSString *)host
                 packageName:(NSString *)packageName
                 serviceName:(NSString *)serviceName {
  return [self initWithHost:host];
}

#pragma mark EmptyCall(grpc.testing.Empty) returns (grpc.testing.Empty)

// One empty request followed by one empty response.
- (void)emptyCallWithRequest:(RMTEmpty *)request
                     handler:(void(^)(RMTEmpty *response, NSError *error))handler {
  [[self RPCToEmptyCallWithRequest:request handler:handler] start];
}

// Returns a not-yet-started RPC object.
- (ProtoRPC *)RPCToEmptyCallWithRequest:(RMTEmpty *)request
                                handler:(void(^)(RMTEmpty *response, NSError *error))handler {
  return [self RPCToMethod:@"EmptyCall"
            requestsWriter:[GRXWriter writerWithValue:request]
             responseClass:[RMTEmpty class]
        responsesWriteable:[GRXWriteable writeableWithSingleValueHandler:handler]];
}


#pragma mark UnaryCall(SimpleRequest) returns (SimpleResponse)

// One request followed by one response.
- (void)unaryCallWithRequest:(RMTSimpleRequest *)request
                     handler:(void(^)(RMTSimpleResponse *response, NSError *error))handler {
  [[self RPCToUnaryCallWithRequest:request handler:handler] start];
}

// Returns a not-yet-started RPC object.
- (ProtoRPC *)RPCToUnaryCallWithRequest:(RMTSimpleRequest *)request
                                handler:(void(^)(RMTSimpleResponse *response, NSError *error))handler {
  return [self RPCToMethod:@"UnaryCall"
            requestsWriter:[GRXWriter writerWithValue:request]
             responseClass:[RMTSimpleResponse class]
        responsesWriteable:[GRXWriteable writeableWithSingleValueHandler:handler]];
}


#pragma mark StreamingOutputCall(StreamingOutputCallRequest) returns (stream StreamingOutputCallResponse)

// One request followed by a sequence of responses (streamed download).
// The server returns the payload with client desired type and sizes.
- (void)streamingOutputCallWithRequest:(RMTStreamingOutputCallRequest *)request
                               handler:(void(^)(BOOL done, RMTStreamingOutputCallResponse *response, NSError *error))handler {
  [[self RPCToStreamingOutputCallWithRequest:request handler:handler] start];
}

// Returns a not-yet-started RPC object.
- (ProtoRPC *)RPCToStreamingOutputCallWithRequest:(RMTStreamingOutputCallRequest *)request
                                          handler:(void(^)(BOOL done, RMTStreamingOutputCallResponse *response, NSError *error))handler {
  return [self RPCToMethod:@"StreamingOutputCall"
            requestsWriter:[GRXWriter writerWithValue:request]
             responseClass:[RMTStreamingOutputCallResponse class]
        responsesWriteable:[GRXWriteable writeableWithStreamHandler:handler]];
}


#pragma mark StreamingInputCall(stream StreamingInputCallRequest) returns (StreamingInputCallResponse)

// A sequence of requests followed by one response (streamed upload).
// The server returns the aggregated size of client payload as the result.
- (void)streamingInputCallWithRequestsWriter:(id<GRXWriter>)requestsWriter
                                     handler:(void(^)(RMTStreamingInputCallResponse *response, NSError *error))handler {
  [[self RPCToStreamingInputCallWithRequestsWriter:requestsWriter handler:handler] start];
}

// Returns a not-yet-started RPC object.
- (ProtoRPC *)RPCToStreamingInputCallWithRequestsWriter:(id<GRXWriter>)requestsWriter
                                                handler:(void(^)(RMTStreamingInputCallResponse *response, NSError *error))handler {
  return [self RPCToMethod:@"StreamingInputCall"
            requestsWriter:requestsWriter
             responseClass:[RMTStreamingInputCallResponse class]
        responsesWriteable:[GRXWriteable writeableWithSingleValueHandler:handler]];
}


#pragma mark FullDuplexCall(stream StreamingOutputCallRequest) returns (stream StreamingOutputCallResponse)

// A sequence of requests with each request served by the server immediately.
// As one request could lead to multiple responses, this interface
// demonstrates the idea of full duplexing.
- (void)fullDuplexCallWithRequestsWriter:(id<GRXWriter>)requestsWriter
                                 handler:(void(^)(BOOL done, RMTStreamingOutputCallResponse *response, NSError *error))handler {
  [[self RPCToFullDuplexCallWithRequestsWriter:requestsWriter handler:handler] start];
}

// Returns a not-yet-started RPC object.
- (ProtoRPC *)RPCToFullDuplexCallWithRequestsWriter:(id<GRXWriter>)requestsWriter
                                            handler:(void(^)(BOOL done, RMTStreamingOutputCallResponse *response, NSError *error))handler {
  return [self RPCToMethod:@"FullDuplexCall"
            requestsWriter:requestsWriter
             responseClass:[RMTStreamingOutputCallResponse class]
        responsesWriteable:[GRXWriteable writeableWithStreamHandler:handler]];
}


#pragma mark HalfDuplexCall(stream StreamingOutputCallRequest) returns (stream StreamingOutputCallResponse)

// A sequence of requests followed by a sequence of responses.
// The server buffers all the client requests and then serves them in order. A
// stream of responses are returned to the client when the server starts with
// first request.
- (void)halfDuplexCallWithRequestsWriter:(id<GRXWriter>)requestsWriter
                                 handler:(void(^)(BOOL done, RMTStreamingOutputCallResponse *response, NSError *error))handler {
  [[self RPCToHalfDuplexCallWithRequestsWriter:requestsWriter handler:handler] start];
}

// Returns a not-yet-started RPC object.
- (ProtoRPC *)RPCToHalfDuplexCallWithRequestsWriter:(id<GRXWriter>)requestsWriter
                                            handler:(void(^)(BOOL done, RMTStreamingOutputCallResponse *response, NSError *error))handler {
  return [self RPCToMethod:@"HalfDuplexCall"
            requestsWriter:requestsWriter
             responseClass:[RMTStreamingOutputCallResponse class]
        responsesWriteable:[GRXWriteable writeableWithStreamHandler:handler]];
}

@end

// @@protoc_insertion_point(global_scope)
