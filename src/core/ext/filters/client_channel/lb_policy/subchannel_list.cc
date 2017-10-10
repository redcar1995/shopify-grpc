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

#include <string.h>

#include <grpc/support/alloc.h>

#include "src/core/ext/filters/client_channel/lb_policy/subchannel_list.h"
#include "src/core/lib/channel/channel_args.h"
#include "src/core/lib/debug/trace.h"
#include "src/core/lib/iomgr/closure.h"
#include "src/core/lib/iomgr/combiner.h"
#include "src/core/lib/iomgr/sockaddr_utils.h"
#include "src/core/lib/transport/connectivity_state.h"

extern grpc_tracer_flag grpc_lb_round_robin_trace;
extern grpc_tracer_flag grpc_lb_pick_first_trace;

void grpc_lb_subchannel_data_unref_subchannel(grpc_exec_ctx *exec_ctx,
                                              grpc_lb_subchannel_data *sd,
                                              const char *reason) {
  if (sd->subchannel != NULL) {
    if (GRPC_TRACER_ON(grpc_lb_round_robin_trace) ||
        GRPC_TRACER_ON(grpc_lb_pick_first_trace)) {
      gpr_log(GPR_DEBUG,
              "[LB %p] subchannel list %p index %" PRIdPTR " of %" PRIdPTR
              " (subchannel %p): unreffing subchannel",
              sd->subchannel_list->policy, sd->subchannel_list,
              (size_t)(sd - sd->subchannel_list->subchannels),
              sd->subchannel_list->num_subchannels, sd->subchannel);
    }
    GRPC_SUBCHANNEL_UNREF(exec_ctx, sd->subchannel, reason);
    sd->subchannel = NULL;
    if (sd->connected_subchannel != NULL) {
      GRPC_CONNECTED_SUBCHANNEL_UNREF(exec_ctx, sd->connected_subchannel,
                                      reason);
      sd->connected_subchannel = NULL;
    }
    if (sd->user_data != NULL) {
      GPR_ASSERT(sd->user_data_vtable != NULL);
      sd->user_data_vtable->destroy(exec_ctx, sd->user_data);
      sd->user_data = NULL;
    }
  }
}

void grpc_lb_subchannel_data_start_connectivity_watch(
    grpc_exec_ctx *exec_ctx, grpc_lb_subchannel_data *sd) {
  if (GRPC_TRACER_ON(grpc_lb_round_robin_trace) ||
      GRPC_TRACER_ON(grpc_lb_pick_first_trace)) {
    gpr_log(GPR_DEBUG,
            "[LB %p] subchannel list %p index %" PRIdPTR " of %" PRIdPTR
            " (subchannel %p): requesting connectivity change notification",
            sd->subchannel_list->policy, sd->subchannel_list,
            (size_t)(sd - sd->subchannel_list->subchannels),
            sd->subchannel_list->num_subchannels, sd->subchannel);
  }
  sd->connectivity_notification_pending = true;
  grpc_subchannel_notify_on_state_change(
      exec_ctx, sd->subchannel, sd->subchannel_list->policy->interested_parties,
      &sd->pending_connectivity_state_unsafe,
      &sd->connectivity_changed_closure);
}

void grpc_lb_subchannel_data_stop_connectivity_watch(
    grpc_exec_ctx *exec_ctx, grpc_lb_subchannel_data *sd) {
  if (GRPC_TRACER_ON(grpc_lb_round_robin_trace) ||
      GRPC_TRACER_ON(grpc_lb_pick_first_trace)) {
    gpr_log(GPR_DEBUG,
            "[LB %p] subchannel list %p index %" PRIdPTR " of %" PRIdPTR
            " (subchannel %p): stopping connectivity watch",
            sd->subchannel_list->policy, sd->subchannel_list,
            (size_t)(sd - sd->subchannel_list->subchannels),
            sd->subchannel_list->num_subchannels, sd->subchannel);
  }
  GPR_ASSERT(sd->connectivity_notification_pending);
  sd->connectivity_notification_pending = false;
}

