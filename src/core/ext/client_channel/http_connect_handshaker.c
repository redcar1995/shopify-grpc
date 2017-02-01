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

#include "src/core/ext/client_channel/http_connect_handshaker.h"

#include <string.h>

#include <grpc/slice_buffer.h>
#include <grpc/support/alloc.h>
#include <grpc/support/log.h>
#include <grpc/support/string_util.h>

#include "src/core/ext/client_channel/client_channel.h"
#include "src/core/ext/client_channel/resolver_registry.h"
#include "src/core/ext/client_channel/uri_parser.h"
#include "src/core/lib/channel/channel_args.h"
#include "src/core/lib/channel/handshaker_registry.h"
#include "src/core/lib/http/format_request.h"
#include "src/core/lib/http/parser.h"
#include "src/core/lib/slice/slice_internal.h"
#include "src/core/lib/support/env.h"
#include "src/core/lib/support/string.h"

typedef struct http_connect_handshaker {
  // Base class.  Must be first.
  grpc_handshaker base;

  gpr_refcount refcount;
  gpr_mu mu;

  bool shutdown;
  // Endpoint and read buffer to destroy after a shutdown.
  grpc_endpoint* endpoint_to_destroy;
  grpc_slice_buffer* read_buffer_to_destroy;

  // State saved while performing the handshake.
  grpc_handshaker_args* args;
  grpc_closure* on_handshake_done;

  // Objects for processing the HTTP CONNECT request and response.
  grpc_slice_buffer write_buffer;
  grpc_closure request_done_closure;
  grpc_closure response_read_closure;
  grpc_http_parser http_parser;
  grpc_http_response http_response;
} http_connect_handshaker;

// Unref and clean up handshaker.
static void http_connect_handshaker_unref(grpc_exec_ctx* exec_ctx,
                                          http_connect_handshaker* handshaker) {
  if (gpr_unref(&handshaker->refcount)) {
    gpr_mu_destroy(&handshaker->mu);
    if (handshaker->endpoint_to_destroy != NULL) {
      grpc_endpoint_destroy(exec_ctx, handshaker->endpoint_to_destroy);
    }
    if (handshaker->read_buffer_to_destroy != NULL) {
      grpc_slice_buffer_destroy_internal(exec_ctx,
                                         handshaker->read_buffer_to_destroy);
      gpr_free(handshaker->read_buffer_to_destroy);
    }
    grpc_slice_buffer_destroy_internal(exec_ctx, &handshaker->write_buffer);
    grpc_http_parser_destroy(&handshaker->http_parser);
    grpc_http_response_destroy(&handshaker->http_response);
    gpr_free(handshaker);
  }
}

// Set args fields to NULL, saving the endpoint and read buffer for
// later destruction.
static void cleanup_args_for_failure_locked(
    grpc_exec_ctx* exec_ctx, http_connect_handshaker* handshaker) {
  handshaker->endpoint_to_destroy = handshaker->args->endpoint;
  handshaker->args->endpoint = NULL;
  handshaker->read_buffer_to_destroy = handshaker->args->read_buffer;
  handshaker->args->read_buffer = NULL;
  grpc_channel_args_destroy(exec_ctx, handshaker->args->args);
  handshaker->args->args = NULL;
}

// If the handshake failed or we're shutting down, clean up and invoke the
// callback with the error.
static void handshake_failed_locked(grpc_exec_ctx* exec_ctx,
                                    http_connect_handshaker* handshaker,
                                    grpc_error* error) {
  if (error == GRPC_ERROR_NONE) {
    // If we were shut down after an endpoint operation succeeded but
    // before the endpoint callback was invoked, we need to generate our
    // own error.
    error = GRPC_ERROR_CREATE("Handshaker shutdown");
  }
  if (!handshaker->shutdown) {
    // TODO(ctiller): It is currently necessary to shutdown endpoints
    // before destroying them, even if we know that there are no
    // pending read/write callbacks.  This should be fixed, at which
    // point this can be removed.
    grpc_endpoint_shutdown(exec_ctx, handshaker->args->endpoint,
                           GRPC_ERROR_REF(error));
    // Not shutting down, so the handshake failed.  Clean up before
    // invoking the callback.
    cleanup_args_for_failure_locked(exec_ctx, handshaker);
    // Set shutdown to true so that subsequent calls to
    // http_connect_handshaker_shutdown() do nothing.
    handshaker->shutdown = true;
  }
  // Invoke callback.
  grpc_closure_sched(exec_ctx, handshaker->on_handshake_done, error);
}

