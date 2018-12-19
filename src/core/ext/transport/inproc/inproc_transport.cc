/*
 *
 * Copyright 2017 gRPC authors.
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

#include <grpc/support/port_platform.h>

#include <grpc/support/alloc.h>
#include <grpc/support/string_util.h>
#include <grpc/support/sync.h>
#include <grpc/support/time.h>
#include <string.h>
#include "src/core/ext/transport/inproc/inproc_transport.h"
#include "src/core/lib/channel/channel_args.h"
#include "src/core/lib/gprpp/manual_constructor.h"
#include "src/core/lib/slice/slice_internal.h"
#include "src/core/lib/surface/api_trace.h"
#include "src/core/lib/surface/channel.h"
#include "src/core/lib/surface/channel_stack_type.h"
#include "src/core/lib/surface/server.h"
#include "src/core/lib/transport/connectivity_state.h"
#include "src/core/lib/transport/error_utils.h"
#include "src/core/lib/transport/transport_impl.h"

#define INPROC_LOG(...)                                    \
  do {                                                     \
    if (grpc_inproc_trace.enabled()) gpr_log(__VA_ARGS__); \
  } while (0)

namespace {
grpc_slice g_empty_slice;
grpc_slice g_fake_path_key;
grpc_slice g_fake_path_value;
grpc_slice g_fake_auth_key;
grpc_slice g_fake_auth_value;

struct inproc_stream;
bool cancel_stream_locked(inproc_stream* s, grpc_error* error);
void op_state_machine(void* arg, grpc_error* error);
void log_metadata(const grpc_metadata_batch* md_batch, bool is_client,
                  bool is_initial);
grpc_error* fill_in_metadata(inproc_stream* s,
                             const grpc_metadata_batch* metadata,
                             uint32_t flags, grpc_metadata_batch* out_md,
                             uint32_t* outflags, bool* markfilled);

struct shared_mu {
  shared_mu() {
    // Share one lock between both sides since both sides get affected
    gpr_mu_init(&mu);
    gpr_ref_init(&refs, 2);
  }

  gpr_mu mu;
  gpr_refcount refs;
};

struct inproc_transport {
  inproc_transport(const grpc_transport_vtable* vtable, shared_mu* mu,
                   bool is_client)
      : mu(mu), is_client(is_client) {
    base.vtable = vtable;
    // Start each side of transport with 2 refs since they each have a ref
    // to the other
    gpr_ref_init(&refs, 2);
    grpc_connectivity_state_init(&connectivity, GRPC_CHANNEL_READY,
                                 is_client ? "inproc_client" : "inproc_server");
  }

  ~inproc_transport() {
    grpc_connectivity_state_destroy(&connectivity);
    if (gpr_unref(&mu->refs)) {
      gpr_free(mu);
    }
  }

  void ref() {
    INPROC_LOG(GPR_INFO, "ref_transport %p", this);
    gpr_ref(&refs);
  }

  void unref() {
    INPROC_LOG(GPR_INFO, "unref_transport %p", this);
    if (!gpr_unref(&refs)) {
      return;
    }
    INPROC_LOG(GPR_INFO, "really_destroy_transport %p", this);
    this->~inproc_transport();
    gpr_free(this);
  }

  grpc_transport base;
  shared_mu* mu;
  gpr_refcount refs;
  bool is_client;
  grpc_connectivity_state_tracker connectivity;
  void (*accept_stream_cb)(void* user_data, grpc_transport* transport,
                           const void* server_data);
  void* accept_stream_data;
  bool is_closed = false;
  struct inproc_transport* other_side;
  struct inproc_stream* stream_list = nullptr;
};

struct inproc_stream {
  inproc_stream(inproc_transport* t, grpc_stream_refcount* refcount,
                const void* server_data, gpr_arena* arena)
      : t(t), refs(refcount), arena(arena) {
    // Ref this stream right now for ctor and list.
    ref("inproc_init_stream:init");
    ref("inproc_init_stream:list");

    grpc_metadata_batch_init(&to_read_initial_md);
    grpc_metadata_batch_init(&to_read_trailing_md);
    GRPC_CLOSURE_INIT(&op_closure, op_state_machine, this,
                      grpc_schedule_on_exec_ctx);
    grpc_metadata_batch_init(&write_buffer_initial_md);
    grpc_metadata_batch_init(&write_buffer_trailing_md);

    stream_list_prev = nullptr;
    gpr_mu_lock(&t->mu->mu);
    stream_list_next = t->stream_list;
    if (t->stream_list) {
      t->stream_list->stream_list_prev = this;
    }
    t->stream_list = this;
    gpr_mu_unlock(&t->mu->mu);

    if (!server_data) {
      t->ref();
      inproc_transport* st = t->other_side;
      st->ref();
      other_side = nullptr;  // will get filled in soon
      // Pass the client-side stream address to the server-side for a ref
      ref("inproc_init_stream:clt");  // ref it now on behalf of server
                                      // side to avoid destruction
      INPROC_LOG(GPR_INFO, "calling accept stream cb %p %p",
                 st->accept_stream_cb, st->accept_stream_data);
      (*st->accept_stream_cb)(st->accept_stream_data, &st->base, (void*)this);
    } else {
      // This is the server-side and is being called through accept_stream_cb
      inproc_stream* cs = (inproc_stream*)server_data;
      other_side = cs;
      // Ref the server-side stream on behalf of the client now
      ref("inproc_init_stream:srv");

      // Now we are about to affect the other side, so lock the transport
      // to make sure that it doesn't get destroyed
      gpr_mu_lock(&t->mu->mu);
      cs->other_side = this;
      // Now transfer from the other side's write_buffer if any to the to_read
      // buffer
      if (cs->write_buffer_initial_md_filled) {
        fill_in_metadata(this, &cs->write_buffer_initial_md,
                         cs->write_buffer_initial_md_flags, &to_read_initial_md,
                         &to_read_initial_md_flags, &to_read_initial_md_filled);
        deadline = GPR_MIN(deadline, cs->write_buffer_deadline);
        grpc_metadata_batch_clear(&cs->write_buffer_initial_md);
        cs->write_buffer_initial_md_filled = false;
      }
      if (cs->write_buffer_trailing_md_filled) {
        fill_in_metadata(this, &cs->write_buffer_trailing_md, 0,
                         &to_read_trailing_md, nullptr,
                         &to_read_trailing_md_filled);
        grpc_metadata_batch_clear(&cs->write_buffer_trailing_md);
        cs->write_buffer_trailing_md_filled = false;
      }
      if (cs->write_buffer_cancel_error != GRPC_ERROR_NONE) {
        cancel_other_error = cs->write_buffer_cancel_error;
        cs->write_buffer_cancel_error = GRPC_ERROR_NONE;
      }

      gpr_mu_unlock(&t->mu->mu);
    }
  }

  ~inproc_stream() {
    GRPC_ERROR_UNREF(write_buffer_cancel_error);
    GRPC_ERROR_UNREF(cancel_self_error);
    GRPC_ERROR_UNREF(cancel_other_error);

    if (recv_inited) {
      grpc_slice_buffer_destroy_internal(&recv_message);
    }

    t->unref();

    if (closure_at_destroy) {
      GRPC_CLOSURE_SCHED(closure_at_destroy, GRPC_ERROR_NONE);
    }
  }

#ifndef NDEBUG
#define STREAM_REF(refs, reason) grpc_stream_ref(refs, reason)
#define STREAM_UNREF(refs, reason) grpc_stream_unref(refs, reason)
#else
#define STREAM_REF(refs, reason) grpc_stream_ref(refs)
#define STREAM_UNREF(refs, reason) grpc_stream_unref(refs)
#endif
  void ref(const char* reason) {
    INPROC_LOG(GPR_INFO, "ref_stream %p %s", this, reason);
    STREAM_REF(refs, reason);
  }

  void unref(const char* reason) {
    INPROC_LOG(GPR_INFO, "unref_stream %p %s", this, reason);
    STREAM_UNREF(refs, reason);
  }
#undef STREAM_REF
#undef STREAM_UNREF

  inproc_transport* t;
  grpc_metadata_batch to_read_initial_md;
  uint32_t to_read_initial_md_flags = 0;
  bool to_read_initial_md_filled = false;
  grpc_metadata_batch to_read_trailing_md;
  bool to_read_trailing_md_filled = false;
  bool ops_needed = false;
  bool op_closure_scheduled = false;
  grpc_closure op_closure;
  // Write buffer used only during gap at init time when client-side
  // stream is set up but server side stream is not yet set up
  grpc_metadata_batch write_buffer_initial_md;
  bool write_buffer_initial_md_filled = false;
  uint32_t write_buffer_initial_md_flags = 0;
  grpc_millis write_buffer_deadline = GRPC_MILLIS_INF_FUTURE;
  grpc_metadata_batch write_buffer_trailing_md;
  bool write_buffer_trailing_md_filled = false;
  grpc_error* write_buffer_cancel_error = GRPC_ERROR_NONE;

  struct inproc_stream* other_side;
  bool other_side_closed = false;               // won't talk anymore
  bool write_buffer_other_side_closed = false;  // on hold
  grpc_stream_refcount* refs;
  grpc_closure* closure_at_destroy = nullptr;

  gpr_arena* arena;

  grpc_transport_stream_op_batch* send_message_op = nullptr;
  grpc_transport_stream_op_batch* send_trailing_md_op = nullptr;
  grpc_transport_stream_op_batch* recv_initial_md_op = nullptr;
  grpc_transport_stream_op_batch* recv_message_op = nullptr;
  grpc_transport_stream_op_batch* recv_trailing_md_op = nullptr;

  grpc_slice_buffer recv_message;
  grpc_core::ManualConstructor<grpc_core::SliceBufferByteStream> recv_stream;
  bool recv_inited = false;

  bool initial_md_sent = false;
  bool trailing_md_sent = false;
  bool initial_md_recvd = false;
  bool trailing_md_recvd = false;

  bool closed = false;

  grpc_error* cancel_self_error = GRPC_ERROR_NONE;
  grpc_error* cancel_other_error = GRPC_ERROR_NONE;

  grpc_millis deadline = GRPC_MILLIS_INF_FUTURE;

  bool listed = true;
  struct inproc_stream* stream_list_prev;
  struct inproc_stream* stream_list_next;
};

void log_metadata(const grpc_metadata_batch* md_batch, bool is_client,
                  bool is_initial) {
  for (grpc_linked_mdelem* md = md_batch->list.head; md != nullptr;
       md = md->next) {
    char* key = grpc_slice_to_c_string(GRPC_MDKEY(md->md));
    char* value = grpc_slice_to_c_string(GRPC_MDVALUE(md->md));
    gpr_log(GPR_INFO, "INPROC:%s:%s: %s: %s", is_initial ? "HDR" : "TRL",
            is_client ? "CLI" : "SVR", key, value);
    gpr_free(key);
    gpr_free(value);
  }
}

grpc_error* fill_in_metadata(inproc_stream* s,
                             const grpc_metadata_batch* metadata,
                             uint32_t flags, grpc_metadata_batch* out_md,
                             uint32_t* outflags, bool* markfilled) {
  if (grpc_inproc_trace.enabled()) {
    log_metadata(metadata, s->t->is_client, outflags != nullptr);
  }

  if (outflags != nullptr) {
    *outflags = flags;
  }
  if (markfilled != nullptr) {
    *markfilled = true;
  }
  grpc_error* error = GRPC_ERROR_NONE;
  for (grpc_linked_mdelem* elem = metadata->list.head;
       (elem != nullptr) && (error == GRPC_ERROR_NONE); elem = elem->next) {
    grpc_linked_mdelem* nelem = static_cast<grpc_linked_mdelem*>(
        gpr_arena_alloc(s->arena, sizeof(*nelem)));
    nelem->md =
        grpc_mdelem_from_slices(grpc_slice_intern(GRPC_MDKEY(elem->md)),
                                grpc_slice_intern(GRPC_MDVALUE(elem->md)));

    error = grpc_metadata_batch_link_tail(out_md, nelem);
  }
  return error;
}

int init_stream(grpc_transport* gt, grpc_stream* gs,
                grpc_stream_refcount* refcount, const void* server_data,
                gpr_arena* arena) {
  INPROC_LOG(GPR_INFO, "init_stream %p %p %p", gt, gs, server_data);
  inproc_transport* t = reinterpret_cast<inproc_transport*>(gt);
  new (gs) inproc_stream(t, refcount, server_data, arena);
  return 0;  // return value is not important
}

void close_stream_locked(inproc_stream* s) {
  if (!s->closed) {
    // Release the metadata that we would have written out
    grpc_metadata_batch_destroy(&s->write_buffer_initial_md);
    grpc_metadata_batch_destroy(&s->write_buffer_trailing_md);

    if (s->listed) {
      inproc_stream* p = s->stream_list_prev;
      inproc_stream* n = s->stream_list_next;
      if (p != nullptr) {
        p->stream_list_next = n;
      } else {
        s->t->stream_list = n;
      }
      if (n != nullptr) {
        n->stream_list_prev = p;
      }
      s->listed = false;
      s->unref("close_stream:list");
    }
    s->closed = true;
    s->unref("close_stream:closing");
  }
}

// This function means that we are done talking/listening to the other side
void close_other_side_locked(inproc_stream* s, const char* reason) {
  if (s->other_side != nullptr) {
    // First release the metadata that came from the other side's arena
    grpc_metadata_batch_destroy(&s->to_read_initial_md);
    grpc_metadata_batch_destroy(&s->to_read_trailing_md);

    s->other_side->unref(reason);
    s->other_side_closed = true;
    s->other_side = nullptr;
  } else if (!s->other_side_closed) {
    s->write_buffer_other_side_closed = true;
  }
}

// Call the on_complete closure associated with this stream_op_batch if
// this stream_op_batch is only one of the pending operations for this
// stream. This is called when one of the pending operations for the stream
// is done and about to be NULLed out
void complete_if_batch_end_locked(inproc_stream* s, grpc_error* error,
                                  grpc_transport_stream_op_batch* op,
                                  const char* msg) {
  int is_sm = static_cast<int>(op == s->send_message_op);
  int is_stm = static_cast<int>(op == s->send_trailing_md_op);
  // TODO(vjpai): We should not consider the recv ops here, since they
  // have their own callbacks.  We should invoke a batch's on_complete
  // as soon as all of the batch's send ops are complete, even if there
  // are still recv ops pending.
  int is_rim = static_cast<int>(op == s->recv_initial_md_op);
  int is_rm = static_cast<int>(op == s->recv_message_op);
  int is_rtm = static_cast<int>(op == s->recv_trailing_md_op);

  if ((is_sm + is_stm + is_rim + is_rm + is_rtm) == 1) {
    INPROC_LOG(GPR_INFO, "%s %p %p %p", msg, s, op, error);
    GRPC_CLOSURE_SCHED(op->on_complete, GRPC_ERROR_REF(error));
  }
}

void maybe_schedule_op_closure_locked(inproc_stream* s, grpc_error* error) {
  if (s && s->ops_needed && !s->op_closure_scheduled) {
    GRPC_CLOSURE_SCHED(&s->op_closure, GRPC_ERROR_REF(error));
    s->op_closure_scheduled = true;
    s->ops_needed = false;
  }
}

void fail_helper_locked(inproc_stream* s, grpc_error* error) {
  INPROC_LOG(GPR_INFO, "op_state_machine %p fail_helper", s);
  // If we're failing this side, we need to make sure that
  // we also send or have already sent trailing metadata
  if (!s->trailing_md_sent) {
    // Send trailing md to the other side indicating cancellation
    s->trailing_md_sent = true;

    grpc_metadata_batch fake_md;
    grpc_metadata_batch_init(&fake_md);

    inproc_stream* other = s->other_side;
    grpc_metadata_batch* dest = (other == nullptr)
                                    ? &s->write_buffer_trailing_md
                                    : &other->to_read_trailing_md;
    bool* destfilled = (other == nullptr) ? &s->write_buffer_trailing_md_filled
                                          : &other->to_read_trailing_md_filled;
    fill_in_metadata(s, &fake_md, 0, dest, nullptr, destfilled);
    grpc_metadata_batch_destroy(&fake_md);

    if (other != nullptr) {
      if (other->cancel_other_error == GRPC_ERROR_NONE) {
        other->cancel_other_error = GRPC_ERROR_REF(error);
      }
      maybe_schedule_op_closure_locked(other, error);
    } else if (s->write_buffer_cancel_error == GRPC_ERROR_NONE) {
      s->write_buffer_cancel_error = GRPC_ERROR_REF(error);
    }
  }
  if (s->recv_initial_md_op) {
    grpc_error* err;
    if (!s->t->is_client) {
      // If this is a server, provide initial metadata with a path and authority
      // since it expects that as well as no error yet
      grpc_metadata_batch fake_md;
      grpc_metadata_batch_init(&fake_md);
      grpc_linked_mdelem* path_md = static_cast<grpc_linked_mdelem*>(
          gpr_arena_alloc(s->arena, sizeof(*path_md)));
      path_md->md = grpc_mdelem_from_slices(g_fake_path_key, g_fake_path_value);
      GPR_ASSERT(grpc_metadata_batch_link_tail(&fake_md, path_md) ==
                 GRPC_ERROR_NONE);
      grpc_linked_mdelem* auth_md = static_cast<grpc_linked_mdelem*>(
          gpr_arena_alloc(s->arena, sizeof(*auth_md)));
      auth_md->md = grpc_mdelem_from_slices(g_fake_auth_key, g_fake_auth_value);
      GPR_ASSERT(grpc_metadata_batch_link_tail(&fake_md, auth_md) ==
                 GRPC_ERROR_NONE);

      fill_in_metadata(
          s, &fake_md, 0,
          s->recv_initial_md_op->payload->recv_initial_metadata
              .recv_initial_metadata,
          s->recv_initial_md_op->payload->recv_initial_metadata.recv_flags,
          nullptr);
      grpc_metadata_batch_destroy(&fake_md);
      err = GRPC_ERROR_NONE;
    } else {
      err = GRPC_ERROR_REF(error);
    }
    if (s->recv_initial_md_op->payload->recv_initial_metadata
            .trailing_metadata_available != nullptr) {
      // Set to true unconditionally, because we're failing the call, so even
      // if we haven't actually seen the send_trailing_metadata op from the
      // other side, we're going to return trailing metadata anyway.
      *s->recv_initial_md_op->payload->recv_initial_metadata
           .trailing_metadata_available = true;
    }
    INPROC_LOG(GPR_INFO,
               "fail_helper %p scheduling initial-metadata-ready %p %p", s,
               error, err);
    GRPC_CLOSURE_SCHED(s->recv_initial_md_op->payload->recv_initial_metadata
                           .recv_initial_metadata_ready,
                       err);
    // Last use of err so no need to REF and then UNREF it

    complete_if_batch_end_locked(
        s, error, s->recv_initial_md_op,
        "fail_helper scheduling recv-initial-metadata-on-complete");
    s->recv_initial_md_op = nullptr;
  }
  if (s->recv_message_op) {
    INPROC_LOG(GPR_INFO, "fail_helper %p scheduling message-ready %p", s,
               error);
    GRPC_CLOSURE_SCHED(
        s->recv_message_op->payload->recv_message.recv_message_ready,
        GRPC_ERROR_REF(error));
    complete_if_batch_end_locked(
        s, error, s->recv_message_op,
        "fail_helper scheduling recv-message-on-complete");
    s->recv_message_op = nullptr;
  }
  if (s->send_message_op) {
    s->send_message_op->payload->send_message.send_message.reset();
    complete_if_batch_end_locked(
        s, error, s->send_message_op,
        "fail_helper scheduling send-message-on-complete");
    s->send_message_op = nullptr;
  }
  if (s->send_trailing_md_op) {
    complete_if_batch_end_locked(
        s, error, s->send_trailing_md_op,
        "fail_helper scheduling send-trailng-md-on-complete");
    s->send_trailing_md_op = nullptr;
  }
  if (s->recv_trailing_md_op) {
    INPROC_LOG(GPR_INFO, "fail_helper %p scheduling trailing-metadata-ready %p",
               s, error);
    GRPC_CLOSURE_SCHED(s->recv_trailing_md_op->payload->recv_trailing_metadata
                           .recv_trailing_metadata_ready,
                       GRPC_ERROR_REF(error));
    INPROC_LOG(GPR_INFO, "fail_helper %p scheduling trailing-md-on-complete %p",
               s, error);
    complete_if_batch_end_locked(
        s, error, s->recv_trailing_md_op,
        "fail_helper scheduling recv-trailing-metadata-on-complete");
    s->recv_trailing_md_op = nullptr;
  }
  close_other_side_locked(s, "fail_helper:other_side");
  close_stream_locked(s);

  GRPC_ERROR_UNREF(error);
}

// TODO(vjpai): It should not be necessary to drain the incoming byte
// stream and create a new one; instead, we should simply pass the byte
// stream from the sender directly to the receiver as-is.
//
// Note that fixing this will also avoid the assumption in this code
// that the incoming byte stream's next() call will always return
// synchronously.  That assumption is true today but may not always be
// true in the future.
void message_transfer_locked(inproc_stream* sender, inproc_stream* receiver) {
  size_t remaining =
      sender->send_message_op->payload->send_message.send_message->length();
  if (receiver->recv_inited) {
    grpc_slice_buffer_destroy_internal(&receiver->recv_message);
  }
  grpc_slice_buffer_init(&receiver->recv_message);
  receiver->recv_inited = true;
  do {
    grpc_slice message_slice;
    grpc_closure unused;
    GPR_ASSERT(
        sender->send_message_op->payload->send_message.send_message->Next(
            SIZE_MAX, &unused));
    grpc_error* error =
        sender->send_message_op->payload->send_message.send_message->Pull(
            &message_slice);
    if (error != GRPC_ERROR_NONE) {
      cancel_stream_locked(sender, GRPC_ERROR_REF(error));
      break;
    }
    GPR_ASSERT(error == GRPC_ERROR_NONE);
    remaining -= GRPC_SLICE_LENGTH(message_slice);
    grpc_slice_buffer_add(&receiver->recv_message, message_slice);
  } while (remaining > 0);
  sender->send_message_op->payload->send_message.send_message.reset();

  receiver->recv_stream.Init(&receiver->recv_message, 0);
  receiver->recv_message_op->payload->recv_message.recv_message->reset(
      receiver->recv_stream.get());
  INPROC_LOG(GPR_INFO, "message_transfer_locked %p scheduling message-ready",
             receiver);
  GRPC_CLOSURE_SCHED(
      receiver->recv_message_op->payload->recv_message.recv_message_ready,
      GRPC_ERROR_NONE);
  complete_if_batch_end_locked(
      sender, GRPC_ERROR_NONE, sender->send_message_op,
      "message_transfer scheduling sender on_complete");
  complete_if_batch_end_locked(
      receiver, GRPC_ERROR_NONE, receiver->recv_message_op,
      "message_transfer scheduling receiver on_complete");

  receiver->recv_message_op = nullptr;
  sender->send_message_op = nullptr;
}

void op_state_machine(void* arg, grpc_error* error) {
  // This function gets called when we have contents in the unprocessed reads
  // Get what we want based on our ops wanted
  // Schedule our appropriate closures
  // and then return to ops_needed state if still needed

  // Since this is a closure directly invoked by the combiner, it should not
  // unref the error parameter explicitly; the combiner will do that implicitly
  grpc_error* new_err = GRPC_ERROR_NONE;

  bool needs_close = false;

  INPROC_LOG(GPR_INFO, "op_state_machine %p", arg);
  inproc_stream* s = static_cast<inproc_stream*>(arg);
  gpr_mu* mu = &s->t->mu->mu;  // keep aside in case s gets closed
  gpr_mu_lock(mu);
  s->op_closure_scheduled = false;
  // cancellation takes precedence
  inproc_stream* other = s->other_side;

  if (s->cancel_self_error != GRPC_ERROR_NONE) {
    fail_helper_locked(s, GRPC_ERROR_REF(s->cancel_self_error));
    goto done;
  } else if (s->cancel_other_error != GRPC_ERROR_NONE) {
    fail_helper_locked(s, GRPC_ERROR_REF(s->cancel_other_error));
    goto done;
  } else if (error != GRPC_ERROR_NONE) {
    fail_helper_locked(s, GRPC_ERROR_REF(error));
    goto done;
  }

  if (s->send_message_op && other) {
    if (other->recv_message_op) {
      message_transfer_locked(s, other);
      maybe_schedule_op_closure_locked(other, GRPC_ERROR_NONE);
    } else if (!s->t->is_client && s->trailing_md_sent) {
      // A server send will never be matched if the server already sent status
      s->send_message_op->payload->send_message.send_message.reset();
      complete_if_batch_end_locked(
          s, GRPC_ERROR_NONE, s->send_message_op,
          "op_state_machine scheduling send-message-on-complete");
      s->send_message_op = nullptr;
    }
  }
  // Pause a send trailing metadata if there is still an outstanding
  // send message unless we know that the send message will never get
  // matched to a receive. This happens on the client if the server has
  // already sent status or on the server if the client has requested
  // status
  if (s->send_trailing_md_op &&
      (!s->send_message_op ||
       (s->t->is_client &&
        (s->trailing_md_recvd || s->to_read_trailing_md_filled)) ||
       (!s->t->is_client && other &&
        (other->trailing_md_recvd || other->to_read_trailing_md_filled ||
         other->recv_trailing_md_op)))) {
    grpc_metadata_batch* dest = (other == nullptr)
                                    ? &s->write_buffer_trailing_md
                                    : &other->to_read_trailing_md;
    bool* destfilled = (other == nullptr) ? &s->write_buffer_trailing_md_filled
                                          : &other->to_read_trailing_md_filled;
    if (*destfilled || s->trailing_md_sent) {
      // The buffer is already in use; that's an error!
      INPROC_LOG(GPR_INFO, "Extra trailing metadata %p", s);
      new_err = GRPC_ERROR_CREATE_FROM_STATIC_STRING("Extra trailing metadata");
      fail_helper_locked(s, GRPC_ERROR_REF(new_err));
      goto done;
    } else {
      if (!other || !other->closed) {
        fill_in_metadata(s,
                         s->send_trailing_md_op->payload->send_trailing_metadata
                             .send_trailing_metadata,
                         0, dest, nullptr, destfilled);
      }
      s->trailing_md_sent = true;
      if (!s->t->is_client && s->trailing_md_recvd && s->recv_trailing_md_op) {
        INPROC_LOG(GPR_INFO,
                   "op_state_machine %p scheduling trailing-metadata-ready", s);
        GRPC_CLOSURE_SCHED(
            s->recv_trailing_md_op->payload->recv_trailing_metadata
                .recv_trailing_metadata_ready,
            GRPC_ERROR_NONE);
        INPROC_LOG(GPR_INFO,
                   "op_state_machine %p scheduling trailing-md-on-complete", s);
        GRPC_CLOSURE_SCHED(s->recv_trailing_md_op->on_complete,
                           GRPC_ERROR_NONE);
        s->recv_trailing_md_op = nullptr;
        needs_close = true;
      }
    }
    maybe_schedule_op_closure_locked(other, GRPC_ERROR_NONE);
    complete_if_batch_end_locked(
        s, GRPC_ERROR_NONE, s->send_trailing_md_op,
        "op_state_machine scheduling send-trailing-metadata-on-complete");
    s->send_trailing_md_op = nullptr;
  }
  if (s->recv_initial_md_op) {
    if (s->initial_md_recvd) {
      new_err =
          GRPC_ERROR_CREATE_FROM_STATIC_STRING("Already recvd initial md");
      INPROC_LOG(
          GPR_INFO,
          "op_state_machine %p scheduling on_complete errors for already "
          "recvd initial md %p",
          s, new_err);
      fail_helper_locked(s, GRPC_ERROR_REF(new_err));
      goto done;
    }

    if (s->to_read_initial_md_filled) {
      s->initial_md_recvd = true;
      new_err = fill_in_metadata(
          s, &s->to_read_initial_md, s->to_read_initial_md_flags,
          s->recv_initial_md_op->payload->recv_initial_metadata
              .recv_initial_metadata,
          s->recv_initial_md_op->payload->recv_initial_metadata.recv_flags,
          nullptr);
      s->recv_initial_md_op->payload->recv_initial_metadata
          .recv_initial_metadata->deadline = s->deadline;
      if (s->recv_initial_md_op->payload->recv_initial_metadata
              .trailing_metadata_available != nullptr) {
        *s->recv_initial_md_op->payload->recv_initial_metadata
             .trailing_metadata_available =
            (other != nullptr && other->send_trailing_md_op != nullptr);
      }
      grpc_metadata_batch_clear(&s->to_read_initial_md);
      s->to_read_initial_md_filled = false;
      INPROC_LOG(GPR_INFO,
                 "op_state_machine %p scheduling initial-metadata-ready %p", s,
                 new_err);
      GRPC_CLOSURE_SCHED(s->recv_initial_md_op->payload->recv_initial_metadata
                             .recv_initial_metadata_ready,
                         GRPC_ERROR_REF(new_err));
      complete_if_batch_end_locked(
          s, new_err, s->recv_initial_md_op,
          "op_state_machine scheduling recv-initial-metadata-on-complete");
      s->recv_initial_md_op = nullptr;

      if (new_err != GRPC_ERROR_NONE) {
        INPROC_LOG(GPR_INFO,
                   "op_state_machine %p scheduling on_complete errors2 %p", s,
                   new_err);
        fail_helper_locked(s, GRPC_ERROR_REF(new_err));
        goto done;
      }
    }
  }
  if (s->recv_message_op) {
    if (other && other->send_message_op) {
      message_transfer_locked(other, s);
      maybe_schedule_op_closure_locked(other, GRPC_ERROR_NONE);
    }
  }
  if (s->to_read_trailing_md_filled) {
    if (s->trailing_md_recvd) {
      new_err =
          GRPC_ERROR_CREATE_FROM_STATIC_STRING("Already recvd trailing md");
      INPROC_LOG(
          GPR_INFO,
          "op_state_machine %p scheduling on_complete errors for already "
          "recvd trailing md %p",
          s, new_err);
      fail_helper_locked(s, GRPC_ERROR_REF(new_err));
      goto done;
    }
    if (s->recv_message_op != nullptr) {
      // This message needs to be wrapped up because it will never be
      // satisfied
      *s->recv_message_op->payload->recv_message.recv_message = nullptr;
      INPROC_LOG(GPR_INFO, "op_state_machine %p scheduling message-ready", s);
      GRPC_CLOSURE_SCHED(
          s->recv_message_op->payload->recv_message.recv_message_ready,
          GRPC_ERROR_NONE);
      complete_if_batch_end_locked(
          s, new_err, s->recv_message_op,
          "op_state_machine scheduling recv-message-on-complete");
      s->recv_message_op = nullptr;
    }
    if ((s->trailing_md_sent || s->t->is_client) && s->send_message_op) {
      // Nothing further will try to receive from this stream, so finish off
      // any outstanding send_message op
      s->send_message_op->payload->send_message.send_message.reset();
      complete_if_batch_end_locked(
          s, new_err, s->send_message_op,
          "op_state_machine scheduling send-message-on-complete");
      s->send_message_op = nullptr;
    }
    if (s->recv_trailing_md_op != nullptr) {
      // We wanted trailing metadata and we got it
      s->trailing_md_recvd = true;
      new_err =
          fill_in_metadata(s, &s->to_read_trailing_md, 0,
                           s->recv_trailing_md_op->payload
                               ->recv_trailing_metadata.recv_trailing_metadata,
                           nullptr, nullptr);
      grpc_metadata_batch_clear(&s->to_read_trailing_md);
      s->to_read_trailing_md_filled = false;

      // We should schedule the recv_trailing_md_op completion if
      // 1. this stream is the client-side
      // 2. this stream is the server-side AND has already sent its trailing md
      //    (If the server hasn't already sent its trailing md, it doesn't have
      //     a final status, so don't mark this op complete)
      if (s->t->is_client || s->trailing_md_sent) {
        INPROC_LOG(GPR_INFO,
                   "op_state_machine %p scheduling trailing-md-on-complete %p",
                   s, new_err);
        GRPC_CLOSURE_SCHED(
            s->recv_trailing_md_op->payload->recv_trailing_metadata
                .recv_trailing_metadata_ready,
            GRPC_ERROR_REF(new_err));
        GRPC_CLOSURE_SCHED(s->recv_trailing_md_op->on_complete,
                           GRPC_ERROR_REF(new_err));
        s->recv_trailing_md_op = nullptr;
        needs_close = true;
      } else {
        INPROC_LOG(GPR_INFO,
                   "op_state_machine %p server needs to delay handling "
                   "trailing-md-on-complete %p",
                   s, new_err);
      }
    } else {
      INPROC_LOG(
          GPR_INFO,
          "op_state_machine %p has trailing md but not yet waiting for it", s);
    }
  }
  if (s->trailing_md_recvd && s->recv_message_op) {
    // No further message will come on this stream, so finish off the
    // recv_message_op
    INPROC_LOG(GPR_INFO, "op_state_machine %p scheduling message-ready", s);
    *s->recv_message_op->payload->recv_message.recv_message = nullptr;
    GRPC_CLOSURE_SCHED(
        s->recv_message_op->payload->recv_message.recv_message_ready,
        GRPC_ERROR_NONE);
    complete_if_batch_end_locked(
        s, new_err, s->recv_message_op,
        "op_state_machine scheduling recv-message-on-complete");
    s->recv_message_op = nullptr;
  }
  if (s->trailing_md_recvd && (s->trailing_md_sent || s->t->is_client) &&
      s->send_message_op) {
    // Nothing further will try to receive from this stream, so finish off
    // any outstanding send_message op
    s->send_message_op->payload->send_message.send_message.reset();
    complete_if_batch_end_locked(
        s, new_err, s->send_message_op,
        "op_state_machine scheduling send-message-on-complete");
    s->send_message_op = nullptr;
  }
  if (s->send_message_op || s->send_trailing_md_op || s->recv_initial_md_op ||
      s->recv_message_op || s->recv_trailing_md_op) {
    // Didn't get the item we wanted so we still need to get
    // rescheduled
    INPROC_LOG(
        GPR_INFO, "op_state_machine %p still needs closure %p %p %p %p %p", s,
        s->send_message_op, s->send_trailing_md_op, s->recv_initial_md_op,
        s->recv_message_op, s->recv_trailing_md_op);
    s->ops_needed = true;
  }
done:
  if (needs_close) {
    close_other_side_locked(s, "op_state_machine");
    close_stream_locked(s);
  }
  gpr_mu_unlock(mu);
  GRPC_ERROR_UNREF(new_err);
}

bool cancel_stream_locked(inproc_stream* s, grpc_error* error) {
  bool ret = false;  // was the cancel accepted
  INPROC_LOG(GPR_INFO, "cancel_stream %p with %s", s, grpc_error_string(error));
  if (s->cancel_self_error == GRPC_ERROR_NONE) {
    ret = true;
    s->cancel_self_error = GRPC_ERROR_REF(error);
    maybe_schedule_op_closure_locked(s, s->cancel_self_error);
    // Send trailing md to the other side indicating cancellation, even if we
    // already have
    s->trailing_md_sent = true;

    grpc_metadata_batch cancel_md;
    grpc_metadata_batch_init(&cancel_md);

    inproc_stream* other = s->other_side;
    grpc_metadata_batch* dest = (other == nullptr)
                                    ? &s->write_buffer_trailing_md
                                    : &other->to_read_trailing_md;
    bool* destfilled = (other == nullptr) ? &s->write_buffer_trailing_md_filled
                                          : &other->to_read_trailing_md_filled;
    fill_in_metadata(s, &cancel_md, 0, dest, nullptr, destfilled);
    grpc_metadata_batch_destroy(&cancel_md);

    if (other != nullptr) {
      if (other->cancel_other_error == GRPC_ERROR_NONE) {
        other->cancel_other_error = GRPC_ERROR_REF(s->cancel_self_error);
      }
      maybe_schedule_op_closure_locked(other, other->cancel_other_error);
    } else if (s->write_buffer_cancel_error == GRPC_ERROR_NONE) {
      s->write_buffer_cancel_error = GRPC_ERROR_REF(s->cancel_self_error);
    }

    // if we are a server and already received trailing md but
    // couldn't complete that because we hadn't yet sent out trailing
    // md, now's the chance
    if (!s->t->is_client && s->trailing_md_recvd && s->recv_trailing_md_op) {
      GRPC_CLOSURE_SCHED(s->recv_trailing_md_op->payload->recv_trailing_metadata
                             .recv_trailing_metadata_ready,
                         GRPC_ERROR_REF(s->cancel_self_error));
      complete_if_batch_end_locked(
          s, s->cancel_self_error, s->recv_trailing_md_op,
          "cancel_stream scheduling trailing-md-on-complete");
      s->recv_trailing_md_op = nullptr;
    }
  }

  close_other_side_locked(s, "cancel_stream:other_side");
  close_stream_locked(s);

  GRPC_ERROR_UNREF(error);
  return ret;
}

void do_nothing(void* arg, grpc_error* error) {}

void perform_stream_op(grpc_transport* gt, grpc_stream* gs,
                       grpc_transport_stream_op_batch* op) {
  INPROC_LOG(GPR_INFO, "perform_stream_op %p %p %p", gt, gs, op);
  inproc_stream* s = reinterpret_cast<inproc_stream*>(gs);
  gpr_mu* mu = &s->t->mu->mu;  // save aside in case s gets closed
  gpr_mu_lock(mu);

  if (grpc_inproc_trace.enabled()) {
    if (op->send_initial_metadata) {
      log_metadata(op->payload->send_initial_metadata.send_initial_metadata,
                   s->t->is_client, true);
    }
    if (op->send_trailing_metadata) {
      log_metadata(op->payload->send_trailing_metadata.send_trailing_metadata,
                   s->t->is_client, false);
    }
  }
  grpc_error* error = GRPC_ERROR_NONE;
  grpc_closure* on_complete = op->on_complete;
  // TODO(roth): This is a hack needed because we use data inside of the
  // closure itself to do the barrier calculation (i.e., to ensure that
  // we don't schedule the closure until all ops in the batch have been
  // completed).  This can go away once we move to a new C++ closure API
  // that provides the ability to create a barrier closure.
  if (on_complete == nullptr) {
    on_complete = GRPC_CLOSURE_INIT(&op->handler_private.closure, do_nothing,
                                    nullptr, grpc_schedule_on_exec_ctx);
  }

  if (op->cancel_stream) {
    // Call cancel_stream_locked without ref'ing the cancel_error because
    // this function is responsible to make sure that that field gets unref'ed
    cancel_stream_locked(s, op->payload->cancel_stream.cancel_error);
    // this op can complete without an error
  } else if (s->cancel_self_error != GRPC_ERROR_NONE) {
    // already self-canceled so still give it an error
    error = GRPC_ERROR_REF(s->cancel_self_error);
  } else {
    INPROC_LOG(GPR_INFO, "perform_stream_op %p %s%s%s%s%s%s%s", s,
               s->t->is_client ? "client" : "server",
               op->send_initial_metadata ? " send_initial_metadata" : "",
               op->send_message ? " send_message" : "",
               op->send_trailing_metadata ? " send_trailing_metadata" : "",
               op->recv_initial_metadata ? " recv_initial_metadata" : "",
               op->recv_message ? " recv_message" : "",
               op->recv_trailing_metadata ? " recv_trailing_metadata" : "");
  }

  bool needs_close = false;

  inproc_stream* other = s->other_side;
  if (error == GRPC_ERROR_NONE &&
      (op->send_initial_metadata || op->send_trailing_metadata)) {
    if (s->t->is_closed) {
      error = GRPC_ERROR_CREATE_FROM_STATIC_STRING("Endpoint already shutdown");
    }
    if (error == GRPC_ERROR_NONE && op->send_initial_metadata) {
      grpc_metadata_batch* dest = (other == nullptr)
                                      ? &s->write_buffer_initial_md
                                      : &other->to_read_initial_md;
      uint32_t* destflags = (other == nullptr)
                                ? &s->write_buffer_initial_md_flags
                                : &other->to_read_initial_md_flags;
      bool* destfilled = (other == nullptr) ? &s->write_buffer_initial_md_filled
                                            : &other->to_read_initial_md_filled;
      if (*destfilled || s->initial_md_sent) {
        // The buffer is already in use; that's an error!
        INPROC_LOG(GPR_INFO, "Extra initial metadata %p", s);
        error = GRPC_ERROR_CREATE_FROM_STATIC_STRING("Extra initial metadata");
      } else {
        if (!other || !other->closed) {
          fill_in_metadata(
              s, op->payload->send_initial_metadata.send_initial_metadata,
              op->payload->send_initial_metadata.send_initial_metadata_flags,
              dest, destflags, destfilled);
        }
        if (s->t->is_client) {
          grpc_millis* dl =
              (other == nullptr) ? &s->write_buffer_deadline : &other->deadline;
          *dl = GPR_MIN(*dl, op->payload->send_initial_metadata
                                 .send_initial_metadata->deadline);
          s->initial_md_sent = true;
        }
      }
      maybe_schedule_op_closure_locked(other, error);
    }
  }

  if (error == GRPC_ERROR_NONE &&
      (op->send_message || op->send_trailing_metadata ||
       op->recv_initial_metadata || op->recv_message ||
       op->recv_trailing_metadata)) {
    // Mark ops that need to be processed by the closure
    if (op->send_message) {
      s->send_message_op = op;
    }
    if (op->send_trailing_metadata) {
      s->send_trailing_md_op = op;
    }
    if (op->recv_initial_metadata) {
      s->recv_initial_md_op = op;
    }
    if (op->recv_message) {
      s->recv_message_op = op;
    }
    if (op->recv_trailing_metadata) {
      s->recv_trailing_md_op = op;
    }

    // We want to initiate the closure if:
    // 1. We want to send a message and the other side wants to receive
    // 2. We want to send trailing metadata and there isn't an unmatched send
    //    or the other side wants trailing metadata
    // 3. We want initial metadata and the other side has sent it
    // 4. We want to receive a message and there is a message ready
    // 5. There is trailing metadata, even if nothing specifically wants
    //    that because that can shut down the receive message as well
    if ((op->send_message && other && other->recv_message_op != nullptr) ||
        (op->send_trailing_metadata &&
         (!s->send_message_op || (other && other->recv_trailing_md_op))) ||
        (op->recv_initial_metadata && s->to_read_initial_md_filled) ||
        (op->recv_message && other && other->send_message_op != nullptr) ||
        (s->to_read_trailing_md_filled || s->trailing_md_recvd)) {
      if (!s->op_closure_scheduled) {
        GRPC_CLOSURE_SCHED(&s->op_closure, GRPC_ERROR_NONE);
        s->op_closure_scheduled = true;
      }
    } else {
      s->ops_needed = true;
    }
  } else {
    if (error != GRPC_ERROR_NONE) {
      // Schedule op's closures that we didn't push to op state machine
      if (op->recv_initial_metadata) {
        if (op->payload->recv_initial_metadata.trailing_metadata_available !=
            nullptr) {
          // Set to true unconditionally, because we're failing the call, so
          // even if we haven't actually seen the send_trailing_metadata op
          // from the other side, we're going to return trailing metadata
          // anyway.
          *op->payload->recv_initial_metadata.trailing_metadata_available =
              true;
        }
        INPROC_LOG(
            GPR_INFO,
            "perform_stream_op error %p scheduling initial-metadata-ready %p",
            s, error);
        GRPC_CLOSURE_SCHED(
            op->payload->recv_initial_metadata.recv_initial_metadata_ready,
            GRPC_ERROR_REF(error));
      }
      if (op->recv_message) {
        INPROC_LOG(
            GPR_INFO,
            "perform_stream_op error %p scheduling recv message-ready %p", s,
            error);
        GRPC_CLOSURE_SCHED(op->payload->recv_message.recv_message_ready,
                           GRPC_ERROR_REF(error));
      }
      if (op->recv_trailing_metadata) {
        INPROC_LOG(
            GPR_INFO,
            "perform_stream_op error %p scheduling trailing-metadata-ready %p",
            s, error);
        GRPC_CLOSURE_SCHED(
            op->payload->recv_trailing_metadata.recv_trailing_metadata_ready,
            GRPC_ERROR_REF(error));
      }
    }
    INPROC_LOG(GPR_INFO, "perform_stream_op %p scheduling on_complete %p", s,
               error);
    GRPC_CLOSURE_SCHED(on_complete, GRPC_ERROR_REF(error));
  }
  if (needs_close) {
    close_other_side_locked(s, "perform_stream_op:other_side");
    close_stream_locked(s);
  }
  gpr_mu_unlock(mu);
  GRPC_ERROR_UNREF(error);
}

void close_transport_locked(inproc_transport* t) {
  INPROC_LOG(GPR_INFO, "close_transport %p %d", t, t->is_closed);
  grpc_connectivity_state_set(
      &t->connectivity, GRPC_CHANNEL_SHUTDOWN,
      GRPC_ERROR_CREATE_FROM_STATIC_STRING("Closing transport."),
      "close transport");
  if (!t->is_closed) {
    t->is_closed = true;
    /* Also end all streams on this transport */
    while (t->stream_list != nullptr) {
      // cancel_stream_locked also adjusts stream list
      cancel_stream_locked(
          t->stream_list,
          grpc_error_set_int(
              GRPC_ERROR_CREATE_FROM_STATIC_STRING("Transport closed"),
              GRPC_ERROR_INT_GRPC_STATUS, GRPC_STATUS_UNAVAILABLE));
    }
  }
}

