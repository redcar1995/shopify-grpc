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

#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>

#import <gRPC/GRPCCall.h>
#import <gRPC/GRPCMethodName.h>
#import <gRPC/GRXWriter+Immediate.h>
#import <gRPC/GRXWriteable.h>
#import <RemoteTest/Messages.pbobjc.h>

// These are a few tests similar to InteropTests, but which use the generic gRPC client (GRPCCall)
// rather than a generated proto library on top of it.

static NSString * const kHostAddress = @"grpc-test.sandbox.google.com";
static NSString * const kPackage = @"grpc.testing";
static NSString * const kService = @"TestService";

static GRPCMethodName *kInexistentMethod;
static GRPCMethodName *kEmptyCallMethod;
static GRPCMethodName *kUnaryCallMethod;

@interface GRPCClientTests : XCTestCase
@end

@implementation GRPCClientTests

- (void)setUp {
  // This method isn't implemented by the remote server.
  kInexistentMethod = [[GRPCMethodName alloc] initWithPackage:kPackage
                                                    interface:kService
                                                       method:@"Inexistent"];
  kEmptyCallMethod = [[GRPCMethodName alloc] initWithPackage:kPackage
                                                   interface:kService
                                                      method:@"EmptyCall"];
  kUnaryCallMethod = [[GRPCMethodName alloc] initWithPackage:kPackage
                                                   interface:kService
                                                      method:@"UnaryCall"];
}

- (void)testConnectionToRemoteServer {
  __weak XCTestExpectation *expectation = [self expectationWithDescription:@"Server reachable."];

  GRPCCall *call = [[GRPCCall alloc] initWithHost:kHostAddress
                                           method:kInexistentMethod
                                   requestsWriter:[GRXWriter writerWithValue:[NSData data]]];

  id<GRXWriteable> responsesWriteable = [[GRXWriteable alloc] initWithValueHandler:^(NSData *value) {
    XCTFail(@"Received unexpected response: %@", value);
  } completionHandler:^(NSError *errorOrNil) {
    XCTAssertNotNil(errorOrNil, @"Finished without error!");
    // TODO(jcanizales): The server should return code 12 UNIMPLEMENTED, not 5 NOT FOUND.
    XCTAssertEqual(errorOrNil.code, 5, @"Finished with unexpected error: %@", errorOrNil);
    [expectation fulfill];
  }];

  [call startWithWriteable:responsesWriteable];

  [self waitForExpectationsWithTimeout:2. handler:nil];
}

- (void)testEmptyRPC {
  __weak XCTestExpectation *response = [self expectationWithDescription:@"Empty response received."];
  __weak XCTestExpectation *completion = [self expectationWithDescription:@"Empty RPC completed."];

  GRPCCall *call = [[GRPCCall alloc] initWithHost:kHostAddress
                                           method:kEmptyCallMethod
                                   requestsWriter:[GRXWriter writerWithValue:[NSData data]]];

  id<GRXWriteable> responsesWriteable = [[GRXWriteable alloc] initWithValueHandler:^(NSData *value) {
    XCTAssertNotNil(value, @"nil value received as response.");
    XCTAssertEqual([value length], 0, @"Non-empty response received: %@", value);
    [response fulfill];
  } completionHandler:^(NSError *errorOrNil) {
    XCTAssertNil(errorOrNil, @"Finished with unexpected error: %@", errorOrNil);
    [completion fulfill];
  }];

  [call startWithWriteable:responsesWriteable];

  [self waitForExpectationsWithTimeout:2. handler:nil];
}

- (void)testSimpleProtoRPC {
  __weak XCTestExpectation *response = [self expectationWithDescription:@"Expected response."];
  __weak XCTestExpectation *completion = [self expectationWithDescription:@"RPC completed."];

  RMTSimpleRequest *request = [RMTSimpleRequest message];
  request.responseSize = 100;
  request.fillUsername = YES;
  request.fillOauthScope = YES;
  id<GRXWriter> requestsWriter = [GRXWriter writerWithValue:[request data]];

  GRPCCall *call = [[GRPCCall alloc] initWithHost:kHostAddress
                                           method:kUnaryCallMethod
                                   requestsWriter:requestsWriter];

  id<GRXWriteable> responsesWriteable = [[GRXWriteable alloc] initWithValueHandler:^(NSData *value) {
    XCTAssertNotNil(value, @"nil value received as response.");
    XCTAssertGreaterThan(value.length, 0, @"Empty response received.");
    RMTSimpleResponse *responseProto = [RMTSimpleResponse parseFromData:value error:NULL];
    // We expect empty strings, not nil:
    XCTAssertNotNil(responseProto.username, @"Response's username is nil.");
    XCTAssertNotNil(responseProto.oauthScope, @"Response's OAuth scope is nil.");
    [response fulfill];
  } completionHandler:^(NSError *errorOrNil) {
    XCTAssertNil(errorOrNil, @"Finished with unexpected error: %@", errorOrNil);
    [completion fulfill];
  }];

  [call startWithWriteable:responsesWriteable];

  [self waitForExpectationsWithTimeout:2. handler:nil];
}

- (void)testMetadata {
  __weak XCTestExpectation *expectation = [self expectationWithDescription:@"RPC unauthorized."];

  RMTSimpleRequest *request = [RMTSimpleRequest message];
  request.fillUsername = YES;
  request.fillOauthScope = YES;
  id<GRXWriter> requestsWriter = [GRXWriter writerWithValue:[request data]];

  GRPCCall *call = [[GRPCCall alloc] initWithHost:kHostAddress
                                           method:kUnaryCallMethod
                                   requestsWriter:requestsWriter];

  call.requestMetadata[@"Authorization"] = @"Bearer bogusToken";

  id<GRXWriteable> responsesWriteable = [[GRXWriteable alloc] initWithValueHandler:^(NSData *value) {
    XCTFail(@"Received unexpected response: %@", value);
  } completionHandler:^(NSError *errorOrNil) {
    XCTAssertNotNil(errorOrNil, @"Finished without error!");
    XCTAssertEqual(errorOrNil.code, 16, @"Finished with unexpected error: %@", errorOrNil);
    NSString *challengeHeader = call.responseMetadata[@"www-authenticate"][0];
    XCTAssertGreaterThan(challengeHeader.length, 0,
                         @"No challenge in response headers %@", call.responseMetadata);
    [expectation fulfill];
  }];

  [call startWithWriteable:responsesWriteable];

  [self waitForExpectationsWithTimeout:2. handler:nil];
}

@end
