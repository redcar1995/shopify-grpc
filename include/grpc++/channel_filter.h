/*
 *
 * Copyright 2016, Google Inc.
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

#ifndef GRPCXX_CHANNEL_FILTER_H
#define GRPCXX_CHANNEL_FILTER_H

#include <grpc/grpc.h>
#include <grpc/census.h>
#include <grpc++/impl/codegen/config.h>

#include <functional>
#include <vector>

#include "src/core/lib/channel/channel_stack.h"
#include "src/core/lib/security/context/security_context.h"
#include "src/core/lib/surface/channel_init.h"
#include "src/core/lib/transport/metadata_batch.h"

//
// An interface to define filters.
//
// To define a filter, implement a subclass of each of CallData and
// ChannelData.  Then register the filter using something like this:
//   RegisterChannelFilter<MyChannelDataSubclass, MyCallDataSubclass>(
//       "name-of-filter", GRPC_SERVER_CHANNEL, INT_MAX, nullptr);
//

namespace grpc {

// A C++ wrapper for the grpc_metadata_batch struct.
class MetadataBatch {
 public:
  explicit MetadataBatch(grpc_metadata_batch* batch) : batch_(batch) {}

  grpc_metadata_batch* batch() const { return batch_; }

  // Adds metadata and returns the newly allocated storage.
  // The caller takes ownership of the result, which must exist for the
  // lifetime of the gRPC call.
  grpc_linked_mdelem* AddMetadata(const string& key, const string& value);

  class const_iterator : public std::iterator<std::bidirectional_iterator_tag,
                                              const grpc_mdelem> {
   public:
    const grpc_mdelem& operator*() const { return *elem_->md; }
    const grpc_mdelem* operator->() const { return elem_->md; }

    const_iterator& operator++() {
      elem_ = elem_->next;
      return *this;
    }
    const_iterator operator++(int) {
      const_iterator tmp(*this);
      operator++();
      return tmp;
    }
    const_iterator& operator--() {
      elem_ = elem_->prev;
      return *this;
    }
    const_iterator operator--(int) {
      const_iterator tmp(*this);
      operator--();
      return tmp;
    }

    bool operator==(const const_iterator& other) const {
      return elem_ == other.elem_;
    }
    bool operator!=(const const_iterator& other) const {
      return elem_ != other.elem_;
    }

   private:
    friend class MetadataBatch;
    explicit const_iterator(grpc_linked_mdelem* elem) : elem_(elem) {}

    grpc_linked_mdelem* elem_;
  };

  const_iterator begin() const { return const_iterator(batch_->list.head); }
  const_iterator end() const { return const_iterator(nullptr); }

 private:
  grpc_metadata_batch* batch_;
};

// A C++ wrapper for the grpc_transport_op struct.
class TransportOp {
 public:
  explicit TransportOp(grpc_transport_op* op) : op_(op) {}

  grpc_transport_op* op() const { return op_; }

// FIXME: add a C++ wrapper for grpc_error?
  grpc_error* disconnect_with_error() const {
    return op_->disconnect_with_error;
  }
  bool send_goaway() const { return op_->send_goaway; }

  // TODO(roth): Add methods for additional fields as needed.

 private:
  grpc_transport_op* op_;  // Do not own.
};

// A C++ wrapper for the grpc_transport_stream_op struct.
class TransportStreamOp {
 public:
  explicit TransportStreamOp(grpc_transport_stream_op* op)
      : op_(op),
        send_initial_metadata_(op->send_initial_metadata),
        send_trailing_metadata_(op->send_trailing_metadata),
        recv_initial_metadata_(op->recv_initial_metadata),
        recv_trailing_metadata_(op->recv_trailing_metadata) {}

  grpc_transport_stream_op* op() const { return op_; }

  grpc_closure* on_complete() const { return op_->on_complete; }
  void set_on_complete(grpc_closure* closure) {
    op_->on_complete = closure;
  }

  MetadataBatch* send_initial_metadata() {
    return op_->send_initial_metadata == nullptr
           ? nullptr : &send_initial_metadata_;
  }
  MetadataBatch* send_trailing_metadata() {
    return op_->send_trailing_metadata == nullptr
           ? nullptr : &send_trailing_metadata_;
  }
  MetadataBatch* recv_initial_metadata() {
    return op_->recv_initial_metadata == nullptr
           ? nullptr : &recv_initial_metadata_;
  }
  MetadataBatch* recv_trailing_metadata() {
    return op_->recv_trailing_metadata == nullptr
           ? nullptr : &recv_trailing_metadata_;
  }

  uint32_t* send_initial_metadata_flags() const {
    return &op_->send_initial_metadata_flags;
  }

  grpc_closure* recv_initial_metadata_ready() const {
    return op_->recv_initial_metadata_ready;
  }
  void set_recv_initial_metadata_ready(grpc_closure* closure) {
    op_->recv_initial_metadata_ready = closure;
  }

  grpc_byte_stream* send_message() const { return op_->send_message; }
  void set_send_message(grpc_byte_stream* send_message) {
    op_->send_message = send_message;
  }

  // To be called only on clients and servers, respectively.
  grpc_client_security_context* client_security_context() const {
    return (grpc_client_security_context*)op_->context[
        GRPC_CONTEXT_SECURITY].value;
  }
  grpc_server_security_context* server_security_context() const {
    return (grpc_server_security_context*)op_->context[
        GRPC_CONTEXT_SECURITY].value;
  }

  census_context* get_census_context() const {
    return (census_context*)op_->context[GRPC_CONTEXT_TRACING].value;
  }

 private:
  grpc_transport_stream_op* op_;  // Do not own.
  MetadataBatch send_initial_metadata_;
  MetadataBatch send_trailing_metadata_;
  MetadataBatch recv_initial_metadata_;
  MetadataBatch recv_trailing_metadata_;
};

// Represents channel data.
class ChannelData {
 public:
  virtual ~ChannelData() {}

  const char* peer() const { return peer_; }

// FIXME: find a way to avoid passing elem into these methods
// (same for CallData below)
  virtual void StartTransportOp(grpc_exec_ctx *exec_ctx,
                                grpc_channel_element *elem,
                                TransportOp *op);

 protected:
  ChannelData(const grpc_channel_args &args, const char *peer) : peer_(peer) {}

 private:
  const char *peer_;  // Do not own.
};

// Represents call data.
class CallData {
 public:
  virtual ~CallData() {}

  virtual grpc_error* Init() { return GRPC_ERROR_NONE; }

  virtual void StartTransportStreamOp(grpc_exec_ctx *exec_ctx,
                                      grpc_call_element *elem,
                                      TransportStreamOp *op);

  virtual void SetPollsetOrPollsetSet(grpc_exec_ctx *exec_ctx,
                                      grpc_call_element *elem,
                                      grpc_polling_entity *pollent);

  virtual char *GetPeer(grpc_exec_ctx *exec_ctx, grpc_call_element *elem);

 protected:
  explicit CallData(const ChannelData &) {}
};

namespace internal {

// Defines static members for passing to C core.
template <typename ChannelDataType, typename CallDataType>
class ChannelFilter GRPC_FINAL {
 public:
  static const size_t channel_data_size = sizeof(ChannelDataType);

  static void InitChannelElement(grpc_exec_ctx *exec_ctx,
                                 grpc_channel_element *elem,
                                 grpc_channel_element_args *args) {
    const char* peer = args->optional_transport
                       ? grpc_transport_get_peer(exec_ctx,
                                                 args->optional_transport)
                       : nullptr;
    // Construct the object in the already-allocated memory.
    new (elem->channel_data) ChannelDataType(*args->channel_args, peer);
  }

  static void DestroyChannelElement(grpc_exec_ctx *exec_ctx,
                                    grpc_channel_element *elem) {
    reinterpret_cast<ChannelDataType *>(elem->channel_data)->~ChannelDataType();
  }

  static void StartTransportOp(grpc_exec_ctx *exec_ctx,
                               grpc_channel_element *elem,
                               grpc_transport_op *op) {
    ChannelDataType *channel_data = (ChannelDataType *)elem->channel_data;
    TransportOp op_wrapper(op);
    channel_data->StartTransportOp(exec_ctx, elem, &op_wrapper);
  }

  static const size_t call_data_size = sizeof(CallDataType);

  static grpc_error* InitCallElement(grpc_exec_ctx *exec_ctx,
                                     grpc_call_element *elem,
                                     grpc_call_element_args *args) {
    const ChannelDataType &channel_data =
        *(ChannelDataType *)elem->channel_data;
    // Construct the object in the already-allocated memory.
    CallDataType* call_data = new (elem->call_data) CallDataType(channel_data);
    return call_data->Init();
  }

  static void DestroyCallElement(grpc_exec_ctx *exec_ctx,
                                 grpc_call_element *elem,
                                 const grpc_call_stats *stats,
                                 void *and_free_memory) {
    reinterpret_cast<CallDataType *>(elem->call_data)->~CallDataType();
  }

  static void StartTransportStreamOp(grpc_exec_ctx *exec_ctx,
                                     grpc_call_element *elem,
                                     grpc_transport_stream_op *op) {
    CallDataType *call_data = (CallDataType *)elem->call_data;
    TransportStreamOp op_wrapper(op);
    call_data->StartTransportStreamOp(exec_ctx, elem, &op_wrapper);
  }

  static void SetPollsetOrPollsetSet(grpc_exec_ctx *exec_ctx,
                                     grpc_call_element *elem,
                                     grpc_polling_entity *pollent) {
    CallDataType *call_data = (CallDataType *)elem->call_data;
    call_data->SetPollsetOrPollsetSet(exec_ctx, elem, pollent);
  }

  static char *GetPeer(grpc_exec_ctx *exec_ctx, grpc_call_element *elem) {
    CallDataType *call_data = (CallDataType *)elem->call_data;
    return call_data->GetPeer(exec_ctx, elem);
  }
};

struct FilterRecord {
  grpc_channel_stack_type stack_type;
  int priority;
  std::function<bool(const grpc_channel_args &)> include_filter;
  grpc_channel_filter filter;
};
extern std::vector<FilterRecord> *channel_filters;

void ChannelFilterPluginInit();
void ChannelFilterPluginShutdown();

}  // namespace internal

// Registers a new filter.
// Must be called by only one thread at a time.
// The include_filter argument specifies a function that will be called
// to determine at run-time whether or not to add the filter.  If the
// value is nullptr, the filter will be added unconditionally.
template <typename ChannelDataType, typename CallDataType>
void RegisterChannelFilter(
    const char *name, grpc_channel_stack_type stack_type, int priority,
    std::function<bool(const grpc_channel_args &)> include_filter) {
  // If we haven't been called before, initialize channel_filters and
  // call grpc_register_plugin().
  if (internal::channel_filters == nullptr) {
    grpc_register_plugin(internal::ChannelFilterPluginInit,
                         internal::ChannelFilterPluginShutdown);
    internal::channel_filters = new std::vector<internal::FilterRecord>();
  }
  // Add an entry to channel_filters.  The filter will be added when the
  // C-core initialization code calls ChannelFilterPluginInit().
  typedef internal::ChannelFilter<ChannelDataType, CallDataType> FilterType;
  internal::FilterRecord filter_record = {
      stack_type,
      priority,
      include_filter,
      {FilterType::StartTransportStreamOp, FilterType::StartTransportOp,
       FilterType::call_data_size, FilterType::InitCallElement,
       FilterType::SetPollsetOrPollsetSet, FilterType::DestroyCallElement,
       FilterType::channel_data_size, FilterType::InitChannelElement,
       FilterType::DestroyChannelElement, FilterType::GetPeer, name}};
  internal::channel_filters->push_back(filter_record);
}

}  // namespace grpc

#endif  // GRPCXX_CHANNEL_FILTER_H
