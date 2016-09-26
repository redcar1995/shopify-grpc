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

#include <limits.h>
#include <string.h>

#include <grpc/support/alloc.h>
#include <grpc/support/log.h>
#include <grpc/support/slice_buffer.h>
#include <grpc/support/string_util.h>

#include "src/core/lib/iomgr/error.h"
#include "src/core/lib/iomgr/network_status_tracker.h"
#include "src/core/lib/iomgr/tcp_uv.h"
#include "src/core/lib/support/string.h"

int grpc_tcp_trace = 0;

typedef struct {
  grpc_endpoint base;
  gpr_refcount refcount;

  uv_tcp_t *handle;

  grpc_closure *read_cb;
  grpc_closure *write_cb;

  gpr_slice read_slice;
  gpr_slice_buffer *read_slices;
  gpr_slice_buffer *write_slices;
  uv_buf_t *write_buffers;

  int shutting_down;
  char *peer_string;
  grpc_pollset *pollset;
} grpc_tcp;

static void uv_close_callback(uv_handle_t *handle) {
  gpr_log(GPR_DEBUG, "Freeing uv_tcp_t handle %p", handle);
  gpr_free(handle);
}

static void tcp_free(grpc_tcp *tcp) { gpr_free(tcp); }

/*#define GRPC_TCP_REFCOUNT_DEBUG*/
#ifdef GRPC_TCP_REFCOUNT_DEBUG
#define TCP_UNREF(tcp, reason) tcp_unref((tcp), (reason), __FILE__, __LINE__)
#define TCP_REF(tcp, reason) tcp_ref((tcp), (reason), __FILE__, __LINE__)
static void tcp_unref(grpc_tcp *tcp, const char *reason, const char *file,
                      int line) {
  gpr_log(file, line, GPR_LOG_SEVERITY_DEBUG, "TCP unref %p : %s %d -> %d", tcp,
          reason, tcp->refcount.count, tcp->refcount.count - 1);
  if (gpr_unref(&tcp->refcount)) {
    tcp_free(tcp);
  }
}

static void tcp_ref(grpc_tcp *tcp, const char *reason, const char *file,
                    int line) {
  gpr_log(file, line, GPR_LOG_SEVERITY_DEBUG, "TCP   ref %p : %s %d -> %d", tcp,
          reason, tcp->refcount.count, tcp->refcount.count + 1);
  gpr_ref(&tcp->refcount);
}
#else
#define TCP_UNREF(tcp, reason) tcp_unref((tcp))
#define TCP_REF(tcp, reason) tcp_ref((tcp))
static void tcp_unref(grpc_tcp *tcp) {
  if (gpr_unref(&tcp->refcount)) {
    tcp_free(tcp);
  }
}

static void tcp_ref(grpc_tcp *tcp) { gpr_ref(&tcp->refcount); }
#endif

static void alloc_uv_buf(uv_handle_t *handle, size_t suggested_size,
                         uv_buf_t *buf) {
  grpc_tcp *tcp = handle->data;
  (void)suggested_size;
  tcp->read_slice = gpr_slice_malloc(GRPC_TCP_DEFAULT_READ_SLICE_SIZE);
  buf->base = (char *)GPR_SLICE_START_PTR(tcp->read_slice);
  buf->len = GPR_SLICE_LENGTH(tcp->read_slice);
}