// Callback invoked when finished writing HTTP CONNECT request.
static void on_write_done(grpc_exec_ctx* exec_ctx, void* arg,
                          grpc_error* error) {
  http_connect_handshaker* handshaker = arg;
  gpr_mu_lock(&handshaker->mu);
  if (error != GRPC_ERROR_NONE || handshaker->shutdown) {
    // If the write failed or we're shutting down, clean up and invoke the
    // callback with the error.
    handshake_failed_locked(exec_ctx, handshaker, GRPC_ERROR_REF(error));
    gpr_mu_unlock(&handshaker->mu);
    http_connect_handshaker_unref(exec_ctx, handshaker);
  } else {
    // Otherwise, read the response.
    // The read callback inherits our ref to the handshaker.
    grpc_endpoint_read(exec_ctx, handshaker->args->endpoint,
                       handshaker->args->read_buffer,
                       &handshaker->response_read_closure);
    gpr_mu_unlock(&handshaker->mu);
  }
}

// Callback invoked for reading HTTP CONNECT response.
static void on_read_done(grpc_exec_ctx* exec_ctx, void* arg,
                         grpc_error* error) {
  http_connect_handshaker* handshaker = arg;
  gpr_mu_lock(&handshaker->mu);
  if (error != GRPC_ERROR_NONE || handshaker->shutdown) {
    // If the read failed or we're shutting down, clean up and invoke the
    // callback with the error.
    handshake_failed_locked(exec_ctx, handshaker, GRPC_ERROR_REF(error));
    goto done;
  }
  // Add buffer to parser.
  for (size_t i = 0; i < handshaker->args->read_buffer->count; ++i) {
    if (GRPC_SLICE_LENGTH(handshaker->args->read_buffer->slices[i]) > 0) {
      size_t body_start_offset = 0;
      error = grpc_http_parser_parse(&handshaker->http_parser,
                                     handshaker->args->read_buffer->slices[i],
                                     &body_start_offset);
      if (error != GRPC_ERROR_NONE) {
        handshake_failed_locked(exec_ctx, handshaker, error);
        goto done;
      }
      if (handshaker->http_parser.state == GRPC_HTTP_BODY) {
        // Remove the data we've already read from the read buffer,
        // leaving only the leftover bytes (if any).
        grpc_slice_buffer tmp_buffer;
        grpc_slice_buffer_init(&tmp_buffer);
        if (body_start_offset <
            GRPC_SLICE_LENGTH(handshaker->args->read_buffer->slices[i])) {
          grpc_slice_buffer_add(
              &tmp_buffer,
              grpc_slice_split_tail(&handshaker->args->read_buffer->slices[i],
                                    body_start_offset));
        }
        grpc_slice_buffer_addn(&tmp_buffer,
                               &handshaker->args->read_buffer->slices[i + 1],
                               handshaker->args->read_buffer->count - i - 1);
        grpc_slice_buffer_swap(handshaker->args->read_buffer, &tmp_buffer);
        grpc_slice_buffer_destroy_internal(exec_ctx, &tmp_buffer);
        break;
      }
    }
  }
  // If we're not done reading the response, read more data.
  // TODO(roth): In practice, I suspect that the response to a CONNECT
  // request will never include a body, in which case this check is
  // sufficient.  However, the language of RFC-2817 doesn't explicitly
  // forbid the response from including a body.  If there is a body,
  // it's possible that we might have parsed part but not all of the
  // body, in which case this check will cause us to fail to parse the
  // remainder of the body.  If that ever becomes an issue, we may
  // need to fix the HTTP parser to understand when the body is
  // complete (e.g., handling chunked transfer encoding or looking
  // at the Content-Length: header).
  if (handshaker->http_parser.state != GRPC_HTTP_BODY) {
    grpc_slice_buffer_reset_and_unref_internal(exec_ctx,
                                               handshaker->args->read_buffer);
    grpc_endpoint_read(exec_ctx, handshaker->args->endpoint,
                       handshaker->args->read_buffer,
                       &handshaker->response_read_closure);
    gpr_mu_unlock(&handshaker->mu);
    return;
  }
  // Make sure we got a 2xx response.
  if (handshaker->http_response.status < 200 ||
      handshaker->http_response.status >= 300) {
    char* msg;
    gpr_asprintf(&msg, "HTTP proxy returned response code %d",
                 handshaker->http_response.status);
    error = GRPC_ERROR_CREATE(msg);
    gpr_free(msg);
    handshake_failed_locked(exec_ctx, handshaker, error);
    goto done;
  }
  // Success.  Invoke handshake-done callback.
  grpc_closure_sched(exec_ctx, handshaker->on_handshake_done, error);
done:
  // Set shutdown to true so that subsequent calls to
  // http_connect_handshaker_shutdown() do nothing.
  handshaker->shutdown = true;
  gpr_mu_unlock(&handshaker->mu);
  http_connect_handshaker_unref(exec_ctx, handshaker);
}

