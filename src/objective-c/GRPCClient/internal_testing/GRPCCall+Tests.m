/*
 *
 * Copyright 2015-2016, Google Inc.
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

#import "GRPCCall+Tests.h"

#import "../private/GRPCHost.h"
#import "../private/GRPCOpBatchLog.h"

@implementation GRPCCall (Tests)

+ (void)useTestCertsPath:(NSString *)certsPath
                testName:(NSString *)testName
                 forHost:(NSString *)host {
#ifdef GRPC_TEST_OBJC
  if (!host || !certsPath || !testName) {
    [NSException raise:NSInvalidArgumentException format:@"host, path and name must be provided."];
  }
  NSError *error = nil;
  NSString *certs = [NSString stringWithContentsOfFile:certsPath
                                                      encoding:NSUTF8StringEncoding
                                                      error:&error];
  if (error != nil) {
      [NSException raise:[error localizedDescription] format:@"failed to load certs"];
  }

  GRPCHost *hostConfig = [GRPCHost hostWithAddress:host];
  [hostConfig setTLSPEMRootCerts:certs withPrivateKey:nil withCertChain:nil error:nil];
  hostConfig.hostNameOverride = testName;
#else
  NSLog(@"This function is for internal testing of gRPC only. "
        "It is not part of gRPC's public interface. Do not use in production. "
        "To enable, set the preprocessor flag GRPC_TEST_OBJC.");
#endif
}

+ (void)useInsecureConnectionsForHost:(NSString *)host {
#ifdef GRPC_TEST_OBJC
  GRPCHost *hostConfig = [GRPCHost hostWithAddress:host];
  hostConfig.secure = NO;
#else
  NSLog(@"This function is for internal testing of gRPC only. "
        "It is not part of gRPC's public interface. Do not use in production. "
        "To enable, set the preprocessor flag GRPC_TEST_OBJC.");
#endif
}

+ (void)resetHostSettings {
#ifdef GRPC_TEST_OBJC
  [GRPCHost resetAllHostSettings];
#else
  NSLog(@"This function is for internal testing of gRPC only. "
        "It is not part of gRPC's public interface. Do not use in production. "
        "To enable, set the preprocessor flag GRPC_TEST_OBJC.");
#endif
}

+ (void)enableOpBatchLog:(BOOL)enabled {
#ifdef GRPC_TEST_OBJC
  [GRPCOpBatchLog enableOpBatchLog:enabled];
#else
  NSLog(@"This function is for internal testing of gRPC only. "
        "It is not part of gRPC's public interface. Do not use in production. "
        "To enable, set the preprocessor flag GRPC_TEST_OBJC.");
#endif
}

+ (NSArray *)obtainAndCleanOpBatchLog {
#ifdef GRPC_TEST_OBJC
  return [GRPCOpBatchLog obtainAndCleanOpBatchLog];
#else
  NSLog(@"This function is for internal testing of gRPC only. "
        "It is not part of gRPC's public interface. Do not use in production. "
        "To enable, set the preprocessor flag GRPC_TEST_OBJC.");
  return nil;
#endif
}

@end