grpc_lb_subchannel_list *grpc_lb_subchannel_list_create(
    grpc_exec_ctx *exec_ctx, grpc_lb_policy *p,
    const grpc_lb_addresses *addresses, const grpc_lb_policy_args *args,
    grpc_iomgr_cb_func connectivity_changed_cb) {
  grpc_lb_subchannel_list *subchannel_list =
      (grpc_lb_subchannel_list *)gpr_zalloc(sizeof(*subchannel_list));
  if (GRPC_TRACER_ON(grpc_lb_round_robin_trace) ||
      GRPC_TRACER_ON(grpc_lb_pick_first_trace)) {
    gpr_log(GPR_DEBUG,
            "[LB %p] Creating subchannel list %p for %" PRIdPTR " subchannels",
            p, subchannel_list, addresses->num_addresses);
  }
  subchannel_list->policy = p;
  gpr_ref_init(&subchannel_list->refcount, 1);
  subchannel_list->subchannels = (grpc_lb_subchannel_data *)gpr_zalloc(
      sizeof(grpc_lb_subchannel_data) * addresses->num_addresses);
  /* We need to remove the LB addresses in order to be able to compare the
   * subchannel keys of subchannels from a different batch of addresses. */
  static const char *keys_to_remove[] = {GRPC_ARG_SUBCHANNEL_ADDRESS,
                                         GRPC_ARG_LB_ADDRESSES};
  /* Create subchannels for addresses in the update. */
  grpc_subchannel_args sc_args;
  size_t subchannel_index = 0;
  for (size_t i = 0; i < addresses->num_addresses; i++) {
    // If there were any balancer, we would have chosen grpclb policy instead.
    GPR_ASSERT(!addresses->addresses[i].is_balancer);
    memset(&sc_args, 0, sizeof(grpc_subchannel_args));
    grpc_arg addr_arg =
        grpc_create_subchannel_address_arg(&addresses->addresses[i].address);
    grpc_channel_args *new_args = grpc_channel_args_copy_and_add_and_remove(
        args->args, keys_to_remove, GPR_ARRAY_SIZE(keys_to_remove), &addr_arg,
        1);
    gpr_free(addr_arg.value.string);
    sc_args.args = new_args;
    grpc_subchannel *subchannel = grpc_client_channel_factory_create_subchannel(
        exec_ctx, args->client_channel_factory, &sc_args);
    grpc_channel_args_destroy(exec_ctx, new_args);
    grpc_error *error;
    // Get the connectivity state of the subchannel. Already existing ones may
    // be in a state other than INIT.
    const grpc_connectivity_state subchannel_connectivity_state =
        grpc_subchannel_check_connectivity(subchannel, &error);
    if (error != GRPC_ERROR_NONE) {
      // The subchannel is in error (e.g. shutting down). Ignore it.
      if (GRPC_TRACER_ON(grpc_lb_round_robin_trace) ||
          GRPC_TRACER_ON(grpc_lb_pick_first_trace)) {
        char *address_uri =
            grpc_sockaddr_to_uri(&addresses->addresses[i].address);
        gpr_log(GPR_DEBUG,
                "[LB %p] subchannel for address uri %s shutting down, ignoring",
                subchannel_list->policy, address_uri);
        gpr_free(address_uri);
      }
      GRPC_SUBCHANNEL_UNREF(exec_ctx, subchannel, "new_sc_connectivity_error");
      GRPC_ERROR_UNREF(error);
      continue;
    }
    if (GRPC_TRACER_ON(grpc_lb_round_robin_trace) ||
        GRPC_TRACER_ON(grpc_lb_pick_first_trace)) {
      char *address_uri =
          grpc_sockaddr_to_uri(&addresses->addresses[i].address);
      gpr_log(GPR_DEBUG, "[LB %p] subchannel list %p index %" PRIdPTR
                         ": Created subchannel %p for address uri %s; "
                         "initial connectivity state: %s",
              p, subchannel_list, subchannel_index, subchannel, address_uri,
              grpc_connectivity_state_name(subchannel_connectivity_state));
      gpr_free(address_uri);
    }
    grpc_lb_subchannel_data *sd =
        &subchannel_list->subchannels[subchannel_index++];
    sd->subchannel_list = subchannel_list;
    sd->subchannel = subchannel;
    GRPC_CLOSURE_INIT(&sd->connectivity_changed_closure,
                      connectivity_changed_cb, sd,
                      grpc_combiner_scheduler(args->combiner));
    /* use some sentinel value outside of the range of
     * grpc_connectivity_state to signal an undefined previous state. We
     * won't be referring to this value again and it'll be overwritten after
     * the first call to rr_connectivity_changed_locked */
    sd->prev_connectivity_state = GRPC_CHANNEL_INIT;
    sd->curr_connectivity_state = subchannel_connectivity_state;
    sd->user_data_vtable = addresses->user_data_vtable;
    if (sd->user_data_vtable != NULL) {
      sd->user_data =
          sd->user_data_vtable->copy(addresses->addresses[i].user_data);
    }
  }
  subchannel_list->num_subchannels = subchannel_index;
  return subchannel_list;
}

static void subchannel_list_destroy(grpc_exec_ctx *exec_ctx,
                                    grpc_lb_subchannel_list *subchannel_list) {
  if (GRPC_TRACER_ON(grpc_lb_round_robin_trace) ||
      GRPC_TRACER_ON(grpc_lb_pick_first_trace)) {
    gpr_log(GPR_DEBUG, "[LB %p] Destroying subchannel_list %p",
            subchannel_list->policy, subchannel_list);
  }
  for (size_t i = 0; i < subchannel_list->num_subchannels; i++) {
    grpc_lb_subchannel_data *sd = &subchannel_list->subchannels[i];
    grpc_lb_subchannel_data_unref_subchannel(exec_ctx, sd,
                                             "subchannel_list_destroy");
  }
  gpr_free(subchannel_list->subchannels);
  gpr_free(subchannel_list);
}