static void read_callback(uv_stream_t *stream, ssize_t nread,
                          const uv_buf_t *buf) {
  gpr_slice sub;
  grpc_error *error;
  grpc_exec_ctx exec_ctx = GRPC_EXEC_CTX_INIT;
  grpc_tcp *tcp = stream->data;
  grpc_closure *cb = tcp->read_cb;
  if (nread == 0) {
    // Nothing happened. Wait for the next callback
    return;
  }
  TCP_UNREF(tcp, "read");
  tcp->read_cb = NULL;
  // TODO(murgatroid99): figure out what the return value here means
  uv_read_stop(stream);
  if (nread == UV_EOF) {
    error = GRPC_ERROR_CREATE("EOF");
  } else if (nread > 0) {
    // Successful read
    sub = gpr_slice_sub_no_ref(tcp->read_slice, 0, nread);
    gpr_slice_buffer_add(tcp->read_slices, sub);
    error = GRPC_ERROR_NONE;
    if (grpc_tcp_trace) {
      size_t i;
      const char *str = grpc_error_string(error);
      gpr_log(GPR_DEBUG, "read: error=%s", str);
      grpc_error_free_string(str);
      for (i = 0; i < tcp->read_slices->count; i++) {
        char *dump = gpr_dump_slice(tcp->read_slices->slices[i],
                                    GPR_DUMP_HEX | GPR_DUMP_ASCII);
        gpr_log(GPR_DEBUG, "READ %p (peer=%s): %s", tcp, tcp->peer_string,
                dump);
        gpr_free(dump);
      }
    }
  } else {
    // nread < 0: Error
    error = GRPC_ERROR_CREATE("TCP Read failed");
  }
  grpc_exec_ctx_sched(&exec_ctx, cb, error, NULL);
  grpc_exec_ctx_finish(&exec_ctx);
}

static void uv_endpoint_read(grpc_exec_ctx *exec_ctx, grpc_endpoint *ep,
                             gpr_slice_buffer *read_slices, grpc_closure *cb) {
  grpc_tcp *tcp = (grpc_tcp *)ep;
  int status;
  grpc_error *error = GRPC_ERROR_NONE;
  GPR_ASSERT(tcp->read_cb == NULL);
  tcp->read_cb = cb;
  tcp->read_slices = read_slices;
  gpr_slice_buffer_reset_and_unref(read_slices);
  TCP_REF(tcp, "read");
  // TODO(murgatroid99): figure out what the return value here means
  status =
      uv_read_start((uv_stream_t *)tcp->handle, alloc_uv_buf, read_callback);
  if (status != 0) {
    error = GRPC_ERROR_CREATE("TCP Read failed at start");
    error =
        grpc_error_set_str(error, GRPC_ERROR_STR_OS_ERROR, uv_strerror(status));
    grpc_exec_ctx_sched(exec_ctx, cb, error, NULL);
  }
  if (grpc_tcp_trace) {
    const char *str = grpc_error_string(error);
    gpr_log(GPR_DEBUG, "Initiating read on %p: error=%s", tcp, str);
  }
}

static void write_callback(uv_write_t *req, int status) {
  grpc_tcp *tcp = req->data;
  grpc_error *error;
  grpc_exec_ctx exec_ctx = GRPC_EXEC_CTX_INIT;
  grpc_closure *cb = tcp->write_cb;
  tcp->write_cb = NULL;
  TCP_UNREF(tcp, "write");
  if (status == 0) {
    error = GRPC_ERROR_NONE;
  } else {
    error = GRPC_ERROR_CREATE("TCP Write failed");
  }
  if (grpc_tcp_trace) {
    const char *str = grpc_error_string(error);
    gpr_log(GPR_DEBUG, "write complete on %p: error=%s", tcp, str);
  }
  gpr_free(tcp->write_buffers);
  gpr_free(req);
  grpc_exec_ctx_sched(&exec_ctx, cb, error, NULL);
  grpc_exec_ctx_finish(&exec_ctx);
}

static void uv_endpoint_write(grpc_exec_ctx *exec_ctx, grpc_endpoint *ep,
                              gpr_slice_buffer *write_slices,
                              grpc_closure *cb) {
  grpc_tcp *tcp = (grpc_tcp *)ep;
  uv_buf_t *buffers;
  unsigned int buffer_count;
  unsigned int i;
  gpr_slice *slice;
  uv_write_t *write_req;

  if (grpc_tcp_trace) {
    size_t i;

    for (i = 0; i < write_slices->count; i++) {
      char *data = gpr_dump_slice(write_slices->slices[i],
                                  GPR_DUMP_HEX | GPR_DUMP_ASCII);
      gpr_log(GPR_DEBUG, "WRITE %p (peer=%s): %s", tcp, tcp->peer_string, data);
      gpr_free(data);
    }
  }

  if (tcp->shutting_down) {
    grpc_exec_ctx_sched(exec_ctx, cb,
                        GRPC_ERROR_CREATE("TCP socket is shutting down"), NULL);
    return;
  }

  GPR_ASSERT(tcp->write_cb == NULL);
  tcp->write_slices = write_slices;
  GPR_ASSERT(tcp->write_slices->count <= UINT_MAX);
  if (tcp->write_slices->count == 0) {
    // No slices means we don't have to do anything,
    // and libuv doesn't like empty writes
    grpc_exec_ctx_sched(exec_ctx, cb, GRPC_ERROR_NONE, NULL);
    return;
  }

  tcp->write_cb = cb;
  buffer_count = (unsigned int)tcp->write_slices->count;
  buffers = gpr_malloc(sizeof(uv_buf_t) * buffer_count);
  for (i = 0; i < buffer_count; i++) {
    slice = &tcp->write_slices->slices[i];
    buffers[i].base = (char *)GPR_SLICE_START_PTR(*slice);
    buffers[i].len = GPR_SLICE_LENGTH(*slice);
  }
  write_req = gpr_malloc(sizeof(uv_write_t));
  write_req->data = tcp;
  TCP_REF(tcp, "write");
  // TODO(murgatroid99): figure out what the return value here means
  uv_write(write_req, (uv_stream_t *)tcp->handle, buffers, buffer_count,
           write_callback);
}

