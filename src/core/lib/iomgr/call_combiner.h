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

#ifndef GRPC_CORE_LIB_IOMGR_CALL_COMBINER_H
#define GRPC_CORE_LIB_IOMGR_CALL_COMBINER_H

#include <stddef.h>

#include <grpc/support/atm.h>

#include "src/core/lib/iomgr/closure.h"
#include "src/core/lib/iomgr/exec_ctx.h"
#include "src/core/lib/support/mpscq.h"

// A simple, lock-free mechanism for serializing activity related to a
// single call.  This is similar to a combiner but is more lightweight.
//
// It requires the callback (or, in the common case where the callback
// actually kicks off a chain of callbacks, the last callback in that
// chain) to explicitly indicate (by calling GRPC_CALL_COMBINER_STOP())
// when it is done with the action that was kicked off by the original
// callback.

extern grpc_tracer_flag grpc_call_combiner_trace;

typedef struct {
  gpr_atm size;  // size_t, num closures in queue or currently executing
  gpr_mpscq queue;
  // Either 0 (if not cancelled and no cancellation closure set),
  // a grpc_closure* (if the lowest bit is 0),
  // or a grpc_error* (if the lowest bit is 1).
  gpr_atm cancel_state;
} grpc_call_combiner;

// Assumes memory was initialized to zero.
void grpc_call_combiner_init(grpc_call_combiner* call_combiner);

void grpc_call_combiner_destroy(grpc_call_combiner* call_combiner);

#ifndef NDEBUG
#define GRPC_CALL_COMBINER_START(exec_ctx, call_combiner, closure, error,   \
                                 reason)                                    \
  grpc_call_combiner_start((exec_ctx), (call_combiner), (closure), (error), \
                           __FILE__, __LINE__, (reason))
#define GRPC_CALL_COMBINER_STOP(exec_ctx, call_combiner, reason)           \
  grpc_call_combiner_stop((exec_ctx), (call_combiner), __FILE__, __LINE__, \
                          (reason))
/// Starts processing \a closure on \a call_combiner.
void grpc_call_combiner_start(grpc_exec_ctx* exec_ctx,
                              grpc_call_combiner* call_combiner,
                              grpc_closure* closure, grpc_error* error,
                              const char* file, int line, const char* reason);
/// Yields the call combiner to the next closure in the queue, if any.
void grpc_call_combiner_stop(grpc_exec_ctx* exec_ctx,
                             grpc_call_combiner* call_combiner,
                             const char* file, int line, const char* reason);
#else
#define GRPC_CALL_COMBINER_START(exec_ctx, call_combiner, closure, error,   \
                                 reason)                                    \
  grpc_call_combiner_start((exec_ctx), (call_combiner), (closure), (error), \
                           (reason))
#define GRPC_CALL_COMBINER_STOP(exec_ctx, call_combiner, reason) \
  grpc_call_combiner_stop((exec_ctx), (call_combiner), (reason))
/// Starts processing \a closure on \a call_combiner.
void grpc_call_combiner_start(grpc_exec_ctx* exec_ctx,
                              grpc_call_combiner* call_combiner,
                              grpc_closure* closure, grpc_error* error,
                              const char* reason);
/// Yields the call combiner to the next closure in the queue, if any.
void grpc_call_combiner_stop(grpc_exec_ctx* exec_ctx,
                             grpc_call_combiner* call_combiner,
                             const char* reason);
#endif

/// Tells \a call_combiner to schedule \a closure when
/// grpc_call_combiner_cancel() is called.
///
/// If grpc_call_combiner_cancel() was previously called, \a closure will be
/// scheduled immediately.
///
/// If \a closure is NULL, then no closure will be invoked on
/// cancellation; this effectively unregisters the previously set closure.
///
/// If a closure was set via a previous call to
/// grpc_call_combiner_set_notify_on_cancel(), the previous closure will be
/// scheduled immediately with GRPC_ERROR_NONE.  This ensures that
/// \a closure will be scheduled exactly once, which allows callers to clean
/// up resources they may be holding for the callback.
void grpc_call_combiner_set_notify_on_cancel(grpc_exec_ctx* exec_ctx,
                                             grpc_call_combiner* call_combiner,
                                             grpc_closure* closure);

/// Indicates that the call has been cancelled.
void grpc_call_combiner_cancel(grpc_exec_ctx* exec_ctx,
                               grpc_call_combiner* call_combiner,
                               grpc_error* error);

#endif /* GRPC_CORE_LIB_IOMGR_CALL_COMBINER_H */