void grpc_lb_subchannel_list_ref(grpc_lb_subchannel_list *subchannel_list,
                                 const char *reason) {
  gpr_ref_non_zero(&subchannel_list->refcount);
  if (GRPC_TRACER_ON(grpc_lb_round_robin_trace) ||
      GRPC_TRACER_ON(grpc_lb_pick_first_trace)) {
    const gpr_atm count = gpr_atm_acq_load(&subchannel_list->refcount.count);
    gpr_log(GPR_DEBUG, "[LB %p] subchannel_list %p REF %lu->%lu (%s)",
            subchannel_list->policy, subchannel_list,
            (unsigned long)(count - 1), (unsigned long)count, reason);
  }
}

void grpc_lb_subchannel_list_unref(grpc_exec_ctx *exec_ctx,
                                   grpc_lb_subchannel_list *subchannel_list,
                                   const char *reason) {
  const bool done = gpr_unref(&subchannel_list->refcount);
  if (GRPC_TRACER_ON(grpc_lb_round_robin_trace) ||
      GRPC_TRACER_ON(grpc_lb_pick_first_trace)) {
    const gpr_atm count = gpr_atm_acq_load(&subchannel_list->refcount.count);
    gpr_log(GPR_DEBUG, "[LB %p] subchannel_list %p UNREF %lu->%lu (%s)",
            subchannel_list->policy, subchannel_list,
            (unsigned long)(count + 1), (unsigned long)count, reason);
  }
  if (done) {
    subchannel_list_destroy(exec_ctx, subchannel_list);
  }
}

void grpc_lb_subchannel_list_ref_for_connectivity_watch(
    grpc_lb_subchannel_list *subchannel_list, const char *reason) {
  GRPC_LB_POLICY_WEAK_REF(subchannel_list->policy, reason);
  grpc_lb_subchannel_list_ref(subchannel_list, reason);
}

void grpc_lb_subchannel_list_unref_for_connectivity_watch(
    grpc_exec_ctx *exec_ctx, grpc_lb_subchannel_list *subchannel_list,
    const char *reason) {
  GRPC_LB_POLICY_WEAK_UNREF(exec_ctx, subchannel_list->policy, reason);
  grpc_lb_subchannel_list_unref(exec_ctx, subchannel_list, reason);
}

static void grpc_lb_subchannel_data_cancel_connectivity_watch(
    grpc_exec_ctx *exec_ctx, grpc_lb_subchannel_data *sd, const char *reason) {
  if (GRPC_TRACER_ON(grpc_lb_round_robin_trace) ||
      GRPC_TRACER_ON(grpc_lb_pick_first_trace)) {
    gpr_log(GPR_DEBUG,
            "[LB %p] subchannel list %p index %" PRIdPTR " of %" PRIdPTR
            " (subchannel %p): canceling connectivity watch (%s)",
            sd->subchannel_list->policy, sd->subchannel_list,
            (size_t)(sd - sd->subchannel_list->subchannels),
            sd->subchannel_list->num_subchannels, sd->subchannel, reason);
  }
  grpc_subchannel_notify_on_state_change(exec_ctx, sd->subchannel, NULL, NULL,
                                         &sd->connectivity_changed_closure);
}

void grpc_lb_subchannel_list_shutdown_and_unref(
    grpc_exec_ctx *exec_ctx, grpc_lb_subchannel_list *subchannel_list,
    const char *reason) {
  if (GRPC_TRACER_ON(grpc_lb_round_robin_trace) ||
      GRPC_TRACER_ON(grpc_lb_pick_first_trace)) {
    gpr_log(GPR_DEBUG, "[LB %p] Shutting down subchannel_list %p (%s)",
            subchannel_list->policy, subchannel_list, reason);
  }
  GPR_ASSERT(!subchannel_list->shutting_down);
  subchannel_list->shutting_down = true;
  for (size_t i = 0; i < subchannel_list->num_subchannels; i++) {
    grpc_lb_subchannel_data *sd = &subchannel_list->subchannels[i];
    // If there's a pending notification for this subchannel, cancel it;
    // the callback is responsible for unreffing the subchannel.
    // Otherwise, unref the subchannel directly.
    if (sd->connectivity_notification_pending) {
      grpc_lb_subchannel_data_cancel_connectivity_watch(exec_ctx, sd, reason);
    } else if (sd->subchannel != NULL) {
      grpc_lb_subchannel_data_unref_subchannel(exec_ctx, sd, reason);
    }
  }
  grpc_lb_subchannel_list_unref(exec_ctx, subchannel_list, reason);
}
