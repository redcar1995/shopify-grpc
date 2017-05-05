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

#ifndef GRPCXX_IMPL_CODEGEN_SYNC_STREAM_H
#define GRPCXX_IMPL_CODEGEN_SYNC_STREAM_H

#include <grpc++/impl/codegen/call.h>
#include <grpc++/impl/codegen/channel_interface.h>
#include <grpc++/impl/codegen/client_context.h>
#include <grpc++/impl/codegen/completion_queue.h>
#include <grpc++/impl/codegen/core_codegen_interface.h>
#include <grpc++/impl/codegen/server_context.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/status.h>

namespace grpc {

/// Common interface for all synchronous client side streaming.
class ClientStreamingInterface {
 public:
  virtual ~ClientStreamingInterface() {}

  /// Block waiting until the stream finishes and a final status of the call is
  /// available.
  ///
  /// It is appropriate to call this method when both:
  ///   * the calling code (client-side) has no more message to send (this can be declared implicitly
  ///     by calling this method, or explicitly through an earlier call to \a
  ///     WritesDone.
  ///   * there are no more messages to be received from the server (which can
  ///     be known implicitly, or explicitly from an earlier call to \a Read that
  ///     returned "false"
  ///
  /// This function will return either:
  /// - when all incoming messages have been read and the server has returned
  ///   status.
  /// - when the server has returned a non-OK status.
  /// - OR when the call failed for some reason and the library generated a
  ///   status.
  ///
  /// Return values:
  ///   - \a Status contains the status code, message and details for the call
  ///   - the \a ClientContext associated with this call is updated with
  ///     possible trailing metadata sent from the server.
  virtual Status Finish() = 0;
};

/// Common interface for all synchronous server side streaming.
class ServerStreamingInterface {
 public:
  virtual ~ServerStreamingInterface() {}

  /// Block to send initial metadata to client.
  /// This call is optional, but if it is used, it cannot be used concurrently
  /// with or after the \a Finish method.
  ///
  /// The initial metadata that will be sent to the client will be
  /// taken from the \a ServerContext associated with the call.
  virtual void SendInitialMetadata() = 0;
};

/// An interface that yields a sequence of messages of type \a R.
template <class R>
class ReaderInterface {
 public:
  virtual ~ReaderInterface() {}

  /// Get an upper bound on the next message size available for reading on this stream.
  virtual bool NextMessageSize(uint32_t* sz) = 0;

  /// Block to read a message and parse to \a msg. Returns \a true on success.
  /// This is thread-safe with respect to \a Write or \WritesDone methods on
  /// the same stream. It should not be called concurrently with another \a
  /// Read on the same stream as the order of delivery will not be defined.
  ///
  /// \param[out] msg The read message.
  ///
  /// \return \a false when there will be no more incoming messages, either
  /// because the other side has called \a WritesDone() or the stream has failed
  /// (or been cancelled).
  virtual bool Read(R* msg) = 0;
};

/// An interface that can be fed a sequence of messages of type \a W.
template <class W>
class WriterInterface {
 public:
  virtual ~WriterInterface() {}

  /// Block to write \a msg to the stream with WriteOptions \a options.
  /// This is thread-safe with respect to \a Read
  ///
  /// \param msg The message to be written to the stream.
  /// \param options The WriteOptions affecting the write operation.
  ///
  /// \return \a true on success, \a false when the stream has been closed.
  virtual bool Write(const W& msg, WriteOptions options) = 0;

  /// Block to write \a msg to the stream with default write options.
  /// This is thread-safe with respect to \a Read
  ///
  /// \param msg The message to be written to the stream.
  ///
  /// \return \a true on success, \a false when the stream has been closed.
  inline bool Write(const W& msg) { return Write(msg, WriteOptions()); }

