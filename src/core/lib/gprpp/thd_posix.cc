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

/* Posix implementation for gpr threads. */

#include <grpc/support/port_platform.h>

#ifdef GPR_POSIX_SYNC

#include "src/core/lib/gprpp/thd.h"

#include <grpc/support/alloc.h>
#include <grpc/support/log.h>
#include <grpc/support/sync.h>
#include <grpc/support/thd_id.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "src/core/lib/gpr/fork.h"
#include "src/core/lib/gpr/useful.h"

namespace grpc_core {
namespace {
gpr_mu g_mu;
gpr_cv g_cv;
int g_thread_count;
int g_awaiting_threads;

struct thd_arg {
  Thread* thread;
  void (*body)(void* arg); /* body of a thread */
  void* arg;               /* argument to a thread */
  const char* name;        /* name of thread. Can be nullptr. */
};

/*****************************************
 * Only used when fork support is enabled
 */

void inc_thd_count() {
  if (grpc_fork_support_enabled()) {
    gpr_mu_lock(&g_mu);
    g_thread_count++;
    gpr_mu_unlock(&g_mu);
  }
}

void dec_thd_count() {
  if (grpc_fork_support_enabled()) {
    gpr_mu_lock(&g_mu);
    g_thread_count--;
    if (g_awaiting_threads && g_thread_count == 0) {
      gpr_cv_signal(&g_cv);
    }
    gpr_mu_unlock(&g_mu);
  }
}

}  // namespace

Thread::Thread(const char* thd_name, void (*thd_body)(void* arg), void* arg,
               bool* success)
    : real_(true), alive_(false), started_(false), joined_(false) {
  gpr_mu_init(&mu_);
  gpr_cv_init(&ready_);
  pthread_attr_t attr;
  /* don't use gpr_malloc as we may cause an infinite recursion with
   * the profiling code */
  thd_arg* a = static_cast<thd_arg*>(malloc(sizeof(*a)));
  GPR_ASSERT(a != nullptr);
  a->thread = this;
  a->body = thd_body;
  a->arg = arg;
  a->name = thd_name;
  inc_thd_count();

  GPR_ASSERT(pthread_attr_init(&attr) == 0);
  GPR_ASSERT(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE) == 0);

  pthread_t p;
  alive_ = (pthread_create(&p, &attr,
                           [](void* v) -> void* {
                             thd_arg a = *static_cast<thd_arg*>(v);
                             free(v);
                             if (a.name != nullptr) {
#if GPR_APPLE_PTHREAD_NAME
                               /* Apple supports 64 characters, and will
                                * truncate if it's longer. */
                               pthread_setname_np(a.name);
#elif GPR_LINUX_PTHREAD_NAME
                               /* Linux supports 16 characters max, and will
                                * error if it's longer. */
                               char buf[16];
                               size_t buf_len = GPR_ARRAY_SIZE(buf) - 1;
                               strncpy(buf, a.name, buf_len);
                               buf[buf_len] = '\0';
                               pthread_setname_np(pthread_self(), buf);
#endif  // GPR_APPLE_PTHREAD_NAME
                             }

                             gpr_mu_lock(&a.thread->mu_);
                             if (!a.thread->started_) {
                               gpr_cv_wait(&a.thread->ready_, &a.thread->mu_,
                                           gpr_inf_future(GPR_CLOCK_MONOTONIC));
                             }
                             gpr_mu_unlock(&a.thread->mu_);

                             (*a.body)(a.arg);
                             dec_thd_count();
                             return nullptr;
                           },
                           a) == 0);

  if (success != nullptr) {
    *success = alive_;
  }

  id_ = gpr_thd_id(p);
  GPR_ASSERT(pthread_attr_destroy(&attr) == 0);

  if (!alive_) {
    /* don't use gpr_free, as this was allocated using malloc (see above) */
    free(a);
    dec_thd_count();
  }
}

Thread::~Thread() {
  if (!alive_) {
    // This thread never existed, so nothing to do
  } else {
    GPR_ASSERT(joined_);
  }
  if (real_) {
    gpr_mu_destroy(&mu_);
    gpr_cv_destroy(&ready_);
  }
}

void Thread::Start() {
  gpr_mu_lock(&mu_);
  if (alive_) {
    started_ = true;
    gpr_cv_signal(&ready_);
  }
  gpr_mu_unlock(&mu_);
}

void Thread::Join() {
  if (alive_) {
    pthread_join(pthread_t(id_), nullptr);
  }
  joined_ = true;
}

void Thread::Init() {
  gpr_mu_init(&g_mu);
  gpr_cv_init(&g_cv);
  g_thread_count = 0;
  g_awaiting_threads = 0;
}

bool Thread::AwaitAll(gpr_timespec deadline) {
  gpr_mu_lock(&g_mu);
  g_awaiting_threads = 1;
  int res = 0;
  if (g_thread_count > 0) {
    res = gpr_cv_wait(&g_cv, &g_mu, deadline);
  }
  g_awaiting_threads = 0;
  gpr_mu_unlock(&g_mu);
  return res == 0;
}

}  // namespace grpc_core

// The following is in the external namespace as it is exposed as C89 API
gpr_thd_id gpr_thd_currentid(void) { return (gpr_thd_id)pthread_self(); }

#endif /* GPR_POSIX_SYNC */
