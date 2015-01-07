/*
 *
 * Copyright 2014, Google Inc.
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

#include "src/core/iomgr/pollset_posix.h"

#include <errno.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "src/core/iomgr/alarm_internal.h"
#include "src/core/iomgr/fd_posix.h"
#include "src/core/iomgr/iomgr_internal.h"
#include "src/core/iomgr/socket_utils_posix.h"
#include <grpc/support/alloc.h>
#include <grpc/support/log.h>
#include <grpc/support/thd.h>
#include <grpc/support/useful.h>

/* kick pipes: we keep a sharded set of pipes to allow breaking from poll.
   Ideally this would be 1:1 with pollsets, but we'd like to avoid associating
   full kernel objects with each pollset to keep them lightweight, so instead
   keep a sharded set and allow associating a pollset with one of the shards.

   TODO(ctiller): move this out from this file, and allow an eventfd
                  implementation on linux */

#define LOG2_KICK_SHARDS 6
#define KICK_SHARDS (1 << LOG2_KICK_SHARDS)

static int g_kick_pipes[KICK_SHARDS][2];
static grpc_pollset g_backup_pollset;
static int g_shutdown_backup_poller;
static gpr_event g_backup_poller_done;

static void backup_poller(void *p) {
  gpr_timespec delta = gpr_time_from_millis(100);
  gpr_timespec last_poll = gpr_now();

  gpr_mu_lock(&g_backup_pollset.mu);
  while (g_shutdown_backup_poller == 0) {
    gpr_timespec next_poll = gpr_time_add(last_poll, delta);
    grpc_pollset_work(&g_backup_pollset, next_poll);
    gpr_mu_unlock(&g_backup_pollset.mu);
    gpr_sleep_until(next_poll);
    gpr_mu_lock(&g_backup_pollset.mu);
    last_poll = next_poll;
  }
  gpr_mu_unlock(&g_backup_pollset.mu);

  gpr_event_set(&g_backup_poller_done, (void *)1);
}

static size_t kick_shard(const grpc_pollset *info) {
  size_t x = (size_t)info;
  return ((x >> 4) ^ (x >> 9) ^ (x >> 14)) & (KICK_SHARDS - 1);
}

int grpc_kick_read_fd(grpc_pollset *p) {
  return g_kick_pipes[kick_shard(p)][0];
}

static int grpc_kick_write_fd(grpc_pollset *p) {
  return g_kick_pipes[kick_shard(p)][1];
}

void grpc_pollset_force_kick(grpc_pollset *p) {
  char c = 0;
  while (write(grpc_kick_write_fd(p), &c, 1) != 1 && errno == EINTR)
    ;
}

void grpc_pollset_kick(grpc_pollset *p) {
  if (!p->counter) return;
  grpc_pollset_force_kick(p);
}

void grpc_kick_drain(grpc_pollset *p) {
  int fd = grpc_kick_read_fd(p);
  char buf[128];
  int r;

  for (;;) {
    r = read(fd, buf, sizeof(buf));
    if (r > 0) continue;
    if (r == 0) return;
    switch (errno) {
      case EAGAIN:
        return;
      case EINTR:
        continue;
      default:
        gpr_log(GPR_ERROR, "error reading pipe: %s", strerror(errno));
        return;
    }
  }
}

/* global state management */

grpc_pollset *grpc_backup_pollset() { return &g_backup_pollset; }

void grpc_pollset_global_init() {
  int i;
  gpr_thd_id id;

  /* initialize the kick shards */
  for (i = 0; i < KICK_SHARDS; i++) {
    GPR_ASSERT(0 == pipe(g_kick_pipes[i]));
    GPR_ASSERT(grpc_set_socket_nonblocking(g_kick_pipes[i][0], 1));
    GPR_ASSERT(grpc_set_socket_nonblocking(g_kick_pipes[i][1], 1));
  }

  /* initialize the backup pollset */
  grpc_pollset_init(&g_backup_pollset);

  /* start the backup poller thread */
  g_shutdown_backup_poller = 0;
  gpr_event_init(&g_backup_poller_done);
  gpr_thd_new(&id, backup_poller, NULL, NULL);
}

void grpc_pollset_global_shutdown() {
  int i;

  /* terminate the backup poller thread */
  gpr_mu_lock(&g_backup_pollset.mu);
  g_shutdown_backup_poller = 1;
  gpr_mu_unlock(&g_backup_pollset.mu);
  gpr_event_wait(&g_backup_poller_done, gpr_inf_future);

  /* destroy the backup pollset */
  grpc_pollset_destroy(&g_backup_pollset);

  /* destroy the kick shards */
  for (i = 0; i < KICK_SHARDS; i++) {
    close(g_kick_pipes[i][0]);
    close(g_kick_pipes[i][1]);
  }
}

/* main interface */

static void become_empty_pollset(grpc_pollset *pollset);
static void become_unary_pollset(grpc_pollset *pollset, grpc_fd *fd);

void grpc_pollset_init(grpc_pollset *pollset) {
  gpr_mu_init(&pollset->mu);
  gpr_cv_init(&pollset->cv);
  become_empty_pollset(pollset);
}

void grpc_pollset_add_fd(grpc_pollset *pollset, grpc_fd *fd) {
  gpr_mu_lock(&pollset->mu);
  pollset->vtable->add_fd(pollset, fd);
  gpr_cv_broadcast(&pollset->cv);
  gpr_mu_unlock(&pollset->mu);
}