//
// Public handshaker methods
//

static void http_connect_handshaker_destroy(grpc_exec_ctx* exec_ctx,
                                            grpc_handshaker* handshaker_in) {
  http_connect_handshaker* handshaker = (http_connect_handshaker*)handshaker_in;
  http_connect_handshaker_unref(exec_ctx, handshaker);
}

static void http_connect_handshaker_shutdown(grpc_exec_ctx* exec_ctx,
                                             grpc_handshaker* handshaker_in,
                                             grpc_error* why) {
  http_connect_handshaker* handshaker = (http_connect_handshaker*)handshaker_in;
  gpr_mu_lock(&handshaker->mu);
  if (!handshaker->shutdown) {
    handshaker->shutdown = true;
    grpc_endpoint_shutdown(exec_ctx, handshaker->args->endpoint,
                           GRPC_ERROR_REF(why));
    cleanup_args_for_failure_locked(exec_ctx, handshaker);
  }
  gpr_mu_unlock(&handshaker->mu);
  GRPC_ERROR_UNREF(why);
}

static void http_connect_handshaker_do_handshake(
    grpc_exec_ctx* exec_ctx, grpc_handshaker* handshaker_in,
    grpc_tcp_server_acceptor* acceptor, grpc_closure* on_handshake_done,
    grpc_handshaker_args* args) {
  http_connect_handshaker* handshaker = (http_connect_handshaker*)handshaker_in;
  // Check for HTTP CONNECT channel arg.
  // If not found, invoke on_handshake_done without doing anything.
  const grpc_arg* arg =
      grpc_channel_args_find(args->args, GRPC_ARG_HTTP_CONNECT_SERVER);
  if (arg == NULL) {
    // Set shutdown to true so that subsequent calls to
    // http_connect_handshaker_shutdown() do nothing.
    gpr_mu_lock(&handshaker->mu);
    handshaker->shutdown = true;
    gpr_mu_unlock(&handshaker->mu);
    grpc_closure_sched(exec_ctx, on_handshake_done, GRPC_ERROR_NONE);
    return;
  }
  GPR_ASSERT(arg->type == GRPC_ARG_STRING);
  char* server_name = arg->value.string;
  // Get headers from channel args.
  arg = grpc_channel_args_find(args->args, GRPC_ARG_HTTP_CONNECT_HEADERS);
  grpc_http_header* headers = NULL;
  size_t num_headers = 0;
  char** header_strings = NULL;
  size_t num_header_strings = 0;
  if (arg != NULL) {
    GPR_ASSERT(arg->type == GRPC_ARG_STRING);
    gpr_string_split(arg->value.string, "\n", &header_strings,
                     &num_header_strings);
    headers = gpr_malloc(sizeof(grpc_http_header) * num_header_strings);
    for (size_t i = 0; i < num_header_strings; ++i) {
      char* sep = strchr(header_strings[i], ':');
      if (sep == NULL) {
        gpr_log(GPR_ERROR, "skipping unparseable HTTP CONNECT header: %s",
                header_strings[i]);
        continue;
      }
      *sep = '\0';
      headers[num_headers].key = header_strings[i];
      headers[num_headers].value = sep + 1;
      ++num_headers;
    }
  }
  // Save state in the handshaker object.
  gpr_mu_lock(&handshaker->mu);
  handshaker->args = args;
  handshaker->on_handshake_done = on_handshake_done;
  // Log connection via proxy.
  char* proxy_name = grpc_endpoint_get_peer(args->endpoint);
  gpr_log(GPR_INFO, "Connecting to server %s via HTTP proxy %s", server_name,
          proxy_name);
  gpr_free(proxy_name);
  // Construct HTTP CONNECT request.
  grpc_httpcli_request request;
  memset(&request, 0, sizeof(request));
  request.host = server_name;
  request.http.method = "CONNECT";
  request.http.path = server_name;
  request.http.hdrs = headers;
  request.http.hdr_count = num_headers;
  request.handshaker = &grpc_httpcli_plaintext;
  grpc_slice request_slice = grpc_httpcli_format_connect_request(&request);
  grpc_slice_buffer_add(&handshaker->write_buffer, request_slice);
  // Clean up.
  gpr_free(headers);
  for (size_t i = 0; i < num_header_strings; ++i) {
    gpr_free(header_strings[i]);
  }
  gpr_free(header_strings);
  // Take a new ref to be held by the write callback.
  gpr_ref(&handshaker->refcount);
  grpc_endpoint_write(exec_ctx, args->endpoint, &handshaker->write_buffer,
                      &handshaker->request_done_closure);
  gpr_mu_unlock(&handshaker->mu);
}

