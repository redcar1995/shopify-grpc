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

#import "ViewController.h"

#import <gRPC/GRPCCall.h>
#import <gRPC/GRPCMethodName.h>
#import <gRPC/GRXWriter+Immediate.h>
#import <gRPC/GRXWriteable.h>
#import <RemoteTest/Messages.pb.h>

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
  [super viewDidLoad];
  // Do any additional setup after loading the view, typically from a nib.

  GRPCMethodName *method = [[GRPCMethodName alloc] initWithPackage:@"grpc.testing"
                                                         interface:@"TestService"
                                                            method:@"UnaryCall"];

  RMTSimpleRequest *request = [[[[[[RMTSimpleRequestBuilder alloc] init]
                                  setResponseSize:100]
                                 setFillUsername:YES]
                                setFillOauthScope:YES]
                               build];
  id<GRXWriter> requestsWriter = [GRXWriter writerWithValue:[request data]];

  GRPCCall *call = [[GRPCCall alloc] initWithHost:@"grpc-test.sandbox.google.com"
                                           method:method
                                   requestsWriter:requestsWriter];

  id<GRXWriteable> responsesWriteable = [[GRXWriteable alloc] initWithValueHandler:^(NSData *value) {
    RMTSimpleResponse *response = [RMTSimpleResponse parseFromData:value];
    NSLog(@"Received response:\n%@", response);
  } completionHandler:^(NSError *errorOrNil) {
    NSLog(@"Finished with error: %@", errorOrNil);
  }];

  [call startWithWriteable:responsesWriteable];
}

- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning];
  // Dispose of any resources that can be recreated.
}

@end
