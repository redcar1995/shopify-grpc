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

#include "src/core/lib/iomgr/port.h"

#ifdef GRPC_TIMER_USE_GENERIC

#include "src/core/lib/iomgr/timer.h"

#include <grpc/support/alloc.h>
#include <grpc/support/log.h>
#include <grpc/support/string_util.h>
#include <grpc/support/sync.h>
#include <grpc/support/tls.h>
#include <grpc/support/useful.h>
#include "src/core/lib/debug/trace.h"
#include "src/core/lib/iomgr/time_averaged_stats.h"
#include "src/core/lib/iomgr/timer_heap.h"
#include "src/core/lib/support/spinlock.h"

#define INVALID_HEAP_INDEX 0xffffffffu

#define LOG2_NUM_SHARDS 5
#define NUM_SHARDS (1 << LOG2_NUM_SHARDS)
#define ADD_DEADLINE_SCALE 0.33
#define MIN_QUEUE_WINDOW_DURATION 0.01
#define MAX_QUEUE_WINDOW_DURATION 1

static int grpc_timer_trace = 0;
static int grpc_timer_check_trace = 0;

typedef struct {
  gpr_mu mu;
  grpc_time_averaged_stats stats;
  /* All and only timers with deadlines <= this will be in the heap. */
  gpr_atm queue_deadline_cap;
  gpr_atm min_deadline;
  /* Index in the g_shard_queue */
  uint32_t shard_queue_index;
  /* This holds all timers with deadlines < queue_deadline_cap. Timers in this
     list have the top bit of their deadline set to 0. */
  grpc_timer_heap heap;
  /* This holds timers whose deadline is >= queue_deadline_cap. */
  grpc_timer list;
} shard_type;

struct shared_mutables {
  gpr_atm min_timer;
  /* Allow only one run_some_expired_timers at once */
  gpr_spinlock checker_mu;
  bool initialized;
  /* Protects g_shard_queue */
  gpr_mu mu;
} GPR_ALIGN_STRUCT(GPR_CACHELINE_SIZE);

static struct shared_mutables g_shared_mutables = {
    .checker_mu = GPR_SPINLOCK_STATIC_INITIALIZER, .initialized = false,
};
static gpr_clock_type g_clock_type;
static shard_type g_shards[NUM_SHARDS];
/* Protected by g_shared_mutables.mu */
static shard_type *g_shard_queue[NUM_SHARDS];
static gpr_timespec g_start_time;

GPR_TLS_DECL(g_last_seen_min_timer);

static gpr_atm saturating_add(gpr_atm a, gpr_atm b) {
  if (a > GPR_ATM_MAX - b) {
    return GPR_ATM_MAX;
  }
  return a + b;
}

static int run_some_expired_timers(grpc_exec_ctx *exec_ctx, gpr_atm now,
                                   gpr_atm *next, grpc_error *error);

static gpr_timespec dbl_to_ts(double d) {
  gpr_timespec ts;
  ts.tv_sec = (int64_t)d;
  ts.tv_nsec = (int32_t)(1e9 * (d - (double)ts.tv_sec));
  ts.clock_type = GPR_TIMESPAN;
  return ts;
}

static gpr_atm timespec_to_atm_round_up(gpr_timespec ts) {
  ts = gpr_time_sub(ts, g_start_time);
  double x = GPR_MS_PER_SEC * (double)ts.tv_sec +
             (double)ts.tv_nsec / GPR_NS_PER_MS + 1.0;
  if (x < 0) return 0;
  if (x > GPR_ATM_MAX) return GPR_ATM_MAX;
  return (gpr_atm)x;
}

static gpr_atm timespec_to_atm_round_down(gpr_timespec ts) {
  ts = gpr_time_sub(ts, g_start_time);
  double x =
      GPR_MS_PER_SEC * (double)ts.tv_sec + (double)ts.tv_nsec / GPR_NS_PER_MS;
  if (x < 0) return 0;
  if (x > GPR_ATM_MAX) return GPR_ATM_MAX;
  return (gpr_atm)x;
}

