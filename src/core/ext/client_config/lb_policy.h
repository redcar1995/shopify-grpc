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

#ifndef GRPC_CORE_EXT_CLIENT_CONFIG_LB_POLICY_H
#define GRPC_CORE_EXT_CLIENT_CONFIG_LB_POLICY_H

#include "src/core/ext/client_config/subchannel.h"
#include "src/core/lib/iomgr/polling_entity.h"
#include "src/core/lib/transport/connectivity_state.h"

/** A load balancing policy: specified by a vtable and a struct (which
    is expected to be extended to contain some parameters) */
typedef struct grpc_lb_policy grpc_lb_policy;
typedef struct grpc_lb_policy_vtable grpc_lb_policy_vtable;

typedef void (*grpc_lb_completion)(void *cb_arg, grpc_subchannel *subchannel,
                                   grpc_status_code status, const char *errmsg);

struct grpc_lb_policy {
  const grpc_lb_policy_vtable *vtable;
  gpr_atm ref_pair;
  /* owned pointer to interested parties in load balancing decisions */
  grpc_pollset_set *interested_parties;
};

/** Extra arguments for an LB pick */
typedef struct grpc_lb_policy_pick_args {
  /** Parties interested in the pick's progress */
  grpc_polling_entity *pollent;
  /** Initial metadata associated with the picking call. */
  grpc_metadata_batch *initial_metadata;
  /** See \a GRPC_INITIAL_METADATA_* in grpc_types.h */
  uint32_t initial_metadata_flags;
} grpc_lb_policy_pick_args;

struct grpc_lb_policy_vtable {
  void (*destroy)(grpc_exec_ctx *exec_ctx, grpc_lb_policy *policy);
  void (*shutdown)(grpc_exec_ctx *exec_ctx, grpc_lb_policy *policy);

  /** \see grpc_lb_policy_pick */
  int (*pick)(grpc_exec_ctx *exec_ctx, grpc_lb_policy *policy,
              const grpc_lb_policy_pick_args *pick_args,
              grpc_connected_subchannel **target, grpc_closure *on_complete);

  /** \see grpc_lb_policy_cancel_pick */
  void (*cancel_pick)(grpc_exec_ctx *exec_ctx, grpc_lb_policy *policy,
                      grpc_connected_subchannel **target);

  /** \see grpc_lb_policy_cancel_picks */
  void (*cancel_picks)(grpc_exec_ctx *exec_ctx, grpc_lb_policy *policy,
                       uint32_t initial_metadata_flags_mask,
                       uint32_t initial_metadata_flags_eq);

  /** \see grpc_lb_policy_ping_one */
  void (*ping_one)(grpc_exec_ctx *exec_ctx, grpc_lb_policy *policy,
                   grpc_closure *closure);

  /** Try to enter a READY connectivity state */
  void (*exit_idle)(grpc_exec_ctx *exec_ctx, grpc_lb_policy *policy);

  /** check the current connectivity of the lb_policy */
  grpc_connectivity_state (*check_connectivity)(
      grpc_exec_ctx *exec_ctx, grpc_lb_policy *policy,
      grpc_error **connectivity_error);

  /** call notify when the connectivity state of a channel changes from *state.
      Updates *state with the new state of the policy. Calling with a NULL \a
      state cancels the subscription.  */
  void (*notify_on_state_change)(grpc_exec_ctx *exec_ctx,
                                 grpc_lb_policy *policy,
                                 grpc_connectivity_state *state,
                                 grpc_closure *closure);
};

/*#define GRPC_LB_POLICY_REFCOUNT_DEBUG*/
#ifdef GRPC_LB_POLICY_REFCOUNT_DEBUG
#define GRPC_LB_POLICY_REF(p, r) \
  grpc_lb_policy_ref((p), __FILE__, __LINE__, (r))
#define GRPC_LB_POLICY_UNREF(exec_ctx, p, r) \
  grpc_lb_policy_unref((exec_ctx), (p), __FILE__, __LINE__, (r))
#define GRPC_LB_POLICY_WEAK_REF(p, r) \
  grpc_lb_policy_weak_ref((p), __FILE__, __LINE__, (r))