static void uv_add_to_pollset(grpc_exec_ctx *exec_ctx, grpc_endpoint *ep,
                              grpc_pollset *pollset) {
  // No-op. We're ignoring pollsets currently
  (void)exec_ctx;
  (void)ep;
  (void)pollset;
  grpc_tcp *tcp = (grpc_tcp *)ep;
  tcp->pollset = pollset;
}

static void uv_add_to_pollset_set(grpc_exec_ctx *exec_ctx, grpc_endpoint *ep,
                                  grpc_pollset_set *pollset) {
  // No-op. We're ignoring pollsets currently
  (void)exec_ctx;
  (void)ep;
  (void)pollset;
}

static void shutdown_callback(uv_shutdown_t *req, int status) { gpr_free(req); }

static void uv_endpoint_shutdown(grpc_exec_ctx *exec_ctx, grpc_endpoint *ep) {
  grpc_tcp *tcp = (grpc_tcp *)ep;
  uv_shutdown_t *req = gpr_malloc(sizeof(uv_shutdown_t));
  uv_shutdown(req, (uv_stream_t *)tcp->handle, shutdown_callback);
}

static void uv_destroy(grpc_exec_ctx *exec_ctx, grpc_endpoint *ep) {
  grpc_network_status_unregister_endpoint(ep);
  grpc_tcp *tcp = (grpc_tcp *)ep;
  gpr_log(GPR_DEBUG, "Closing uv_tcp_t handle %p", tcp->handle);
  uv_close((uv_handle_t *)tcp->handle, uv_close_callback);
  TCP_UNREF(tcp, "destroy");
}

static char *uv_get_peer(grpc_endpoint *ep) {
  grpc_tcp *tcp = (grpc_tcp *)ep;
  return gpr_strdup(tcp->peer_string);
}

static grpc_workqueue *uv_get_workqueue(grpc_endpoint *ep) { return NULL; }

static grpc_endpoint_vtable vtable = {uv_endpoint_read,
                                      uv_endpoint_write,
                                      uv_get_workqueue,
                                      uv_add_to_pollset,
                                      uv_add_to_pollset_set,
                                      uv_endpoint_shutdown,
                                      uv_destroy,
                                      uv_get_peer};

grpc_endpoint *grpc_tcp_create(uv_tcp_t *handle, char *peer_string) {
  grpc_tcp *tcp = (grpc_tcp *)gpr_malloc(sizeof(grpc_tcp));

  if (grpc_tcp_trace) {
    gpr_log(GPR_DEBUG, "Creating TCP endpoint %p", tcp);
  }

  memset(tcp, 0, sizeof(grpc_tcp));
  tcp->base.vtable = &vtable;
  tcp->handle = handle;
  handle->data = tcp;
  gpr_ref_init(&tcp->refcount, 1);
  tcp->peer_string = gpr_strdup(peer_string);
  /* Tell network status tracking code about the new endpoint */
  grpc_network_status_register_endpoint(&tcp->base);

#ifndef GRPC_UV_TCP_HOLD_LOOP
  uv_unref((uv_handle_t *)handle);
#endif

  return &tcp->base;
}

#endif /* GRPC_UV */