static gpr_timespec atm_to_timespec(gpr_atm x) {
  return gpr_time_add(g_start_time, dbl_to_ts((double)x / 1000.0));
}

static gpr_atm compute_min_deadline(shard_type *shard) {
  return grpc_timer_heap_is_empty(&shard->heap)
             ? shard->queue_deadline_cap
             : grpc_timer_heap_top(&shard->heap)->deadline;
}

void grpc_timer_list_init(gpr_timespec now) {
  uint32_t i;

  g_shared_mutables.initialized = true;
  gpr_mu_init(&g_shared_mutables.mu);
  g_clock_type = now.clock_type;
  g_start_time = now;
  g_shared_mutables.min_timer = timespec_to_atm_round_down(now);
  gpr_tls_init(&g_last_seen_min_timer);
  gpr_tls_set(&g_last_seen_min_timer, 0);
  grpc_register_tracer("timer", &grpc_timer_trace);
  grpc_register_tracer("timer_check", &grpc_timer_check_trace);

  for (i = 0; i < NUM_SHARDS; i++) {
    shard_type *shard = &g_shards[i];
    gpr_mu_init(&shard->mu);
    grpc_time_averaged_stats_init(&shard->stats, 1.0 / ADD_DEADLINE_SCALE, 0.1,
                                  0.5);
    shard->queue_deadline_cap = g_shared_mutables.min_timer;
    shard->shard_queue_index = i;
    grpc_timer_heap_init(&shard->heap);
    shard->list.next = shard->list.prev = &shard->list;
    shard->min_deadline = compute_min_deadline(shard);
    g_shard_queue[i] = shard;
  }
}

void grpc_timer_list_shutdown(grpc_exec_ctx *exec_ctx) {
  int i;
  run_some_expired_timers(
      exec_ctx, GPR_ATM_MAX, NULL,
      GRPC_ERROR_CREATE_FROM_STATIC_STRING("Timer list shutdown"));
  for (i = 0; i < NUM_SHARDS; i++) {
    shard_type *shard = &g_shards[i];
    gpr_mu_destroy(&shard->mu);
    grpc_timer_heap_destroy(&shard->heap);
  }
  gpr_mu_destroy(&g_shared_mutables.mu);
  gpr_tls_destroy(&g_last_seen_min_timer);
  g_shared_mutables.initialized = false;
}

static double ts_to_dbl(gpr_timespec ts) {
  return (double)ts.tv_sec + 1e-9 * ts.tv_nsec;
}

static void list_join(grpc_timer *head, grpc_timer *timer) {
  timer->next = head;
  timer->prev = head->prev;
  timer->next->prev = timer->prev->next = timer;
}

static void list_remove(grpc_timer *timer) {
  timer->next->prev = timer->prev;
  timer->prev->next = timer->next;
}

static void swap_adjacent_shards_in_queue(uint32_t first_shard_queue_index) {
  shard_type *temp;
  temp = g_shard_queue[first_shard_queue_index];
  g_shard_queue[first_shard_queue_index] =
      g_shard_queue[first_shard_queue_index + 1];
  g_shard_queue[first_shard_queue_index + 1] = temp;
  g_shard_queue[first_shard_queue_index]->shard_queue_index =
      first_shard_queue_index;
  g_shard_queue[first_shard_queue_index + 1]->shard_queue_index =
      first_shard_queue_index + 1;
}

static void note_deadline_change(shard_type *shard) {
  while (shard->shard_queue_index > 0 &&
         shard->min_deadline <
             g_shard_queue[shard->shard_queue_index - 1]->min_deadline) {
    swap_adjacent_shards_in_queue(shard->shard_queue_index - 1);
  }
  while (shard->shard_queue_index < NUM_SHARDS - 1 &&
         shard->min_deadline >
             g_shard_queue[shard->shard_queue_index + 1]->min_deadline) {
    swap_adjacent_shards_in_queue(shard->shard_queue_index);
  }
}