  /// Write \a msg and coalesce it with the writing of trailing metadata, using
  /// WriteOptions \a options.
  ///
  /// For client, WriteLast is equivalent of performing Write and WritesDone in
  /// a single step. \a msg and trailing metadata are coalesced and sent on wire
  /// by calling this function.
  /// For server, WriteLast buffers the \a msg. The writing of \a msg is held
  /// until the service handler returns, where \a msg and trailing metadata are
  /// coalesced and sent on wire. Note that WriteLast can only buffer \a msg up
  /// to the flow control window size. If \a msg size is larger than the window
  /// size, it will be sent on wire without buffering.
  ///
  /// \param[in] msg The message to be written to the stream.
  /// \param[in] options The WriteOptions to be used to write this message.
  void WriteLast(const W& msg, WriteOptions options) {
    Write(msg, options.set_last_message());
  }
};

/// Client-side interface for streaming reads of message of type \a R.
template <class R>
class ClientReaderInterface : public ClientStreamingInterface,
                              public ReaderInterface<R> {
 public:
  /// Block to wait for initial metadata from server. The received metadata
  /// can only be accessed after this call returns. Should only be called before
  /// the first read. Calling this method is optional, and if it is not called
  /// the metadata will be available in ClientContext after the first read.
  virtual void WaitForInitialMetadata() = 0;
};

/// Synchronous (blocking) client-side API for doing server-streaming RPCs, where the
/// stream of messages coming from the server has messages of type \a R.
template <class R>
class ClientReader final : public ClientReaderInterface<R> {
 public:
  /// Block to create a stream and write the initial metadata and \a request out.
  /// Note that \a context will be used to fill in custom initial metadata
  /// used to send to the server when starting the call.
  template <class W>
  ClientReader(ChannelInterface* channel, const RpcMethod& method,
               ClientContext* context, const W& request)
      : context_(context),
        cq_(grpc_completion_queue_attributes{
            GRPC_CQ_CURRENT_VERSION, GRPC_CQ_PLUCK,
            GRPC_CQ_DEFAULT_POLLING}),  // Pluckable cq
        call_(channel->CreateCall(method, context, &cq_)) {
    CallOpSet<CallOpSendInitialMetadata, CallOpSendMessage,
              CallOpClientSendClose>
        ops;
    ops.SendInitialMetadata(context->send_initial_metadata_,
                            context->initial_metadata_flags());
    // TODO(ctiller): don't assert
    GPR_CODEGEN_ASSERT(ops.SendMessage(request).ok());
    ops.ClientSendClose();
    call_.PerformOps(&ops);
    cq_.Pluck(&ops);
  }

  /// See the \a ClientStreamingInterface.WaitForInitialMetadata method for
  /// semantics.
  ///
  //  Side effect:
  ///   Once complete, the initial metadata read from
  ///   the server will be accessable through the \a ClientContext used to
  ///   construct this object.
  void WaitForInitialMetadata() override {
    GPR_CODEGEN_ASSERT(!context_->initial_metadata_received_);

    CallOpSet<CallOpRecvInitialMetadata> ops;
    ops.RecvInitialMetadata(context_);
    call_.PerformOps(&ops);
    cq_.Pluck(&ops);  /// status ignored
  }

  /// See the \a ReaderInterface.NextMessageSize for semantics.
  bool NextMessageSize(uint32_t* sz) override {
    *sz = call_.max_receive_message_size();
    return true;
  }

  /// See the \a ReaderInterface.Read method for semantics.
  /// Side effect:
  ///   this also receives initial metadata from the server, if not
  ///   already received (if initial metadata is received, it can be then accessed
  ///   through the \a ClientContext associated with this call).
  bool Read(R* msg) override {
    CallOpSet<CallOpRecvInitialMetadata, CallOpRecvMessage<R>> ops;
    if (!context_->initial_metadata_received_) {
      ops.RecvInitialMetadata(context_);
    }
    ops.RecvMessage(msg);
    call_.PerformOps(&ops);
    return cq_.Pluck(&ops) && ops.got_message;
  }

  /// See the \a ClientStreamingInterface.Finish method for semantics.
  ///
  /// Side effect:
  ///   - the \a ClientContext associated with this call is updated with
  ///     possible metadata received from the server.
  Status Finish() override {
    CallOpSet<CallOpClientRecvStatus> ops;
    Status status;
    ops.ClientRecvStatus(context_, &status);
    call_.PerformOps(&ops);
    GPR_CODEGEN_ASSERT(cq_.Pluck(&ops));
    return status;
  }

