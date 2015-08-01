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

#import "GRPCChannel.h"

#include <grpc/grpc.h>

#import "GRPCCompletionQueue.h"
#import "GRPCSecureChannel.h"
#import "GRPCUnsecuredChannel.h"

@implementation GRPCChannel {
  grpc_channel *_unmanagedChannel;
  NSString *_hostName;
  GRPCCompletionQueue *_queue;
}

+ (instancetype)channelToHost:(NSString *)host {
  // TODO(mlumish): Investigate whether a cache with strong links is a good idea
  static NSMutableDictionary *channelCache;
  static dispatch_once_t cacheInitialization;
  dispatch_once(&cacheInitialization, ^{
    channelCache = [NSMutableDictionary dictionary];
  });
  GRPCChannel *channel = channelCache[host];
  if (!channel) {
    channel = [self uncachedChannelToHost:host];
    channelCache[host] = channel;
  }
  return channel;
}

+ (instancetype)uncachedChannelToHost:(NSString *)host {
  if (![host rangeOfString:@"://"].length) {
    // No scheme provided; assume https.
    host = [@"https://" stringByAppendingString:host];
  }
  NSURL *hostURL = [NSURL URLWithString:host];
  if (!hostURL) {
    [NSException raise:NSInvalidArgumentException format:@"Invalid URL: %@", host];
  }
  if ([hostURL.scheme isEqualToString:@"https"]) {
    host = [@[hostURL.host, hostURL.port ?: @443] componentsJoinedByString:@":"];
    return [[GRPCSecureChannel alloc] initWithHost:host];
  }
  if ([hostURL.scheme isEqualToString:@"http"]) {
    host = [@[hostURL.host, hostURL.port ?: @80] componentsJoinedByString:@":"];
    return [[GRPCUnsecuredChannel alloc] initWithHost:host];
  }
  [NSException raise:NSInvalidArgumentException
              format:@"URL scheme %@ isn't supported.", hostURL.scheme];
  return nil; // silence warning.
}

- (instancetype)init {
  return [self initWithChannel:NULL hostName:nil];
}

- (instancetype)initWithChannel:(struct grpc_channel *)unmanagedChannel
                       hostName:(NSString *)hostName {
  if (!unmanagedChannel || !hostName) {
    [NSException raise:NSInvalidArgumentException
                format:@"Neither unmanagedChannel nor hostName can be nil."];
  }
  if ((self = [super init])) {
    _unmanagedChannel = unmanagedChannel;
    _hostName = hostName;
    // In case sharing the queue creates contention, we can change it to one per grpc_call.
    _queue = [GRPCCompletionQueue completionQueue];
    if (!_queue) {
      return nil;
    }
  }
  return self;
}

- (grpc_call *)unmanagedCallWithPath:(NSString *)path {
  return grpc_channel_create_call(_unmanagedChannel,
                                  _queue.unmanagedQueue,
                                  path.UTF8String,
                                  _hostName.UTF8String,
                                  gpr_inf_future(GPR_CLOCK_REALTIME));
}

- (void)dealloc {
  // TODO(jcanizales): Be sure to add a test with a server that closes the connection prematurely,
  // as in the past that made this call to crash.
  grpc_channel_destroy(_unmanagedChannel);
}
@end