void grpc_timer_init(grpc_exec_ctx *exec_ctx, grpc_timer *timer,
                     gpr_timespec deadline, grpc_closure *closure,
                     gpr_timespec now) {
  int is_first_timer = 0;
  shard_type *shard = &g_shards[GPR_HASH_POINTER(timer, NUM_SHARDS)];
  GPR_ASSERT(deadline.clock_type == g_clock_type);
  GPR_ASSERT(now.clock_type == g_clock_type);
  timer->closure = closure;
  timer->deadline = timespec_to_atm_round_up(deadline);

  if (grpc_timer_trace) {
    gpr_log(GPR_DEBUG, "TIMER %p: SET %" PRIdPTR ".%09d [%" PRIdPTR
                       "] now %" PRIdPTR ".%09d [%" PRIdPTR "] call %p[%p]",
            timer, deadline.tv_sec, deadline.tv_nsec, timer->deadline,
            now.tv_sec, now.tv_nsec, timespec_to_atm_round_down(now), closure,
            closure->cb);
  }

  if (!g_shared_mutables.initialized) {
    timer->pending = false;
    grpc_closure_sched(exec_ctx, timer->closure,
                       GRPC_ERROR_CREATE_FROM_STATIC_STRING(
                           "Attempt to create timer before initialization"));
    return;
  }

  gpr_mu_lock(&shard->mu);
  timer->pending = true;
  if (gpr_time_cmp(deadline, now) <= 0) {
    timer->pending = false;
    grpc_closure_sched(exec_ctx, timer->closure, GRPC_ERROR_NONE);
    gpr_mu_unlock(&shard->mu);
    /* early out */
    return;
  }

  grpc_time_averaged_stats_add_sample(&shard->stats,
                                      ts_to_dbl(gpr_time_sub(deadline, now)));
  if (timer->deadline < shard->queue_deadline_cap) {
    is_first_timer = grpc_timer_heap_add(&shard->heap, timer);
  } else {
    timer->heap_index = INVALID_HEAP_INDEX;
    list_join(&shard->list, timer);
  }
  gpr_mu_unlock(&shard->mu);

  /* Deadline may have decreased, we need to adjust the master queue.  Note
     that there is a potential racy unlocked region here.  There could be a
     reordering of multiple grpc_timer_init calls, at this point, but the < test
     below should ensure that we err on the side of caution.  There could
     also be a race with grpc_timer_check, which might beat us to the lock.  In
     that case, it is possible that the timer that we added will have already
     run by the time we hold the lock, but that too is a safe error.
     Finally, it's possible that the grpc_timer_check that intervened failed to
     trigger the new timer because the min_deadline hadn't yet been reduced.
     In that case, the timer will simply have to wait for the next
     grpc_timer_check. */
  if (is_first_timer) {
    gpr_mu_lock(&g_shared_mutables.mu);
    if (timer->deadline < shard->min_deadline) {
      gpr_atm old_min_deadline = g_shard_queue[0]->min_deadline;
      shard->min_deadline = timer->deadline;
      note_deadline_change(shard);
      if (shard->shard_queue_index == 0 && timer->deadline < old_min_deadline) {
        gpr_atm_no_barrier_store(&g_shared_mutables.min_timer, timer->deadline);
        grpc_kick_poller();
      }
    }
    gpr_mu_unlock(&g_shared_mutables.mu);
  }
}

void grpc_timer_consume_kick(void) {
  /* force re-evaluation of last seeen min */
  gpr_tls_set(&g_last_seen_min_timer, 0);
}