static const grpc_handshaker_vtable http_connect_handshaker_vtable = {
    http_connect_handshaker_destroy, http_connect_handshaker_shutdown,
    http_connect_handshaker_do_handshake};

static grpc_handshaker* grpc_http_connect_handshaker_create() {
  http_connect_handshaker* handshaker = gpr_malloc(sizeof(*handshaker));
  memset(handshaker, 0, sizeof(*handshaker));
  grpc_handshaker_init(&http_connect_handshaker_vtable, &handshaker->base);
  gpr_mu_init(&handshaker->mu);
  gpr_ref_init(&handshaker->refcount, 1);
  grpc_slice_buffer_init(&handshaker->write_buffer);
  grpc_closure_init(&handshaker->request_done_closure, on_write_done,
                    handshaker, grpc_schedule_on_exec_ctx);
  grpc_closure_init(&handshaker->response_read_closure, on_read_done,
                    handshaker, grpc_schedule_on_exec_ctx);
  grpc_http_parser_init(&handshaker->http_parser, GRPC_HTTP_RESPONSE,
                        &handshaker->http_response);
  return &handshaker->base;
}

//
// handshaker factory
//

static void handshaker_factory_add_handshakers(
    grpc_exec_ctx* exec_ctx, grpc_handshaker_factory* factory,
    const grpc_channel_args* args, grpc_handshake_manager* handshake_mgr) {
  grpc_handshake_manager_add(handshake_mgr,
                             grpc_http_connect_handshaker_create());
}

static void handshaker_factory_destroy(grpc_exec_ctx* exec_ctx,
                                       grpc_handshaker_factory* factory) {}

static const grpc_handshaker_factory_vtable handshaker_factory_vtable = {
    handshaker_factory_add_handshakers, handshaker_factory_destroy};

static grpc_handshaker_factory handshaker_factory = {
    &handshaker_factory_vtable};

void grpc_http_connect_register_handshaker_factory() {
  grpc_handshaker_factory_register(true /* at_start */, HANDSHAKER_CLIENT,
                                   &handshaker_factory);
}
