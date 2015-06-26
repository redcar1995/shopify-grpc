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

#include "src/core/client_config/lb_policies/pick_first.h"

#include <string.h>

#include <grpc/support/alloc.h>

typedef struct pending_pick {
  struct pending_pick *next;
  grpc_pollset *pollset;
  grpc_subchannel **target;
  grpc_iomgr_closure *on_complete;
} pending_pick;

typedef struct {
  /** base policy: must be first */
  grpc_lb_policy base;
  /** ref count */
  gpr_refcount refs;
  /** all our subchannels */
  grpc_subchannel **subchannels;
  size_t num_subchannels;

  grpc_iomgr_closure connectivity_changed;

  /** mutex protecting remaining members */
  gpr_mu mu;
  /** the selected channel
      TODO(ctiller): this should be atomically set so we don't
                     need to take a mutex in the common case */
  grpc_subchannel *selected;
  /** have we started picking? */
  int started_picking;
  /** which subchannel are we watching? */
  size_t checking_subchannel;
  /** what is the connectivity of that channel? */
  grpc_connectivity_state checking_connectivity;
  /** list of picks that are waiting on connectivity */
  pending_pick *pending_picks;
} pick_first_lb_policy;

void pf_ref(grpc_lb_policy *pol) {
  pick_first_lb_policy *p = (pick_first_lb_policy *)pol;
  gpr_ref(&p->refs);
}

void pf_unref(grpc_lb_policy *pol) {
  pick_first_lb_policy *p = (pick_first_lb_policy *)pol;
  if (gpr_unref(&p->refs)) {
    gpr_free(p->subchannels);
    gpr_mu_destroy(&p->mu);
    gpr_free(p);
  }
}

void pf_shutdown(grpc_lb_policy *pol) {
  /*	pick_first_lb_policy *p = (pick_first_lb_policy*)pol; */
  abort();
}

void pf_pick(grpc_lb_policy *pol, grpc_pollset *pollset,
             grpc_metadata_batch *initial_metadata, grpc_subchannel **target,
             grpc_iomgr_closure *on_complete) {
  pick_first_lb_policy *p = (pick_first_lb_policy*)pol;
  pending_pick *pp;
  gpr_mu_lock(&p->mu);
  if (p->selected) {
    gpr_mu_unlock(&p->mu);
    *target = p->selected;
    on_complete->cb(on_complete->cb_arg, 1);
  } else {
    if (!p->started_picking) {
      p->started_picking = 1;
      p->checking_subchannel = 0;
      p->checking_connectivity = GRPC_CHANNEL_IDLE;
      pf_ref(pol);
      grpc_subchannel_notify_on_state_change(p->subchannels[0], &p->checking_connectivity, &p->connectivity_changed);
    }
    grpc_subchannel_add_interested_party(p->subchannels[p->checking_subchannel], pollset);
    pp = gpr_malloc(sizeof(*pp));
    pp->next = p->pending_picks;
    pp->pollset = pollset;
    pp->target = target;
    pp->on_complete = on_complete;
    p->pending_picks = pp;
    gpr_mu_unlock(&p->mu);
  }
}

static void del_interested_parties_locked(pick_first_lb_policy *p) {
  pending_pick *pp;
  for (pp = p->pending_picks; pp; pp = pp->next) {
    grpc_subchannel_del_interested_party(p->subchannels[p->checking_subchannel], pp->pollset);
  }
}

static void add_interested_parties_locked(pick_first_lb_policy *p) {
  pending_pick *pp;
  for (pp = p->pending_picks; pp; pp = pp->next) {
    grpc_subchannel_add_interested_party(p->subchannels[p->checking_subchannel], pp->pollset);
  }
}

static void pf_connectivity_changed(void *arg, int iomgr_success) {
  pick_first_lb_policy *p = arg;
  pending_pick *pp;
  int unref = 0;

  gpr_mu_lock(&p->mu);
loop:
  switch (p->checking_connectivity) {
    case GRPC_CHANNEL_READY:
      p->selected = p->subchannels[p->checking_connectivity];
      while ((pp = p->pending_picks)) {
        p->pending_picks = pp->next;
        *pp->target = p->selected;
        grpc_subchannel_del_interested_party(p->selected, pp->pollset);
        grpc_iomgr_add_delayed_callback(pp->on_complete, 1);
        gpr_free(pp);
      }
      unref = 1;
      break;
    case GRPC_CHANNEL_TRANSIENT_FAILURE:
      del_interested_parties_locked(p);
      p->checking_subchannel = (p->checking_subchannel + 1) % p->num_subchannels;
      p->checking_connectivity = grpc_subchannel_check_connectivity(p->subchannels[p->checking_subchannel]);
      add_interested_parties_locked(p);
      goto loop;
    case GRPC_CHANNEL_CONNECTING:
    case GRPC_CHANNEL_IDLE:
      grpc_subchannel_notify_on_state_change(p->subchannels[p->checking_subchannel], &p->checking_connectivity, &p->connectivity_changed);
      break;
    case GRPC_CHANNEL_FATAL_FAILURE:
      del_interested_parties_locked(p);
      GPR_SWAP(grpc_subchannel *, p->subchannels[p->checking_subchannel], p->subchannels[p->num_subchannels - 1]);
      p->checking_subchannel %= p->num_subchannels;
      p->checking_connectivity = grpc_subchannel_check_connectivity(p->subchannels[p->checking_subchannel]);
      p->num_subchannels--;
      grpc_subchannel_unref(p->subchannels[p->num_subchannels]);
      add_interested_parties_locked(p);
      if (p->num_subchannels == 0) {
        abort();
      } else {
        goto loop;
      }
  }
  gpr_mu_unlock(&p->mu);

  if (unref) {
    pf_unref(&p->base);
  }
}

static const grpc_lb_policy_vtable pick_first_lb_policy_vtable = {
    pf_ref, pf_unref, pf_shutdown, pf_pick};

grpc_lb_policy *grpc_create_pick_first_lb_policy(grpc_subchannel **subchannels,
                                                 size_t num_subchannels) {
  pick_first_lb_policy *p = gpr_malloc(sizeof(*p));
  GPR_ASSERT(num_subchannels);
  memset(p, 0, sizeof(*p));
  p->base.vtable = &pick_first_lb_policy_vtable;
  gpr_ref_init(&p->refs, 1);
  p->subchannels = gpr_malloc(sizeof(grpc_subchannel *) * num_subchannels);
  p->num_subchannels = num_subchannels;
  memcpy(p->subchannels, subchannels,
         sizeof(grpc_subchannel *) * num_subchannels);
  grpc_iomgr_closure_init(&p->connectivity_changed, pf_connectivity_changed, p);
  gpr_mu_init(&p->mu);
  return &p->base;
}