 private:
  ClientContext* context_;
  CompletionQueue cq_;
  Call call_;
};

/// Client-side interface for streaming writes of message type \a W.
template <class W>
class ClientWriterInterface : public ClientStreamingInterface,
                              public WriterInterface<W> {
 public:
  /// Half close writing from the client. (signal that the stream of messages
  /// coming from the clinet is complete).
  /// Blocks until currently-pending writes are completed.
  /// Thread safe with respect to \a Read operations only
  ///
  /// \return Whether the writes were successful.
  virtual bool WritesDone() = 0;
};

/// Synchronous (blocking) client-side API for doing client-streaming RPCs,
/// where the outgoing message stream coming from the client has messages of type \a W.
template <class W>
class ClientWriter : public ClientWriterInterface<W> {
 public:
  /// Block to create a stream (i.e. send request headers and other initial metadata to the server).
  /// Note that \a context will be used to fill in custom initial metadata.
  /// \a response will be filled in with the single expected response
  /// message from the server upon a successful call to the \a Finish
  /// method of this instance.
  template <class R>
  ClientWriter(ChannelInterface* channel, const RpcMethod& method,
               ClientContext* context, R* response)
      : context_(context),
        cq_(grpc_completion_queue_attributes{
            GRPC_CQ_CURRENT_VERSION, GRPC_CQ_PLUCK,
            GRPC_CQ_DEFAULT_POLLING}),  // Pluckable cq
        call_(channel->CreateCall(method, context, &cq_)) {
    finish_ops_.RecvMessage(response);
    finish_ops_.AllowNoMessage();

    if (!context_->initial_metadata_corked_) {
      CallOpSet<CallOpSendInitialMetadata> ops;
      ops.SendInitialMetadata(context->send_initial_metadata_,
                              context->initial_metadata_flags());
      call_.PerformOps(&ops);
      cq_.Pluck(&ops);
    }
  }

  /// See the \a ClientStreamingInterface.WaitForInitialMetadata method for
  /// semantics.
  ///
  //  Side effect:
  ///   Once complete, the initial metadata read from
  ///   the server will be accessable through the \a ClientContext used to
  ///   construct this object.
  void WaitForInitialMetadata() {
    GPR_CODEGEN_ASSERT(!context_->initial_metadata_received_);

    CallOpSet<CallOpRecvInitialMetadata> ops;
    ops.RecvInitialMetadata(context_);
    call_.PerformOps(&ops);
    cq_.Pluck(&ops);  // status ignored
  }

  /// See the WriterInterface.Write(const W& msg, WriteOptions options) method
  /// for semantics.
  ///
  /// Side effect:
  ///   Also sends initial metadata if not already sent (using the \a ClientContext
  ///   associated with this call).
  using WriterInterface<W>::Write;
  bool Write(const W& msg, WriteOptions options) override {
    CallOpSet<CallOpSendInitialMetadata, CallOpSendMessage,
              CallOpClientSendClose>
        ops;

    if (options.is_last_message()) {
      options.set_buffer_hint();
      ops.ClientSendClose();
    }
    if (context_->initial_metadata_corked_) {
      ops.SendInitialMetadata(context_->send_initial_metadata_,
                              context_->initial_metadata_flags());
      context_->set_initial_metadata_corked(false);
    }
    if (!ops.SendMessage(msg, options).ok()) {
      return false;
    }

    call_.PerformOps(&ops);
    return cq_.Pluck(&ops);
  }

  /// See the \a ClientWriterInterface.WritesDone method for semantics.
  bool WritesDone() override {
    CallOpSet<CallOpClientSendClose> ops;
    ops.ClientSendClose();
    call_.PerformOps(&ops);
    return cq_.Pluck(&ops);
  }

  /// See the ClientStreamingInterface.Finish method for semantics.
  /// Side effects:
  ///   - Also receives initial metadata if not already received.
  ///   - Attempts to fill in the \a response parameter passed to the constructor
  ///     of this instance with the response message from the server.
  Status Finish() override {
    Status status;
    if (!context_->initial_metadata_received_) {
      finish_ops_.RecvInitialMetadata(context_);
    }
    finish_ops_.ClientRecvStatus(context_, &status);
    call_.PerformOps(&finish_ops_);
    GPR_CODEGEN_ASSERT(cq_.Pluck(&finish_ops_));
    return status;
  }

