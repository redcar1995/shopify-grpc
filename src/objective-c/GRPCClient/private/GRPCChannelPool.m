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

#import "../internal/GRPCCallOptions+Internal.h"
#import "GRPCChannel.h"
#import "GRPCChannelFactory.h"
#import "GRPCChannelPool.h"
#import "GRPCConnectivityMonitor.h"
#import "GRPCCronetChannelFactory.h"
#import "GRPCInsecureChannelFactory.h"
#import "GRPCSecureChannelFactory.h"
#import "utilities.h"
#import "version.h"
#import "GRPCWrappedCall.h"
#import "GRPCCompletionQueue.h"

#import <GRPCClient/GRPCCall+Cronet.h>
#include <grpc/support/log.h>

extern const char *kCFStreamVarName;

static GRPCChannelPool *gChannelPool;
static dispatch_once_t gInitChannelPool;

/** When all calls of a channel are destroyed, destroy the channel after this much seconds. */
static const NSTimeInterval kDefaultChannelDestroyDelay = 30;

@implementation GRPCPooledChannel {
  GRPCChannelConfiguration *_channelConfiguration;
  NSTimeInterval _destroyDelay;

  NSHashTable<GRPCWrappedCall *> *_wrappedCalls;
  GRPCChannel *_wrappedChannel;
  NSDate *_lastTimedDestroy;
  dispatch_queue_t _timerQueue;
}

- (instancetype)initWithChannelConfiguration:(GRPCChannelConfiguration *)channelConfiguration {
  return [self initWithChannelConfiguration:channelConfiguration destroyDelay:kDefaultChannelDestroyDelay];
}

- (void)dealloc {
  if ([_wrappedCalls objectEnumerator].allObjects.count != 0) {
    NSEnumerator *enumerator = [_wrappedCalls objectEnumerator];
    GRPCWrappedCall *wrappedCall;
    while ((wrappedCall = [enumerator nextObject])) {
      [wrappedCall channelDisconnected];
    };
  }
}

- (GRPCWrappedCall *)wrappedCallWithPath:(NSString *)path
completionQueue:(GRPCCompletionQueue *)queue
callOptions:(GRPCCallOptions *)callOptions {
  NSAssert(path.length > 0, @"path must not be empty.");
  NSAssert(queue != nil, @"completionQueue must not be empty.");
  NSAssert(callOptions, @"callOptions must not be empty.");
  if (path.length == 0 || queue == nil || callOptions == nil) return NULL;

  GRPCWrappedCall *call = nil;

  @synchronized(self) {
    if (_wrappedChannel == nil) {
      _wrappedChannel = [[GRPCChannel alloc] initWithChannelConfiguration:_channelConfiguration];
      if (_wrappedChannel == nil) {
        NSAssert(_wrappedChannel != nil, @"Unable to get a raw channel for proxy.");
        return nil;
      }
    }
    _lastTimedDestroy = nil;

    grpc_call *unmanagedCall = [_wrappedChannel unmanagedCallWithPath:path
                                                      completionQueue:[GRPCCompletionQueue completionQueue]
                                                          callOptions:callOptions];
    if (unmanagedCall == NULL) {
      NSAssert(unmanagedCall != NULL, @"Unable to create grpc_call object");
      return nil;
    }

    call = [[GRPCWrappedCall alloc] initWithUnmanagedCall:unmanagedCall pooledChannel:self];
    if (call == nil) {
      NSAssert(call != nil, @"Unable to create GRPCWrappedCall object");
      return nil;
    }

    [_wrappedCalls addObject:call];
  }
  return call;
}

- (void)notifyWrappedCallDealloc:(GRPCWrappedCall *)wrappedCall {
  NSAssert(wrappedCall != nil, @"wrappedCall cannot be empty.");
  if (wrappedCall == nil) {
    return;
  }
  @synchronized(self) {
    if ([_wrappedCalls objectEnumerator].allObjects.count == 0) {
      NSDate *now = [NSDate date];
      _lastTimedDestroy = now;
      dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)_destroyDelay * NSEC_PER_SEC),
                     _timerQueue, ^{
                       @synchronized(self) {
                         if (self->_lastTimedDestroy == now) {
                           self->_wrappedChannel = nil;
                           self->_lastTimedDestroy = nil;
                         }
                       }
                     });
    }
  }
}