void perform_transport_op(grpc_transport* gt, grpc_transport_op* op) {
  inproc_transport* t = reinterpret_cast<inproc_transport*>(gt);
  INPROC_LOG(GPR_INFO, "perform_transport_op %p %p", t, op);
  gpr_mu_lock(&t->mu->mu);
  if (op->on_connectivity_state_change) {
    grpc_connectivity_state_notify_on_state_change(
        &t->connectivity, op->connectivity_state,
        op->on_connectivity_state_change);
  }
  if (op->set_accept_stream) {
    t->accept_stream_cb = op->set_accept_stream_fn;
    t->accept_stream_data = op->set_accept_stream_user_data;
  }
  if (op->on_consumed) {
    GRPC_CLOSURE_SCHED(op->on_consumed, GRPC_ERROR_NONE);
  }

  bool do_close = false;
  if (op->goaway_error != GRPC_ERROR_NONE) {
    do_close = true;
    GRPC_ERROR_UNREF(op->goaway_error);
  }
  if (op->disconnect_with_error != GRPC_ERROR_NONE) {
    do_close = true;
    GRPC_ERROR_UNREF(op->disconnect_with_error);
  }

  if (do_close) {
    close_transport_locked(t);
  }
  gpr_mu_unlock(&t->mu->mu);
}

