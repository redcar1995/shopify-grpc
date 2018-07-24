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

#ifndef GRPC_CORE_LIB_CHANNEL_CHANNELZ_H
#define GRPC_CORE_LIB_CHANNEL_CHANNELZ_H

#include <grpc/impl/codegen/port_platform.h>

#include <grpc/grpc.h>

#include "src/core/lib/channel/channel_trace.h"
#include "src/core/lib/gprpp/manual_constructor.h"
#include "src/core/lib/gprpp/ref_counted.h"
#include "src/core/lib/gprpp/ref_counted_ptr.h"
#include "src/core/lib/iomgr/error.h"
#include "src/core/lib/iomgr/exec_ctx.h"
#include "src/core/lib/json/json.h"

// Channel arg key for client channel factory.
#define GRPC_ARG_CHANNELZ_CHANNEL_NODE_CREATION_FUNC \
  "grpc.channelz_channel_node_creation_func"

// Channel arg key to signal that the channel is an internal channel.
#define GRPC_ARG_CHANNELZ_CHANNEL_IS_INTERNAL_CHANNEL \
  "grpc.channelz_channel_is_internal_channel"

namespace grpc_core {
namespace channelz {

namespace testing {
class CallCountingAndTracingNodePeer;
}

// base class for all channelz entities
class BaseNode : public RefCounted<BaseNode> {
 public:
  // There are only four high level channelz entities. However, to support
  // GetTopChannelsRequest, we split the Channel entity into two different
  // types. All children of BaseNode must be one of these types.
  enum class EntityType {
    kTopLevelChannel,
    kInternalChannel,
    kSubchannel,
    kServer,
    kSocket,
  };

  BaseNode(EntityType type);
  virtual ~BaseNode();

  // All children must implement this function.
  virtual grpc_json* RenderJson() GRPC_ABSTRACT;

  // Renders the json and returns allocated string that must be freed by the
  // caller.
  char* RenderJsonString();

  EntityType type() const { return type_; }
  intptr_t uuid() const { return uuid_; }

 private:
  friend class ChannelTrace;
  EntityType type_;
  const intptr_t uuid_;
};

// This class is the parent for the channelz entities that deal with Channels
// Subchannels, and Servers, since those have similar proto definitions.
// This class has the ability to:
//   - track calls_{started,succeeded,failed}
//   - track last_call_started_timestamp
//   - hold the channel trace.
//   - perform common rendering.
class CallCountingAndTracingNode {
 public:
  CallCountingAndTracingNode(size_t channel_tracer_max_nodes);
  ~CallCountingAndTracingNode();

  void RecordCallStarted();
  void RecordCallFailed() {
    gpr_atm_no_barrier_fetch_add(&calls_failed_, (gpr_atm(1)));
  }
  void RecordCallSucceeded() {
    gpr_atm_no_barrier_fetch_add(&calls_succeeded_, (gpr_atm(1)));
  }
  ChannelTrace* trace() { return trace_.get(); }

  // Common rendering of the channel trace.
  void PopulateTrace(grpc_json* json);

  // Common rendering of the call count data and last_call_started_timestamp.
  void PopulateCallData(grpc_json* json);

 private:
  // testing peer friend.
  friend class testing::CallCountingAndTracingNodePeer;

  gpr_atm calls_started_ = 0;
  gpr_atm calls_succeeded_ = 0;
  gpr_atm calls_failed_ = 0;
  gpr_atm last_call_started_millis_ = 0;
  ManualConstructor<ChannelTrace> trace_;
};

// Handles channelz bookkeeping for channels
class ChannelNode : public BaseNode {
 public:
  static RefCountedPtr<ChannelNode> MakeChannelNode(
      grpc_channel* channel, size_t channel_tracer_max_nodes,
      bool is_top_level_channel);

  ChannelNode(grpc_channel* channel, size_t channel_tracer_max_nodes,
              bool is_top_level_channel);
  ~ChannelNode() override;

  grpc_json* RenderJson() override;

  void MarkChannelDestroyed() {
    GPR_ASSERT(channel_ != nullptr);
    channel_ = nullptr;
  }

  bool ChannelIsDestroyed() { return channel_ == nullptr; }

  CallCountingAndTracingNode* counter_and_tracer() {
    return &counter_and_tracer_;
  }

 protected:
  // provides view of target for child.
  char* target_view() { return target_.get(); }

 private:
  grpc_channel* channel_ = nullptr;
  UniquePtr<char> target_;
  CallCountingAndTracingNode counter_and_tracer_;
};

// Handles channelz bookkeeping for servers
// TODO(ncteisen): implement in subsequent PR.
class ServerNode : public BaseNode {
 public:
  ServerNode(size_t channel_tracer_max_nodes) : BaseNode(EntityType::kServer) {}
  ~ServerNode() override {}
};

// Handles channelz bookkeeping for sockets
// TODO(ncteisen): implement in subsequent PR.
class SocketNode : public BaseNode {
 public:
  SocketNode() : BaseNode(EntityType::kSocket) {}
  ~SocketNode() override {}
};

// Creation functions

typedef RefCountedPtr<ChannelNode> (*ChannelNodeCreationFunc)(grpc_channel*,
                                                              size_t, bool);

}  // namespace channelz
}  // namespace grpc_core

#endif /* GRPC_CORE_LIB_CHANNEL_CHANNELZ_H */