#define GRPC_LB_POLICY_WEAK_UNREF(exec_ctx, p, r) \
  grpc_lb_policy_weak_unref((exec_ctx), (p), __FILE__, __LINE__, (r))
void grpc_lb_policy_ref(grpc_lb_policy *policy, const char *file, int line,
                        const char *reason);
void grpc_lb_policy_unref(grpc_exec_ctx *exec_ctx, grpc_lb_policy *policy,
                          const char *file, int line, const char *reason);
void grpc_lb_policy_weak_ref(grpc_lb_policy *policy, const char *file, int line,
                             const char *reason);
void grpc_lb_policy_weak_unref(grpc_exec_ctx *exec_ctx, grpc_lb_policy *policy,
                               const char *file, int line, const char *reason);
#else
#define GRPC_LB_POLICY_REF(p, r) grpc_lb_policy_ref((p))
#define GRPC_LB_POLICY_UNREF(cl, p, r) grpc_lb_policy_unref((cl), (p))
#define GRPC_LB_POLICY_WEAK_REF(p, r) grpc_lb_policy_weak_ref((p))
#define GRPC_LB_POLICY_WEAK_UNREF(cl, p, r) grpc_lb_policy_weak_unref((cl), (p))
void grpc_lb_policy_ref(grpc_lb_policy *policy);
void grpc_lb_policy_unref(grpc_exec_ctx *exec_ctx, grpc_lb_policy *policy);
void grpc_lb_policy_weak_ref(grpc_lb_policy *policy);
void grpc_lb_policy_weak_unref(grpc_exec_ctx *exec_ctx, grpc_lb_policy *policy);
#endif

/** called by concrete implementations to initialize the base struct */
void grpc_lb_policy_init(grpc_lb_policy *policy,
                         const grpc_lb_policy_vtable *vtable);

/** Find an appropriate target for this call, based on \a pick_args.
    Upon completion \a on_complete will be called, with \a *target set to an
    appropriate connected subchannel if the pick was successful or NULL
    otherwise.
    Picking can be asynchronous. Any IO should be done under \a
    pick_args->pollent. */
int grpc_lb_policy_pick(grpc_exec_ctx *exec_ctx, grpc_lb_policy *policy,
                        const grpc_lb_policy_pick_args *pick_args,
                        grpc_connected_subchannel **target,
                        grpc_closure *on_complete);

/** Perform a connected subchannel ping (see \a grpc_connected_subchannel_ping)
    against one of the connected subchannels managed by \a policy. */
void grpc_lb_policy_ping_one(grpc_exec_ctx *exec_ctx, grpc_lb_policy *policy,
                             grpc_closure *closure);

/** Cancel picks for \a target.
    The \a on_complete callback of the pending picks will be invoked with \a
    *target set to NULL. */
void grpc_lb_policy_cancel_pick(grpc_exec_ctx *exec_ctx, grpc_lb_policy *policy,
                                grpc_connected_subchannel **target);

/** Cancel all pending picks for which their \a initial_metadata_flags (as given
    in the call to \a grpc_lb_policy_pick) matches \a initial_metadata_flags_eq
    when AND'd with \a initial_metadata_flags_mask */
void grpc_lb_policy_cancel_picks(grpc_exec_ctx *exec_ctx,
                                 grpc_lb_policy *policy,
                                 uint32_t initial_metadata_flags_mask,
                                 uint32_t initial_metadata_flags_eq);

/** Try to enter a READY connectivity state */
void grpc_lb_policy_exit_idle(grpc_exec_ctx *exec_ctx, grpc_lb_policy *policy);

/* Call notify when the connectivity state of a channel changes from \a *state.
 * Updates \a *state with the new state of the policy */
void grpc_lb_policy_notify_on_state_change(grpc_exec_ctx *exec_ctx,
                                           grpc_lb_policy *policy,
                                           grpc_connectivity_state *state,
                                           grpc_closure *closure);

grpc_connectivity_state grpc_lb_policy_check_connectivity(
    grpc_exec_ctx *exec_ctx, grpc_lb_policy *policy,
    grpc_error **connectivity_error);

#endif /* GRPC_CORE_EXT_CLIENT_CONFIG_LB_POLICY_H */