void destroy_stream(grpc_transport* gt, grpc_stream* gs,
                    grpc_closure* then_schedule_closure) {
  INPROC_LOG(GPR_INFO, "destroy_stream %p %p", gs, then_schedule_closure);
  inproc_stream* s = reinterpret_cast<inproc_stream*>(gs);
  s->closure_at_destroy = then_schedule_closure;
  s->~inproc_stream();
}

void destroy_transport(grpc_transport* gt) {
  inproc_transport* t = reinterpret_cast<inproc_transport*>(gt);
  INPROC_LOG(GPR_INFO, "destroy_transport %p", t);
  gpr_mu_lock(&t->mu->mu);
  close_transport_locked(t);
  gpr_mu_unlock(&t->mu->mu);
  t->other_side->unref();
  t->unref();
}

/*******************************************************************************
 * INTEGRATION GLUE
 */

void set_pollset(grpc_transport* gt, grpc_stream* gs, grpc_pollset* pollset) {
  // Nothing to do here
}

void set_pollset_set(grpc_transport* gt, grpc_stream* gs,
                     grpc_pollset_set* pollset_set) {
  // Nothing to do here
}

grpc_endpoint* get_endpoint(grpc_transport* t) { return nullptr; }

const grpc_transport_vtable inproc_vtable = {
    sizeof(inproc_stream), "inproc",        init_stream,
    set_pollset,           set_pollset_set, perform_stream_op,
    perform_transport_op,  destroy_stream,  destroy_transport,
    get_endpoint};