 private:
  ClientContext* context_;
  CallOpSet<CallOpRecvInitialMetadata, CallOpGenericRecvMessage,
            CallOpClientRecvStatus>
      finish_ops_;
  CompletionQueue cq_;
  Call call_;
};

/// Client-side interface for bi-directional streaming with
/// client-to-server stream messages of type \a W and
/// server-to-client stream messages of type \a R.
template <class W, class R>
class ClientReaderWriterInterface : public ClientStreamingInterface,
                                    public WriterInterface<W>,
                                    public ReaderInterface<R> {
 public:
  /// Block to wait for initial metadata from server. The received metadata
  /// can only be accessed after this call returns. Should only be called before
  /// the first read. Calling this method is optional, and if it is not called
  /// the metadata will be available in ClientContext after the first read.
  virtual void WaitForInitialMetadata() = 0;

  /// Half close writing from the client. (signal that the stream of messages
  /// coming from the clinet is complete).
  /// Blocks until currently-pending writes are completed.
  /// Thread-safe with respect to \a Read
  ///
  /// \return Whether the writes were successful.
  virtual bool WritesDone() = 0;
};

/// Synchronous (blocking) client-side API for bi-directional streaming RPCs, where the
/// outgoing message stream coming from the client has messages of type \a W,
/// and the incoming messages stream coming from the server has messages of type
/// \a R.
template <class W, class R>
class ClientReaderWriter final : public ClientReaderWriterInterface<W, R> {
 public:
  /// Block to create a stream and write the initial metadata and \a request out.
  /// Note that \a context will be used to fill in custom initial metadata
  /// used to send to the server when starting the call.
  ClientReaderWriter(ChannelInterface* channel, const RpcMethod& method,
                     ClientContext* context)
      : context_(context),
        cq_(grpc_completion_queue_attributes{
            GRPC_CQ_CURRENT_VERSION, GRPC_CQ_PLUCK,
            GRPC_CQ_DEFAULT_POLLING}),  // Pluckable cq
        call_(channel->CreateCall(method, context, &cq_)) {
    if (!context_->initial_metadata_corked_) {
      CallOpSet<CallOpSendInitialMetadata> ops;
      ops.SendInitialMetadata(context->send_initial_metadata_,
                              context->initial_metadata_flags());
      call_.PerformOps(&ops);
      cq_.Pluck(&ops);
    }
  }

  /// Block waiting to read initial metadata from the server.
  /// This call is optional, but if it is used, it cannot be used concurrently
  /// with or after the \a Finish method.
  ///
  /// Once complete, the initial metadata read from
  /// the server will be accessable through the \a ClientContext used to
  /// construct this object.
  void WaitForInitialMetadata() override {
    GPR_CODEGEN_ASSERT(!context_->initial_metadata_received_);

    CallOpSet<CallOpRecvInitialMetadata> ops;
    ops.RecvInitialMetadata(context_);
    call_.PerformOps(&ops);
    cq_.Pluck(&ops);  // status ignored
  }

  bool NextMessageSize(uint32_t* sz) override {
    *sz = call_.max_receive_message_size();
    return true;
  }

  /// See the \a ReaderInterface.Read method for semantics.
  /// Side effect:
  ///   Also receives initial metadata if not already received (updates the \a
  ///   ClientContext associated with this call in that case).
  bool Read(R* msg) override {
    CallOpSet<CallOpRecvInitialMetadata, CallOpRecvMessage<R>> ops;
    if (!context_->initial_metadata_received_) {
      ops.RecvInitialMetadata(context_);
    }
    ops.RecvMessage(msg);
    call_.PerformOps(&ops);
    return cq_.Pluck(&ops) && ops.got_message;
  }

  /// See the \a WriterInterface.Write method for semantics.
  ///
  /// Side effect:
  ///   Also sends initial metadata if not already sent (using the
  ///   \a ClientContext associated with this call to fill in values).
  using WriterInterface<W>::Write;
  bool Write(const W& msg, WriteOptions options) override {
    CallOpSet<CallOpSendInitialMetadata, CallOpSendMessage,
              CallOpClientSendClose>
        ops;

    if (options.is_last_message()) {
      options.set_buffer_hint();
      ops.ClientSendClose();
    }
    if (context_->initial_metadata_corked_) {
      ops.SendInitialMetadata(context_->send_initial_metadata_,
                              context_->initial_metadata_flags());
      context_->set_initial_metadata_corked(false);
    }
    if (!ops.SendMessage(msg, options).ok()) {
      return false;
    }

    call_.PerformOps(&ops);
    return cq_.Pluck(&ops);
  }

  /// See the ClientWriterInterface.WritesDone method for semantics.
  bool WritesDone() override {
    CallOpSet<CallOpClientSendClose> ops;
    ops.ClientSendClose();
    call_.PerformOps(&ops);
    return cq_.Pluck(&ops);
  }

