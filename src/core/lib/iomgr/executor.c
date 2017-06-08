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

#include "src/core/lib/iomgr/executor.h"

#include <string.h>

#include <grpc/support/alloc.h>
#include <grpc/support/cpu.h>
#include <grpc/support/log.h>
#include <grpc/support/sync.h>
#include <grpc/support/thd.h>
#include <grpc/support/tls.h>
#include <grpc/support/useful.h>

#include "src/core/lib/iomgr/exec_ctx.h"
#include "src/core/lib/support/spinlock.h"

#define MAX_DEPTH 2

typedef struct {
  gpr_mu mu;
  gpr_cv cv;
  grpc_closure_list elems;
  size_t depth;
  bool shutdown;
  gpr_thd_id id;
} thread_state;

static thread_state *g_thread_state;
static size_t g_max_threads;
static gpr_atm g_cur_threads;
static gpr_spinlock g_adding_thread_lock = GPR_SPINLOCK_STATIC_INITIALIZER;

GPR_TLS_DECL(g_this_thread_state);

static void executor_thread(void *arg);

static size_t run_closures(grpc_exec_ctx *exec_ctx, grpc_closure_list list) {
  size_t n = 0;

  grpc_closure *c = list.head;
  while (c != NULL) {
    grpc_closure *next = c->next_data.next;
    grpc_error *error = c->error_data.error;
#ifndef NDEBUG
    c->scheduled = false;
#endif
    c->cb(exec_ctx, c->cb_arg, error);
    GRPC_ERROR_UNREF(error);
    c = next;
  }

  return n;
}

bool grpc_executor_is_threaded() {
  return gpr_atm_no_barrier_load(&g_cur_threads) > 0;
}

void grpc_executor_set_threading(grpc_exec_ctx *exec_ctx, bool threading) {
  gpr_atm cur_threads = gpr_atm_no_barrier_load(&g_cur_threads);
  if (threading) {
    if (cur_threads > 0) return;
    g_max_threads = GPR_MAX(1, 2 * gpr_cpu_num_cores());
    gpr_atm_no_barrier_store(&g_cur_threads, 1);
    gpr_tls_init(&g_this_thread_state);
    g_thread_state = gpr_zalloc(sizeof(thread_state) * g_max_threads);
    for (size_t i = 0; i < g_max_threads; i++) {
      gpr_mu_init(&g_thread_state[i].mu);
      gpr_cv_init(&g_thread_state[i].cv);
      g_thread_state[i].elems = (grpc_closure_list)GRPC_CLOSURE_LIST_INIT;
    }

    gpr_thd_options opt = gpr_thd_options_default();
    gpr_thd_options_set_joinable(&opt);
    gpr_thd_new(&g_thread_state[0].id, executor_thread, &g_thread_state[0],
                &opt);
  } else {
    if (cur_threads == 0) return;
    for (size_t i = 0; i < g_max_threads; i++) {
      gpr_mu_lock(&g_thread_state[i].mu);
      g_thread_state[i].shutdown = true;
      gpr_cv_signal(&g_thread_state[i].cv);
      gpr_mu_unlock(&g_thread_state[i].mu);
    }
    /* ensure no thread is adding a new thread... once this is past, then
       no thread will try to add a new one either (since shutdown is true) */
    gpr_spinlock_lock(&g_adding_thread_lock);
    gpr_spinlock_unlock(&g_adding_thread_lock);
    for (gpr_atm i = 0; i < g_cur_threads; i++) {
      gpr_thd_join(g_thread_state[i].id);
    }
    gpr_atm_no_barrier_store(&g_cur_threads, 0);
    for (size_t i = 0; i < g_max_threads; i++) {
      gpr_mu_destroy(&g_thread_state[i].mu);
      gpr_cv_destroy(&g_thread_state[i].cv);
      run_closures(exec_ctx, g_thread_state[i].elems);
    }
    gpr_free(g_thread_state);
    gpr_tls_destroy(&g_this_thread_state);
  }
}

void grpc_executor_init(grpc_exec_ctx *exec_ctx) {
  gpr_atm_no_barrier_store(&g_cur_threads, 0);
  grpc_executor_set_threading(exec_ctx, true);
}

void grpc_executor_shutdown(grpc_exec_ctx *exec_ctx) {
  grpc_executor_set_threading(exec_ctx, false);
}

static void executor_thread(void *arg) {
  thread_state *ts = arg;
  gpr_tls_set(&g_this_thread_state, (intptr_t)ts);

  grpc_exec_ctx exec_ctx =
      GRPC_EXEC_CTX_INITIALIZER(0, grpc_never_ready_to_finish, NULL);

  size_t subtract_depth = 0;
  for (;;) {
    gpr_mu_lock(&ts->mu);
    ts->depth -= subtract_depth;
    while (grpc_closure_list_empty(ts->elems) && !ts->shutdown) {
      gpr_cv_wait(&ts->cv, &ts->mu, gpr_inf_future(GPR_CLOCK_REALTIME));
    }
    if (ts->shutdown) {
      gpr_mu_unlock(&ts->mu);
      break;
    }
    grpc_closure_list exec = ts->elems;
    ts->elems = (grpc_closure_list)GRPC_CLOSURE_LIST_INIT;
    gpr_mu_unlock(&ts->mu);

    subtract_depth = run_closures(&exec_ctx, exec);
    grpc_exec_ctx_flush(&exec_ctx);
  }
  grpc_exec_ctx_finish(&exec_ctx);
}

static void executor_push(grpc_exec_ctx *exec_ctx, grpc_closure *closure,
                          grpc_error *error) {
  size_t cur_thread_count = (size_t)gpr_atm_no_barrier_load(&g_cur_threads);
  if (cur_thread_count == 0) {
    grpc_closure_list_append(&exec_ctx->closure_list, closure, error);
    return;
  }
  thread_state *ts = (thread_state *)gpr_tls_get(&g_this_thread_state);
  if (ts == NULL) {
    ts = &g_thread_state[GPR_HASH_POINTER(exec_ctx, cur_thread_count)];
  }
  gpr_mu_lock(&ts->mu);
  if (grpc_closure_list_empty(ts->elems)) {
    gpr_cv_signal(&ts->cv);
  }
  grpc_closure_list_append(&ts->elems, closure, error);
  ts->depth++;
  bool try_new_thread = ts->depth > MAX_DEPTH &&
                        cur_thread_count < g_max_threads && !ts->shutdown;
  gpr_mu_unlock(&ts->mu);
  if (try_new_thread && gpr_spinlock_trylock(&g_adding_thread_lock)) {
    cur_thread_count = (size_t)gpr_atm_no_barrier_load(&g_cur_threads);
    if (cur_thread_count < g_max_threads) {
      gpr_atm_no_barrier_store(&g_cur_threads, cur_thread_count + 1);

      gpr_thd_options opt = gpr_thd_options_default();
      gpr_thd_options_set_joinable(&opt);
      gpr_thd_new(&g_thread_state[cur_thread_count].id, executor_thread,
                  &g_thread_state[cur_thread_count], &opt);
    }
    gpr_spinlock_unlock(&g_adding_thread_lock);
  }
}

static const grpc_closure_scheduler_vtable executor_vtable = {
    executor_push, executor_push, "executor"};
static grpc_closure_scheduler executor_scheduler = {&executor_vtable};
grpc_closure_scheduler *grpc_executor_scheduler = &executor_scheduler;