void grpc_timer_cancel(grpc_exec_ctx *exec_ctx, grpc_timer *timer) {
  if (!g_shared_mutables.initialized) {
    /* must have already been cancelled, also the shard mutex is invalid */
    return;
  }

  shard_type *shard = &g_shards[GPR_HASH_POINTER(timer, NUM_SHARDS)];
  gpr_mu_lock(&shard->mu);
  if (timer->pending) {
    grpc_closure_sched(exec_ctx, timer->closure, GRPC_ERROR_CANCELLED);
    timer->pending = false;
    if (timer->heap_index == INVALID_HEAP_INDEX) {
      list_remove(timer);
    } else {
      grpc_timer_heap_remove(&shard->heap, timer);
    }
  }
  gpr_mu_unlock(&shard->mu);
}

/* This is called when the queue is empty and "now" has reached the
   queue_deadline_cap.  We compute a new queue deadline and then scan the map
   for timers that fall at or under it.  Returns true if the queue is no
   longer empty.
   REQUIRES: shard->mu locked */
static int refill_queue(shard_type *shard, gpr_atm now) {
  /* Compute the new queue window width and bound by the limits: */
  double computed_deadline_delta =
      grpc_time_averaged_stats_update_average(&shard->stats) *
      ADD_DEADLINE_SCALE;
  double deadline_delta =
      GPR_CLAMP(computed_deadline_delta, MIN_QUEUE_WINDOW_DURATION,
                MAX_QUEUE_WINDOW_DURATION);
  grpc_timer *timer, *next;

  /* Compute the new cap and put all timers under it into the queue: */
  shard->queue_deadline_cap =
      saturating_add(GPR_MAX(now, shard->queue_deadline_cap),
                     (gpr_atm)(deadline_delta * 1000.0));
  for (timer = shard->list.next; timer != &shard->list; timer = next) {
    next = timer->next;

    if (timer->deadline < shard->queue_deadline_cap) {
      list_remove(timer);
      grpc_timer_heap_add(&shard->heap, timer);
    }
  }
  return !grpc_timer_heap_is_empty(&shard->heap);
}

/* This pops the next non-cancelled timer with deadline <= now from the
   queue, or returns NULL if there isn't one.
   REQUIRES: shard->mu locked */
static grpc_timer *pop_one(shard_type *shard, gpr_atm now) {
  grpc_timer *timer;
  for (;;) {
    if (grpc_timer_heap_is_empty(&shard->heap)) {
      if (now < shard->queue_deadline_cap) return NULL;
      if (!refill_queue(shard, now)) return NULL;
    }
    timer = grpc_timer_heap_top(&shard->heap);
    if (timer->deadline >= now) return NULL;
    timer->pending = false;
    grpc_timer_heap_pop(&shard->heap);
    return timer;
  }
}

/* REQUIRES: shard->mu unlocked */
static size_t pop_timers(grpc_exec_ctx *exec_ctx, shard_type *shard,
                         gpr_atm now, gpr_atm *new_min_deadline,
                         grpc_error *error) {
  size_t n = 0;
  grpc_timer *timer;
  gpr_mu_lock(&shard->mu);
  while ((timer = pop_one(shard, now))) {
    grpc_closure_sched(exec_ctx, timer->closure, GRPC_ERROR_REF(error));
    n++;
  }
  *new_min_deadline = compute_min_deadline(shard);
  gpr_mu_unlock(&shard->mu);
  return n;
}