void grpc_pollset_del_fd(grpc_pollset *pollset, grpc_fd *fd) {
  gpr_mu_lock(&pollset->mu);
  pollset->vtable->del_fd(pollset, fd);
  gpr_cv_broadcast(&pollset->cv);
  gpr_mu_unlock(&pollset->mu);
}

int grpc_pollset_work(grpc_pollset *pollset, gpr_timespec deadline) {
  /* pollset->mu already held */
  gpr_timespec now;
  now = gpr_now();
  if (gpr_time_cmp(now, deadline) > 0) {
    return 0;
  }
  if (grpc_maybe_call_delayed_callbacks(&pollset->mu, 1)) {
    return 1;
  }
  if (grpc_alarm_check(&pollset->mu, now, &deadline)) {
    return 1;
  }
  return pollset->vtable->maybe_work(pollset, deadline, now, 1);
}

void grpc_pollset_destroy(grpc_pollset *pollset) {
  pollset->vtable->destroy(pollset);
  gpr_mu_destroy(&pollset->mu);
  gpr_cv_destroy(&pollset->cv);
}

/*
 * empty_pollset - a vtable that provides polling for NO file descriptors
 */

static void empty_pollset_add_fd(grpc_pollset *pollset, grpc_fd *fd) {
  become_unary_pollset(pollset, fd);
}

static void empty_pollset_del_fd(grpc_pollset *pollset, grpc_fd *fd) {}

static int empty_pollset_maybe_work(grpc_pollset *pollset,
                                    gpr_timespec deadline, gpr_timespec now,
                                    int allow_synchronous_callback) {
  return 0;
}

static void empty_pollset_destroy(grpc_pollset *pollset) {}

static const grpc_pollset_vtable empty_pollset = {
    empty_pollset_add_fd, empty_pollset_del_fd, empty_pollset_maybe_work,
    empty_pollset_destroy};

static void become_empty_pollset(grpc_pollset *pollset) {
  pollset->vtable = &empty_pollset;
}

/*
 * unary_poll_pollset - a vtable that provides polling for one file descriptor
 *                      via poll()
 */

static void unary_poll_pollset_add_fd(grpc_pollset *pollset, grpc_fd *fd) {
  grpc_fd *fds[2];
  if (fd == pollset->data.ptr) return;
  fds[0] = pollset->data.ptr;
  fds[1] = fd;
  grpc_platform_become_multipoller(pollset, fds, GPR_ARRAY_SIZE(fds));
  grpc_fd_unref(fds[0]);
}

static void unary_poll_pollset_del_fd(grpc_pollset *pollset, grpc_fd *fd) {
  if (fd == pollset->data.ptr) {
    grpc_fd_unref(pollset->data.ptr);
    become_empty_pollset(pollset);
  }
}

static int unary_poll_pollset_maybe_work(grpc_pollset *pollset,
                                         gpr_timespec deadline,
                                         gpr_timespec now,
                                         int allow_synchronous_callback) {
  struct pollfd pfd[2];
  grpc_fd *fd;
  int timeout;
  int r;

  if (pollset->counter) {
    return 0;
  }
  fd = pollset->data.ptr;
  if (grpc_fd_is_orphaned(fd)) {
    grpc_fd_unref(fd);
    become_empty_pollset(pollset);
    return 0;
  }
  if (gpr_time_cmp(deadline, gpr_inf_future) == 0) {
    timeout = -1;
  } else {
    timeout = gpr_time_to_millis(gpr_time_sub(deadline, now));
    if (timeout <= 0) {
      return 1;
    }
  }
  pfd[0].fd = grpc_kick_read_fd(pollset);
  pfd[0].events = POLLIN;
  pfd[0].revents = 0;
  pfd[1].fd = fd->fd;
  pfd[1].events = grpc_fd_begin_poll(fd, pollset, POLLIN, POLLOUT);
  pfd[1].revents = 0;
  pollset->counter = 1;
  gpr_mu_unlock(&pollset->mu);

  r = poll(pfd, GPR_ARRAY_SIZE(pfd), timeout);
  if (r < 0) {
    gpr_log(GPR_ERROR, "poll() failed: %s", strerror(errno));
  } else if (r == 0) {
    /* do nothing */
  } else {
    if (pfd[0].revents & POLLIN) {
      grpc_kick_drain(pollset);
    }
    if (pfd[1].revents & POLLIN) {
      grpc_fd_become_readable(fd, allow_synchronous_callback);
    }
    if (pfd[1].revents & POLLOUT) {
      grpc_fd_become_writable(fd, allow_synchronous_callback);
    }
  }

  gpr_mu_lock(&pollset->mu);
  grpc_fd_end_poll(fd, pollset);
  pollset->counter = 0;
  gpr_cv_broadcast(&pollset->cv);
  return 1;
}

static void unary_poll_pollset_destroy(grpc_pollset *pollset) {
  GPR_ASSERT(pollset->counter == 0);
  grpc_fd_unref(pollset->data.ptr);
}

static const grpc_pollset_vtable unary_poll_pollset = {
    unary_poll_pollset_add_fd, unary_poll_pollset_del_fd,
    unary_poll_pollset_maybe_work, unary_poll_pollset_destroy};

static void become_unary_pollset(grpc_pollset *pollset, grpc_fd *fd) {
  pollset->vtable = &unary_poll_pollset;
  pollset->counter = 0;
  pollset->data.ptr = fd;
  grpc_fd_ref(fd);
}