  /// See the ClientStreamingInterface.Finish method for semantics.
  ///
  /// Side effect:
  ///   - the \a ClientContext associated with this call is updated with
  ///     possible trailing metadata sent from the server.
  Status Finish() override {
    CallOpSet<CallOpRecvInitialMetadata, CallOpClientRecvStatus> ops;
    if (!context_->initial_metadata_received_) {
      ops.RecvInitialMetadata(context_);
    }
    Status status;
    ops.ClientRecvStatus(context_, &status);
    call_.PerformOps(&ops);
    GPR_CODEGEN_ASSERT(cq_.Pluck(&ops));
    return status;
  }

 private:
  ClientContext* context_;
  CompletionQueue cq_;
  Call call_;
};

/// Server-side interface for streaming reads of message of type \a R.
template <class R>
class ServerReaderInterface : public ServerStreamingInterface,
                              public ReaderInterface<R> {};

/// Synchronous (blocking) server-side API for doing client-streaming RPCs,
/// where the incoming message stream coming from the client has messages of
/// type \a R.
template <class R>
class ServerReader final : public ServerReaderInterface<R> {
 public:
  ServerReader(Call* call, ServerContext* ctx) : call_(call), ctx_(ctx) {}

  /// See the \a ServerStreamingInterface.SendInitialMetadata method
  /// for semantics.
  /// Note that initial metadata will be affected by the
  /// \a ServerContext associated with this call.
  void SendInitialMetadata() override {
    GPR_CODEGEN_ASSERT(!ctx_->sent_initial_metadata_);

    CallOpSet<CallOpSendInitialMetadata> ops;
    ops.SendInitialMetadata(ctx_->initial_metadata_,
                            ctx_->initial_metadata_flags());
    if (ctx_->compression_level_set()) {
      ops.set_compression_level(ctx_->compression_level());
    }
    ctx_->sent_initial_metadata_ = true;
    call_->PerformOps(&ops);
    call_->cq()->Pluck(&ops);
  }

  /// See the \a ReaderInterface.NextMessageSize method.
  bool NextMessageSize(uint32_t* sz) override {
    *sz = call_->max_receive_message_size();
    return true;
  }

  /// See the \a ReaderInterface.Read method for semantics.
  bool Read(R* msg) override {
    CallOpSet<CallOpRecvMessage<R>> ops;
    ops.RecvMessage(msg);
    call_->PerformOps(&ops);
    return call_->cq()->Pluck(&ops) && ops.got_message;
  }

 private:
  Call* const call_;
  ServerContext* const ctx_;
};

/// Server-side interface for streaming writes of message of type \a W.
template <class W>
class ServerWriterInterface : public ServerStreamingInterface,
                              public WriterInterface<W> {};

/// Synchronous (blocking) server-side API for doing for doing a
/// server-streaming RPCs, where the outgoing message stream coming from the
/// server has messages of type \a W.
template <class W>
class ServerWriter final : public ServerWriterInterface<W> {
 public:
  ServerWriter(Call* call, ServerContext* ctx) : call_(call), ctx_(ctx) {}

  /// See the \a ServerStreamingInterface.SendInitialMetadata method
  /// for semantics.
  /// Note that initial metadata will be affected by the
  /// \a ServerContext associated with this call.
  void SendInitialMetadata() override {
    GPR_CODEGEN_ASSERT(!ctx_->sent_initial_metadata_);

    CallOpSet<CallOpSendInitialMetadata> ops;
    ops.SendInitialMetadata(ctx_->initial_metadata_,
                            ctx_->initial_metadata_flags());
    if (ctx_->compression_level_set()) {
      ops.set_compression_level(ctx_->compression_level());
    }
    ctx_->sent_initial_metadata_ = true;
    call_->PerformOps(&ops);
    call_->cq()->Pluck(&ops);
  }

  /// See the \a WriterInterface.Write method for semantics.
  ///
  /// Side effect:
  ///   Also sends initial metadata if not already sent (using the
  ///   \a ClientContext associated with this call to fill in values).
  using WriterInterface<W>::Write;
  bool Write(const W& msg, WriteOptions options) override {
    if (options.is_last_message()) {
      options.set_buffer_hint();
    }
    CallOpSet<CallOpSendInitialMetadata, CallOpSendMessage> ops;
    if (!ops.SendMessage(msg, options).ok()) {
      return false;
    }
    if (!ctx_->sent_initial_metadata_) {
      ops.SendInitialMetadata(ctx_->initial_metadata_,
                              ctx_->initial_metadata_flags());
      if (ctx_->compression_level_set()) {
        ops.set_compression_level(ctx_->compression_level());
      }
      ctx_->sent_initial_metadata_ = true;
    }
    call_->PerformOps(&ops);
    return call_->cq()->Pluck(&ops);
  }

