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

#include "src/core/iomgr/iomgr.h"

#include <assert.h>
#include <stdlib.h>

#include "src/core/iomgr/iomgr_internal.h"
#include "src/core/iomgr/alarm_internal.h"
#include <grpc/support/alloc.h>
#include <grpc/support/log.h>
#include <grpc/support/thd.h>
#include <grpc/support/sync.h>

static gpr_mu g_mu;
static gpr_cv g_rcv;
static grpc_iomgr_closure *g_cbs_head = NULL;
static grpc_iomgr_closure *g_cbs_tail = NULL;
static int g_shutdown;
static int g_refs;
static gpr_event g_background_callback_executor_done;

/* Execute followup callbacks continuously.
   Other threads may check in and help during pollset_work() */
static void background_callback_executor(void *ignored) {
  gpr_mu_lock(&g_mu);
  while (!g_shutdown) {
    gpr_timespec deadline = gpr_inf_future;
    gpr_timespec short_deadline =
        gpr_time_add(gpr_now(), gpr_time_from_millis(100));
    if (g_cbs_head) {
      grpc_iomgr_closure *closure = g_cbs_head;
      g_cbs_head = closure->next;
      if (!g_cbs_head) g_cbs_tail = NULL;
      gpr_mu_unlock(&g_mu);
      assert(closure->success >= 0);
      closure->cb(closure->cb_arg, closure->success);
      gpr_mu_lock(&g_mu);
    } else if (grpc_alarm_check(&g_mu, gpr_now(), &deadline)) {
    } else {
      gpr_mu_unlock(&g_mu);
      gpr_sleep_until(gpr_time_min(short_deadline, deadline));
      gpr_mu_lock(&g_mu);
    }
  }
  gpr_mu_unlock(&g_mu);
  gpr_event_set(&g_background_callback_executor_done, (void *)1);
}

void grpc_kick_poller(void) {
  /* Empty. The background callback executor polls periodically. The activity
   * the kicker is trying to draw the executor's attention to will be picked up
   * either by one of the periodic wakeups or by one of the polling application
   * threads. */
}

void grpc_iomgr_init(void) {
  gpr_thd_id id;
  gpr_mu_init(&g_mu);
  gpr_cv_init(&g_rcv);
  grpc_alarm_list_init(gpr_now());
  g_refs = 0;
  grpc_iomgr_platform_init();
  gpr_event_init(&g_background_callback_executor_done);
  gpr_thd_new(&id, background_callback_executor, NULL, NULL);
}

void grpc_iomgr_shutdown(void) {
  grpc_iomgr_closure *closure;
  gpr_timespec shutdown_deadline =
      gpr_time_add(gpr_now(), gpr_time_from_seconds(10));


  gpr_mu_lock(&g_mu);
  g_shutdown = 1;
  while (g_cbs_head || g_refs) {
    gpr_log(GPR_DEBUG, "Waiting for %d iomgr objects to be destroyed%s", g_refs,
            g_cbs_head ? " and executing final callbacks" : "");
    while (g_cbs_head) {
      closure = g_cbs_head;
      g_cbs_head = closure->next;
      if (!g_cbs_head) g_cbs_tail = NULL;
      gpr_mu_unlock(&g_mu);

      closure->cb(closure->cb_arg, 0);
      gpr_mu_lock(&g_mu);
    }
    if (g_refs) {
      int timeout = 0;
      gpr_timespec short_deadline = gpr_time_add(gpr_now(),
                                                 gpr_time_from_millis(100));
      while (gpr_cv_wait(&g_rcv, &g_mu, short_deadline) && g_cbs_head == NULL) {
        if (gpr_time_cmp(gpr_now(), shutdown_deadline) > 0) {
          timeout = 1;
          break;
        }
      }
      if (timeout) {
        gpr_log(GPR_DEBUG,
                "Failed to free %d iomgr objects before shutdown deadline: "
                "memory leaks are likely",
                g_refs);
        break;
      }
    }
  }
  gpr_mu_unlock(&g_mu);

  grpc_kick_poller();
  gpr_event_wait(&g_background_callback_executor_done, gpr_inf_future);

  grpc_iomgr_platform_shutdown();
  grpc_alarm_list_shutdown();
  gpr_mu_destroy(&g_mu);
  gpr_cv_destroy(&g_rcv);
}

void grpc_iomgr_ref(void) {
  gpr_mu_lock(&g_mu);
  ++g_refs;
  gpr_mu_unlock(&g_mu);
}

void grpc_iomgr_unref(void) {
  gpr_mu_lock(&g_mu);
  if (0 == --g_refs) {
    gpr_cv_signal(&g_rcv);
  }
  gpr_mu_unlock(&g_mu);
}


void grpc_iomgr_closure_init(grpc_iomgr_closure *closure, grpc_iomgr_cb_func cb,
                             void *cb_arg) {
  closure->cb = cb;
  closure->cb_arg = cb_arg;
  closure->success = -1;  /* uninitialized */
  closure->next = NULL;
}

typedef struct {
  grpc_iomgr_closure managed;
  grpc_iomgr_closure *manager;
} managed_closure_arg;

static void closure_manager_func(void *arg, int success) {
  managed_closure_arg *mc_arg = (managed_closure_arg*) arg;

  mc_arg->managed.cb(mc_arg->managed.cb_arg, success);
  gpr_free(mc_arg->manager);
  gpr_free(mc_arg);
}

void grpc_iomgr_managed_closure_init(grpc_iomgr_closure *manager,
                                     grpc_iomgr_cb_func managed_cb,
                                     void *managed_cb_arg) {
    managed_closure_arg *managed_arg = gpr_malloc(sizeof(managed_closure_arg));

    managed_arg->managed.cb = managed_cb;
    managed_arg->managed.cb_arg = managed_cb_arg;
    managed_arg->manager= manager;

    grpc_iomgr_closure_init(manager, closure_manager_func, managed_arg);
}


void grpc_iomgr_add_delayed_callback(grpc_iomgr_closure *closure, int success) {
  closure->success = success;
  gpr_mu_lock(&g_mu);
  closure->next = NULL;
  if (!g_cbs_tail) {
    g_cbs_head = g_cbs_tail = closure;
  } else {
    g_cbs_tail->next = closure;
    g_cbs_tail = closure;
  }
  gpr_mu_unlock(&g_mu);
}


void grpc_iomgr_add_callback(grpc_iomgr_closure *closure) {
  grpc_iomgr_add_delayed_callback(closure, 1);
}


int grpc_maybe_call_delayed_callbacks(gpr_mu *drop_mu, int success) {
  int n = 0;
  gpr_mu *retake_mu = NULL;
  grpc_iomgr_closure *closure;
  for (;;) {
    /* check for new work */
    if (!gpr_mu_trylock(&g_mu)) {
      break;
    }
    closure = g_cbs_head;
    if (!closure) {
      gpr_mu_unlock(&g_mu);
      break;
    }
    g_cbs_head = closure->next;
    if (!g_cbs_head) g_cbs_tail = NULL;
    gpr_mu_unlock(&g_mu);
    /* if we have a mutex to drop, do so before executing work */
    if (drop_mu) {
      gpr_mu_unlock(drop_mu);
      retake_mu = drop_mu;
      drop_mu = NULL;
    }
    assert(closure->success >= 0);
    closure->cb(closure->cb_arg, success && closure->success);
    n++;
  }
  if (retake_mu) {
    gpr_mu_lock(retake_mu);
  }
  return n;
}