/*******************************************************************************
 * Main inproc transport functions
 */
void inproc_transports_create(grpc_transport** server_transport,
                              const grpc_channel_args* server_args,
                              grpc_transport** client_transport,
                              const grpc_channel_args* client_args) {
  INPROC_LOG(GPR_INFO, "inproc_transports_create");
  shared_mu* mu = new (gpr_malloc(sizeof(*mu))) shared_mu();
  inproc_transport* st = new (gpr_malloc(sizeof(*st)))
      inproc_transport(&inproc_vtable, mu, /*is_client=*/false);
  inproc_transport* ct = new (gpr_malloc(sizeof(*ct)))
      inproc_transport(&inproc_vtable, mu, /*is_client=*/true);
  st->other_side = ct;
  ct->other_side = st;
  *server_transport = reinterpret_cast<grpc_transport*>(st);
  *client_transport = reinterpret_cast<grpc_transport*>(ct);
}
}  // namespace

/*******************************************************************************
 * GLOBAL INIT AND DESTROY
 */
void grpc_inproc_transport_init(void) {
  grpc_core::ExecCtx exec_ctx;
  g_empty_slice = grpc_slice_from_static_buffer(nullptr, 0);

  grpc_slice key_tmp = grpc_slice_from_static_string(":path");
  g_fake_path_key = grpc_slice_intern(key_tmp);
  grpc_slice_unref_internal(key_tmp);

  g_fake_path_value = grpc_slice_from_static_string("/");

  grpc_slice auth_tmp = grpc_slice_from_static_string(":authority");
  g_fake_auth_key = grpc_slice_intern(auth_tmp);
  grpc_slice_unref_internal(auth_tmp);

  g_fake_auth_value = grpc_slice_from_static_string("inproc-fail");
}