 private:
  Call* const call_;
  ServerContext* const ctx_;
};

/// Server-side interface for bi-directional streaming.
template <class W, class R>
class ServerReaderWriterInterface : public ServerStreamingInterface,
                                    public WriterInterface<W>,
                                    public ReaderInterface<R> {};

/// Actual implementation of bi-directional streaming
namespace internal {
template <class W, class R>
class ServerReaderWriterBody final {
 public:
  ServerReaderWriterBody(Call* call, ServerContext* ctx)
      : call_(call), ctx_(ctx) {}

  void SendInitialMetadata() {
    GPR_CODEGEN_ASSERT(!ctx_->sent_initial_metadata_);

    CallOpSet<CallOpSendInitialMetadata> ops;
    ops.SendInitialMetadata(ctx_->initial_metadata_,
                            ctx_->initial_metadata_flags());
    if (ctx_->compression_level_set()) {
      ops.set_compression_level(ctx_->compression_level());
    }
    ctx_->sent_initial_metadata_ = true;
    call_->PerformOps(&ops);
    call_->cq()->Pluck(&ops);
  }

  bool NextMessageSize(uint32_t* sz) {
    *sz = call_->max_receive_message_size();
    return true;
  }

  bool Read(R* msg) {
    CallOpSet<CallOpRecvMessage<R>> ops;
    ops.RecvMessage(msg);
    call_->PerformOps(&ops);
    return call_->cq()->Pluck(&ops) && ops.got_message;
  }

  bool Write(const W& msg, WriteOptions options) {
    if (options.is_last_message()) {
      options.set_buffer_hint();
    }
    CallOpSet<CallOpSendInitialMetadata, CallOpSendMessage> ops;
    if (!ops.SendMessage(msg, options).ok()) {
      return false;
    }
    if (!ctx_->sent_initial_metadata_) {
      ops.SendInitialMetadata(ctx_->initial_metadata_,
                              ctx_->initial_metadata_flags());
      if (ctx_->compression_level_set()) {
        ops.set_compression_level(ctx_->compression_level());
      }
      ctx_->sent_initial_metadata_ = true;
    }
    call_->PerformOps(&ops);
    return call_->cq()->Pluck(&ops);
  }

 private:
  Call* const call_;
  ServerContext* const ctx_;
};
}  // namespace internal

/// Synchronous (blocking) server-side API for a bidirectional
/// streaming call, where the incoming message stream coming from the client has messages of
/// type \a R, and the outgoing message streaming coming from the server has messages of type \a W.
template <class W, class R>
class ServerReaderWriter final : public ServerReaderWriterInterface<W, R> {
 public:
  ServerReaderWriter(Call* call, ServerContext* ctx) : body_(call, ctx) {}

  /// See the \a ServerStreamingInterface.SendInitialMetadata method
  /// for semantics.
  /// Note that initial metadata will be affected by the
  /// \a ServerContext associated with this call.
  void SendInitialMetadata() override { body_.SendInitialMetadata(); }

  /// See the \a ReaderInterface.NextMessageSize method for semantics
  bool NextMessageSize(uint32_t* sz) override {
    return body_.NextMessageSize(sz);
  }

  /// See the \a ReaderInterface.Read method for semantics
  bool Read(R* msg) override { return body_.Read(msg); }

  /// See the \a WriterInterface.Write(const W& msg, WriteOptions options) method for semantics.
  /// Side effect:
  ///   Also sends initial metadata if not already sent (using the \a
  ///   ServerContext associated with this call).
  using WriterInterface<W>::Write;
  bool Write(const W& msg, WriteOptions options) override {
    return body_.Write(msg, options);
  }