- (void)disconnect {
  NSHashTable<GRPCWrappedCall *> *copiedWrappedCalls = nil;
  @synchronized(self) {
    if (_wrappedChannel != nil) {
      _wrappedChannel = nil;
      copiedWrappedCalls = [_wrappedCalls copy];
      [_wrappedCalls removeAllObjects];
    }
  }
  NSEnumerator *enumerator = [copiedWrappedCalls objectEnumerator];
  GRPCWrappedCall *wrappedCall;
  while ((wrappedCall = [enumerator nextObject])) {
    [wrappedCall channelDisconnected];
  }
}

@end

@implementation GRPCPooledChannel (Test)

- (nullable instancetype)initWithChannelConfiguration:(GRPCChannelConfiguration *)channelConfiguration
                                         destroyDelay:(NSTimeInterval)destroyDelay {
  NSAssert(channelConfiguration != nil, @"channelConfiguration cannot be empty.");
  if (channelConfiguration == nil) {
    return nil;
  }

  if ((self = [super init])) {
    _channelConfiguration = channelConfiguration;
    _destroyDelay = destroyDelay;
    _wrappedCalls = [[NSHashTable alloc] initWithOptions:NSHashTableWeakMemory capacity:1];
    _wrappedChannel = nil;
    _lastTimedDestroy = nil;
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 110000 || __MAC_OS_X_VERSION_MAX_ALLOWED >= 101300
    if (@available(iOS 8.0, macOS 10.10, *)) {
      _timerQueue = dispatch_queue_create(NULL,
                                          dispatch_queue_attr_make_with_qos_class(DISPATCH_QUEUE_SERIAL, QOS_CLASS_DEFAULT, 0));
    } else {
#else
    {
#endif
      _timerQueue = dispatch_queue_create(NULL, DISPATCH_QUEUE_SERIAL);
    }
  }

  return self;
}

- (GRPCChannel *)wrappedChannel {
  GRPCChannel *channel = nil;
  @synchronized(self) {
    channel = _wrappedChannel;
  }
  return channel;
}

@end

@interface GRPCChannelPool ()

- (instancetype)initInstance NS_DESIGNATED_INITIALIZER;

@end

@implementation GRPCChannelPool {
  NSMutableDictionary<GRPCChannelConfiguration *, GRPCPooledChannel *> *_channelPool;
}

+ (instancetype)sharedInstance {
  dispatch_once(&gInitChannelPool, ^{
    gChannelPool =
        [[GRPCChannelPool alloc] initInstance];
    NSAssert(gChannelPool != nil, @"Cannot initialize global channel pool.");
  });
  return gChannelPool;
}

- (instancetype)initInstance {
  if ((self = [super init])) {
    _channelPool = [NSMutableDictionary dictionary];

    // Connectivity monitor is not required for CFStream
    char *enableCFStream = getenv(kCFStreamVarName);
    if (enableCFStream == nil || enableCFStream[0] != '1') {
      [GRPCConnectivityMonitor registerObserver:self selector:@selector(connectivityChange:)];
    }
  }
  return self;
}

- (void)dealloc {
  [GRPCConnectivityMonitor unregisterObserver:self];
}

- (GRPCPooledChannel *)channelWithHost:(NSString *)host callOptions:(GRPCCallOptions *)callOptions {
  NSAssert(host.length > 0, @"Host must not be empty.");
  NSAssert(callOptions != nil, @"callOptions must not be empty.");
  if (host.length == 0 || callOptions == nil) {
    return nil;
  }

  GRPCPooledChannel *pooledChannel = nil;
  GRPCChannelConfiguration *configuration =
      [[GRPCChannelConfiguration alloc] initWithHost:host callOptions:callOptions];
  @synchronized(self) {
    pooledChannel = _channelPool[configuration];
    if (pooledChannel == nil) {
      pooledChannel = [[GRPCPooledChannel alloc] initWithChannelConfiguration:configuration];
      _channelPool[configuration] = pooledChannel;
    }
  }
  return pooledChannel;
}

- (void)disconnectAllChannels {
  NSDictionary *copiedPooledChannels;
  @synchronized(self) {
    copiedPooledChannels = [NSDictionary dictionaryWithDictionary:_channelPool];
  }

  // Disconnect pooled channels.
  [copiedPooledChannels enumerateKeysAndObjectsUsingBlock:^(id  _Nonnull key, id  _Nonnull obj, BOOL * _Nonnull stop) {
    [obj disconnect];
  }];
}

- (void)connectivityChange:(NSNotification *)note {
  [self disconnectAllChannels];
}

@end

@implementation GRPCChannelPool (Test)

- (instancetype)initTestPool {
  return [self initInstance];
}

@end