grpc_channel* grpc_inproc_channel_create(grpc_server* server,
                                         grpc_channel_args* args,
                                         void* reserved) {
  GRPC_API_TRACE("grpc_inproc_channel_create(server=%p, args=%p)", 2,
                 (server, args));

  grpc_core::ExecCtx exec_ctx;

  const grpc_channel_args* server_args = grpc_server_get_channel_args(server);

  // Add a default authority channel argument for the client

  grpc_arg default_authority_arg;
  default_authority_arg.type = GRPC_ARG_STRING;
  default_authority_arg.key = (char*)GRPC_ARG_DEFAULT_AUTHORITY;
  default_authority_arg.value.string = (char*)"inproc.authority";
  grpc_channel_args* client_args =
      grpc_channel_args_copy_and_add(args, &default_authority_arg, 1);

  grpc_transport* server_transport;
  grpc_transport* client_transport;
  inproc_transports_create(&server_transport, server_args, &client_transport,
                           client_args);

  // TODO(ncteisen): design and support channelz GetSocket for inproc.
  grpc_server_setup_transport(server, server_transport, nullptr, server_args,
                              nullptr);
  grpc_channel* channel = grpc_channel_create(
      "inproc", client_args, GRPC_CLIENT_DIRECT_CHANNEL, client_transport);

  // Free up created channel args
  grpc_channel_args_destroy(client_args);

  // Now finish scheduled operations

  return channel;
}

void grpc_inproc_transport_shutdown(void) {
  grpc_core::ExecCtx exec_ctx;
  grpc_slice_unref_internal(g_empty_slice);
  grpc_slice_unref_internal(g_fake_path_key);
  grpc_slice_unref_internal(g_fake_path_value);
  grpc_slice_unref_internal(g_fake_auth_key);
  grpc_slice_unref_internal(g_fake_auth_value);
}