 private:
  internal::ServerReaderWriterBody<W, R> body_;
};

/// A class to represent a flow-controlled unary call. This is something
/// of a hybrid between conventional unary and streaming. This is invoked
/// through a unary call on the client side, but the server responds to it
/// as though it were a single-ping-pong streaming call. The server can use
/// the \a NextMessageSize method to determine an upper-bound on the size of
/// the message.
/// A key difference relative to streaming: ServerUnaryStreamer
/// must have exactly 1 Read and exactly 1 Write, in that order, to function
/// correctly. Otherwise, the RPC is in error.
template <class RequestType, class ResponseType>
class ServerUnaryStreamer final
    : public ServerReaderWriterInterface<ResponseType, RequestType> {
 public:
  ServerUnaryStreamer(Call* call, ServerContext* ctx)
      : body_(call, ctx), read_done_(false), write_done_(false) {}

  /// Block to send initial metadata to client.
  /// Implicit input parameter:
  ///    - the \a ServerContext associated with this call will be used for
  ///      sending initial metadata.
  void SendInitialMetadata() override { body_.SendInitialMetadata(); }

  /// Get an upper bound on the request message size from the client.
  bool NextMessageSize(uint32_t* sz) override {
    return body_.NextMessageSize(sz);
  }

  /// Read a message of type \a R into \a msg. Completion will be notified by \a
  /// tag on the associated completion queue.
  /// This is thread-safe with respect to \a Write or \a WritesDone methods. It
  /// should not be called concurrently with other streaming APIs
  /// on the same stream. It is not meaningful to call it concurrently
  /// with another \a Read on the same stream since reads on the same stream
  /// are delivered in order.
  ///
  /// \param[out] msg Where to eventually store the read message.
  /// \param[in] tag The tag identifying the operation.
  bool Read(RequestType* request) override {
    if (read_done_) {
      return false;
    }
    read_done_ = true;
    return body_.Read(request);
  }

  /// Block to write \a msg to the stream with WriteOptions \a options.
  /// This is thread-safe with respect to \a Read
  ///
  /// \param msg The message to be written to the stream.
  /// \param options The WriteOptions affecting the write operation.
  ///
  /// \return \a true on success, \a false when the stream has been closed.
  using WriterInterface<ResponseType>::Write;
  bool Write(const ResponseType& response, WriteOptions options) override {
    if (write_done_ || !read_done_) {
      return false;
    }
    write_done_ = true;
    return body_.Write(response, options);
  }

 private:
  internal::ServerReaderWriterBody<ResponseType, RequestType> body_;
  bool read_done_;
  bool write_done_;
};

/// A class to represent a flow-controlled server-side streaming call.
/// This is something of a hybrid between server-side and bidi streaming.
/// This is invoked through a server-side streaming call on the client side,
/// but the server responds to it as though it were a bidi streaming call that
/// must first have exactly 1 Read and then any number of Writes.
template <class RequestType, class ResponseType>
class ServerSplitStreamer final
    : public ServerReaderWriterInterface<ResponseType, RequestType> {
 public:
  ServerSplitStreamer(Call* call, ServerContext* ctx)
      : body_(call, ctx), read_done_(false) {}

  /// Block to send initial metadata to client.
  /// Implicit input parameter:
  ///    - the \a ServerContext associated with this call will be used for
  ///      sending initial metadata.
  void SendInitialMetadata() override { body_.SendInitialMetadata(); }

  /// Get an upper bound on the request message size from the client.
  bool NextMessageSize(uint32_t* sz) override {
    return body_.NextMessageSize(sz);
  }

  /// Read a message of type \a R into \a msg. Completion will be notified by \a
  /// tag on the associated completion queue.
  /// This is thread-safe with respect to \a Write or \a WritesDone methods. It
  /// should not be called concurrently with other streaming APIs
  /// on the same stream. It is not meaningful to call it concurrently
  /// with another \a Read on the same stream since reads on the same stream
  /// are delivered in order.
  ///
  /// \param[out] msg Where to eventually store the read message.
  /// \param[in] tag The tag identifying the operation.
  bool Read(RequestType* request) override {
    if (read_done_) {
      return false;
    }
    read_done_ = true;
    return body_.Read(request);
  }

  /// Block to write \a msg to the stream with WriteOptions \a options.
  /// This is thread-safe with respect to \a Read
  ///
  /// \param msg The message to be written to the stream.
  /// \param options The WriteOptions affecting the write operation.
  ///
  /// \return \a true on success, \a false when the stream has been closed.
  using WriterInterface<ResponseType>::Write;
  bool Write(const ResponseType& response, WriteOptions options) override {
    return read_done_ && body_.Write(response, options);
  }

 private:
  internal::ServerReaderWriterBody<ResponseType, RequestType> body_;
  bool read_done_;
};

}  // namespace grpc

#endif  // GRPCXX_IMPL_CODEGEN_SYNC_STREAM_H
