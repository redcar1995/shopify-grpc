/*
 *
 * Copyright 2018 gRPC authors.
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

#import "GRPCInsecureChannelFactory.h"

#import "ChannelArgsUtil.h"
#import "GRPCChannel.h"

NS_ASSUME_NONNULL_BEGIN

@implementation GRPCInsecureChannelFactory

+ (instancetype _Nullable)sharedInstance {
  static GRPCInsecureChannelFactory *instance;
  static dispatch_once_t onceToken;
  dispatch_once(&onceToken, ^{
    instance = [[self alloc] init];
  });
  return instance;
}

- (grpc_channel * _Nullable)createChannelWithHost:(NSString *)host
                                     channelArgs:(NSDictionary * _Nullable)args {
  grpc_channel_args *coreChannelArgs = GRPCBuildChannelArgs([args copy]);
  grpc_channel *unmanagedChannel =
      grpc_insecure_channel_create(host.UTF8String, coreChannelArgs, NULL);
  GRPCFreeChannelArgs(coreChannelArgs);
  return unmanagedChannel;
}

@end

NS_ASSUME_NONNULL_END