static int run_some_expired_timers(grpc_exec_ctx *exec_ctx, gpr_atm now,
                                   gpr_atm *next, grpc_error *error) {
  size_t n = 0;

  /* fetch from a thread-local first: this avoids contention on a globally
     mutable cacheline in the common case */
  gpr_atm min_timer = gpr_tls_get(&g_last_seen_min_timer);
  if (now < min_timer) {
    if (next != NULL) *next = GPR_MIN(*next, min_timer);
    return 0;
  }

  min_timer = gpr_atm_no_barrier_load(&g_shared_mutables.min_timer);
  gpr_tls_set(&g_last_seen_min_timer, min_timer);
  if (now < min_timer) {
    if (next != NULL) *next = GPR_MIN(*next, min_timer);
    return 0;
  }

  if (gpr_spinlock_trylock(&g_shared_mutables.checker_mu)) {
    gpr_mu_lock(&g_shared_mutables.mu);

    while (g_shard_queue[0]->min_deadline < now) {
      gpr_atm new_min_deadline;

      /* For efficiency, we pop as many available timers as we can from the
         shard.  This may violate perfect timer deadline ordering, but that
         shouldn't be a big deal because we don't make ordering guarantees. */
      n +=
          pop_timers(exec_ctx, g_shard_queue[0], now, &new_min_deadline, error);

      /* An grpc_timer_init() on the shard could intervene here, adding a new
         timer that is earlier than new_min_deadline.  However,
         grpc_timer_init() will block on the master_lock before it can call
         set_min_deadline, so this one will complete first and then the Addtimer
         will reduce the min_deadline (perhaps unnecessarily). */
      g_shard_queue[0]->min_deadline = new_min_deadline;
      note_deadline_change(g_shard_queue[0]);
    }

    if (next) {
      *next = GPR_MIN(*next, g_shard_queue[0]->min_deadline);
    }

    gpr_atm_no_barrier_store(&g_shared_mutables.min_timer,
                             g_shard_queue[0]->min_deadline);
    gpr_mu_unlock(&g_shared_mutables.mu);
    gpr_spinlock_unlock(&g_shared_mutables.checker_mu);
  }

  GRPC_ERROR_UNREF(error);

  return (int)n;
}

bool grpc_timer_check(grpc_exec_ctx *exec_ctx, gpr_timespec now,
                      gpr_timespec *next) {
  GPR_ASSERT(now.clock_type == g_clock_type);
  gpr_atm now_atm = timespec_to_atm_round_down(now);
  grpc_error *shutdown_error =
      gpr_time_cmp(now, gpr_inf_future(now.clock_type)) != 0
          ? GRPC_ERROR_NONE
<<<<<<< HEAD
          : GRPC_ERROR_CREATE("Shutting down timer system");
  if (grpc_timer_check_trace) {
    char *next_str;
    if (next == NULL) {
      next_str = gpr_strdup("NULL");
    } else {
      gpr_asprintf(&next_str, "%" PRIdPTR ".%09d [%" PRIdPTR "]", next->tv_sec,
                   next->tv_nsec, timespec_to_atm_round_down(*next));
    }
    gpr_log(GPR_DEBUG, "TIMER CHECK BEGIN: now=%" PRIdPTR ".%09d [%" PRIdPTR
                       "] next=%s tls_min=%" PRIdPTR " glob_min=%" PRIdPTR,
            now.tv_sec, now.tv_nsec, now_atm, next_str,
            gpr_tls_get(&g_last_seen_min_timer),
            gpr_atm_no_barrier_load(&g_shared_mutables.min_timer));
    gpr_free(next_str);
  }
  bool r;
  gpr_atm next_atm;
  if (next == NULL) {
    r = run_some_expired_timers(exec_ctx, now_atm, NULL, shutdown_error);
  } else {
    next_atm = timespec_to_atm_round_down(*next);
    r = run_some_expired_timers(exec_ctx, now_atm, &next_atm, shutdown_error);
    *next = atm_to_timespec(next_atm);
  }
  if (grpc_timer_check_trace) {
    char *next_str;
    if (next == NULL) {
      next_str = gpr_strdup("NULL");
    } else {
      gpr_asprintf(&next_str, "%" PRIdPTR ".%09d [%" PRIdPTR "]", next->tv_sec,
                   next->tv_nsec, next_atm);
    }
    gpr_log(GPR_DEBUG, "TIMER CHECK END: %d timers triggered; next=%s", r,
            next_str);
    gpr_free(next_str);
  }
  return r > 0;
=======
          : GRPC_ERROR_CREATE_FROM_STATIC_STRING("Shutting down timer system"));
>>>>>>> 7e6b7df8d6bbb80c19ae1736e0c35b4eab06c541
}

#endif /* GRPC_TIMER_USE_GENERIC */
