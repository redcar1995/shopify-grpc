/*
 *
 * Copyright 2016, Google Inc.
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

#ifdef GRPC_UV

#include <string.h>

#include <grpc/support/alloc.h>
#include <grpc/support/log.h>

#include "src/core/lib/iomgr/error.h"
#include "src/core/lib/iomgr/sockaddr_utils.h"
#include "src/core/lib/iomgr/tcp_client.h"
#include "src/core/lib/iomgr/tcp_uv.h"
#include "src/core/lib/iomgr/timer.h"

typedef struct grpc_uv_tcp_connect {
  uv_connect_t connect_req;
  grpc_timer alarm;
  uv_tcp_t *tcp_handle;
  grpc_closure *closure;
  grpc_endpoint **endpoint;
  int refs;
  char *addr_name;
} grpc_uv_tcp_connect;

static void uv_tcp_connect_cleanup(grpc_uv_tcp_connect *connect) {
  gpr_free(connect);
}

static void tcp_close_callback(uv_handle_t *handle) {
  gpr_log(GPR_DEBUG, "Freeing uv_tcp_t handle %p", handle);
  gpr_free(handle);
}

static void uv_tc_on_alarm(grpc_exec_ctx *exec_ctx, void *acp, grpc_error *error) {
  int done;
  grpc_uv_tcp_connect *connect = acp;
  if (error == GRPC_ERROR_NONE) {
    /* error == NONE implies that the timer ran out, and wasn't cancelled. If
       it was cancelled, then the handler that cancelled it also should close
       the handle, if applicable */
    gpr_log(GPR_DEBUG, "Closing uv_tcp_t handle %p", connect->tcp_handle);
    uv_close((uv_handle_t *)connect->tcp_handle, tcp_close_callback);
  }
  done = (--connect->refs == 0);
  if (done) {
    uv_tcp_connect_cleanup(connect);
  }
}

static void uv_tc_on_connect(uv_connect_t *req, int status) {
  grpc_uv_tcp_connect *connect = req->data;
  grpc_exec_ctx exec_ctx = GRPC_EXEC_CTX_INIT;
  grpc_error *error = GRPC_ERROR_NONE;
  int done;
  grpc_closure *closure = connect->closure;
  grpc_timer_cancel(&exec_ctx, &connect->alarm);
  if (status == 0) {
    *connect->endpoint = grpc_tcp_create(connect->tcp_handle,
                                         connect->addr_name);
  } else {
    error = GRPC_ERROR_CREATE("Failed to connect to remote host");
    error = grpc_error_set_int(error, GRPC_ERROR_INT_ERRNO, -status);
    error = grpc_error_set_str(error, GRPC_ERROR_STR_OS_ERROR,
                               uv_strerror(status));
    if (status == UV_ECANCELED) {
      error = grpc_error_set_str(error, GRPC_ERROR_STR_OS_ERROR,
                                 "Timeout occurred");
      // This should only happen if the handle is already closed
    } else {
      error = grpc_error_set_str(error, GRPC_ERROR_STR_OS_ERROR,
                                 uv_strerror(status));
      gpr_log(GPR_DEBUG, "Closing uv_tcp_t handle %p", connect->tcp_handle);
      uv_close((uv_handle_t *)connect->tcp_handle, tcp_close_callback);
    }
  }
  done = (--connect->refs == 0);
  if (done) {
    uv_tcp_connect_cleanup(connect);
  }
  grpc_exec_ctx_sched(&exec_ctx, closure, error, NULL);
  grpc_exec_ctx_finish(&exec_ctx);
}

void grpc_tcp_client_connect(grpc_exec_ctx *exec_ctx,
                             grpc_closure *closure, grpc_endpoint **ep,
                             grpc_pollset_set *interested_parties,
                             const struct sockaddr *addr,
                             size_t addr_len, gpr_timespec deadline) {
  grpc_uv_tcp_connect *connect;
  (void)interested_parties;
  connect = gpr_malloc(sizeof(grpc_uv_tcp_connect));
  memset(connect, 0, sizeof(grpc_uv_tcp_connect));
  connect->closure = closure;
  connect->endpoint = ep;
  connect->tcp_handle = gpr_malloc(sizeof(uv_tcp_t));
  gpr_log(GPR_DEBUG, "Allocated uv_tcp_t handle %p", connect->tcp_handle);
  connect->addr_name = grpc_sockaddr_to_uri(addr);
  uv_tcp_init(uv_default_loop(), connect->tcp_handle);
  connect->connect_req.data = connect;
  // TODO(murgatroid99): figure out what the return value here means
  uv_tcp_connect(&connect->connect_req, connect->tcp_handle, addr,
                 uv_tc_on_connect);
  grpc_timer_init(exec_ctx, &connect->alarm,
                  gpr_convert_clock_type(deadline, GPR_CLOCK_MONOTONIC),
                  uv_tc_on_alarm, connect, gpr_now(GPR_CLOCK_MONOTONIC));
}

#endif /* GRPC_UV */
