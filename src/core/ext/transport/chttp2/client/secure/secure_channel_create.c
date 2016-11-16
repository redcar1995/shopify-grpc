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

#include <grpc/grpc.h>

#include <stdlib.h>
#include <string.h>

#include <grpc/slice.h>
#include <grpc/slice_buffer.h>
#include <grpc/support/alloc.h>

#include "src/core/ext/client_channel/client_channel.h"
#include "src/core/ext/client_channel/http_connect_handshaker.h"
#include "src/core/ext/client_channel/resolver_registry.h"
#include "src/core/ext/transport/chttp2/transport/chttp2_transport.h"
#include "src/core/lib/channel/channel_args.h"
#include "src/core/lib/channel/handshaker.h"
#include "src/core/lib/iomgr/tcp_client.h"
#include "src/core/lib/security/context/security_context.h"
#include "src/core/lib/security/credentials/credentials.h"
#include "src/core/lib/security/transport/auth_filters.h"
#include "src/core/lib/surface/api_trace.h"
#include "src/core/lib/surface/channel.h"
#include "src/core/lib/tsi/transport_security_interface.h"

//
// connector
//

typedef struct {
  grpc_connector base;
  gpr_refcount refs;

  grpc_channel_security_connector *security_connector;

  grpc_closure *notify;
  grpc_connect_in_args args;
  grpc_connect_out_args *result;
  grpc_closure initial_string_sent;
  grpc_slice_buffer initial_string_buffer;

  gpr_mu mu;
  grpc_endpoint *connecting_endpoint;
  grpc_endpoint *newly_connecting_endpoint;

  grpc_closure connected_closure;

  grpc_handshake_manager *handshake_mgr;
} connector;

static void connector_ref(grpc_connector *con) {
  connector *c = (connector *)con;
  gpr_ref(&c->refs);
}

static void connector_unref(grpc_exec_ctx *exec_ctx, grpc_connector *con) {
  connector *c = (connector *)con;
  if (gpr_unref(&c->refs)) {
    /* c->initial_string_buffer does not need to be destroyed */
    grpc_handshake_manager_destroy(exec_ctx, c->handshake_mgr);
    gpr_free(c);
  }
}

static void on_handshake_done(grpc_exec_ctx *exec_ctx, void *arg,
                              grpc_error *error) {
  grpc_handshaker_args *args = arg;
  connector *c = args->user_data;
  gpr_mu_lock(&c->mu);
  if (error != GRPC_ERROR_NONE) {
    c->connecting_endpoint = NULL;
    gpr_mu_unlock(&c->mu);
    grpc_channel_args_destroy(args->args);
    gpr_free(args->read_buffer);
  } else {
    if (c->connecting_endpoint == NULL) {
      memset(c->result, 0, sizeof(*c->result));
      gpr_mu_unlock(&c->mu);
    } else {
      c->connecting_endpoint = NULL;
      gpr_mu_unlock(&c->mu);
      c->result->transport = grpc_create_chttp2_transport(
          exec_ctx, args->args, args->endpoint, 1);
      grpc_chttp2_transport_start_reading(exec_ctx, c->result->transport,
                                          args->read_buffer);
    }
    c->result->channel_args = args->args;
  }
  grpc_closure *notify = c->notify;
  c->notify = NULL;
  grpc_exec_ctx_sched(exec_ctx, notify, GRPC_ERROR_REF(error), NULL);
  gpr_free(args);
}

static void on_initial_connect_string_sent(grpc_exec_ctx *exec_ctx, void *arg,
                                           grpc_error *error) {
  connector *c = arg;
  grpc_handshake_manager_do_handshake(
      exec_ctx, c->handshake_mgr, c->connecting_endpoint, c->args.channel_args,
      c->args.deadline, NULL /* acceptor */, on_handshake_done, c);
}

static void connected(grpc_exec_ctx *exec_ctx, void *arg, grpc_error *error) {
  connector *c = arg;
  grpc_endpoint *tcp = c->newly_connecting_endpoint;
  if (tcp != NULL) {
    gpr_mu_lock(&c->mu);
    GPR_ASSERT(c->connecting_endpoint == NULL);
    c->connecting_endpoint = tcp;
    gpr_mu_unlock(&c->mu);
    if (!GRPC_SLICE_IS_EMPTY(c->args.initial_connect_string)) {
      grpc_closure_init(&c->initial_string_sent, on_initial_connect_string_sent,
                        c);
      grpc_slice_buffer_init(&c->initial_string_buffer);
      grpc_slice_buffer_add(&c->initial_string_buffer,
                            c->args.initial_connect_string);
      grpc_endpoint_write(exec_ctx, tcp, &c->initial_string_buffer,
                          &c->initial_string_sent);
    } else {
      grpc_handshake_manager_do_handshake(
          exec_ctx, c->handshake_mgr, tcp, c->args.channel_args,
          c->args.deadline, NULL /* acceptor */, on_handshake_done, c);
    }
  } else {
    memset(c->result, 0, sizeof(*c->result));
    grpc_closure *notify = c->notify;
    c->notify = NULL;
    grpc_exec_ctx_sched(exec_ctx, notify, GRPC_ERROR_REF(error), NULL);
  }
}

static void connector_shutdown(grpc_exec_ctx *exec_ctx, grpc_connector *con) {
  connector *c = (connector *)con;
  grpc_endpoint *ep;
  gpr_mu_lock(&c->mu);
  ep = c->connecting_endpoint;
  c->connecting_endpoint = NULL;
  gpr_mu_unlock(&c->mu);
  if (ep) {
    grpc_endpoint_shutdown(exec_ctx, ep);
  }
}

static void connector_connect(grpc_exec_ctx *exec_ctx, grpc_connector *con,
                              const grpc_connect_in_args *args,
                              grpc_connect_out_args *result,
                              grpc_closure *notify) {
  connector *c = (connector *)con;
  GPR_ASSERT(c->notify == NULL);
  c->notify = notify;
  c->args = *args;
  c->result = result;
  gpr_mu_lock(&c->mu);
  GPR_ASSERT(c->connecting_endpoint == NULL);
  gpr_mu_unlock(&c->mu);
  grpc_closure_init(&c->connected_closure, connected, c);
  grpc_tcp_client_connect(
      exec_ctx, &c->connected_closure, &c->newly_connecting_endpoint,
      args->interested_parties, args->channel_args, args->addr, args->deadline);
}

static const grpc_connector_vtable connector_vtable = {
    connector_ref, connector_unref, connector_shutdown, connector_connect};

//
// client_channel_factory
//

typedef struct {
  grpc_client_channel_factory base;
  gpr_refcount refs;
  grpc_channel_security_connector *security_connector;
} client_channel_factory;

static void client_channel_factory_ref(
    grpc_client_channel_factory *cc_factory) {
  client_channel_factory *f = (client_channel_factory *)cc_factory;
  gpr_ref(&f->refs);
}

static void client_channel_factory_unref(
    grpc_exec_ctx *exec_ctx, grpc_client_channel_factory *cc_factory) {
  client_channel_factory *f = (client_channel_factory *)cc_factory;
  if (gpr_unref(&f->refs)) {
    GRPC_SECURITY_CONNECTOR_UNREF(&f->security_connector->base,
                                  "client_channel_factory");
    gpr_free(f);
  }
}

static grpc_subchannel *client_channel_factory_create_subchannel(
    grpc_exec_ctx *exec_ctx, grpc_client_channel_factory *cc_factory,
    const grpc_subchannel_args *args) {
  client_channel_factory *f = (client_channel_factory *)cc_factory;
  connector *c = gpr_malloc(sizeof(*c));
  memset(c, 0, sizeof(*c));
  c->base.vtable = &connector_vtable;
  c->security_connector = f->security_connector;
  c->handshake_mgr = grpc_handshake_manager_create();
  char *proxy_name = grpc_get_http_proxy_server();
  if (proxy_name != NULL) {
    grpc_handshake_manager_add(
        c->handshake_mgr,
        grpc_http_connect_handshaker_create(proxy_name, args->server_name));
    gpr_free(proxy_name);
  }
  grpc_channel_security_connector_create_handshakers(
      exec_ctx, c->security_connector, c->handshake_mgr);
  gpr_mu_init(&c->mu);
  gpr_ref_init(&c->refs, 1);
  grpc_subchannel *s = grpc_subchannel_create(exec_ctx, &c->base, args);
  grpc_connector_unref(exec_ctx, &c->base);
  return s;
}

static grpc_channel *client_channel_factory_create_channel(
    grpc_exec_ctx *exec_ctx, grpc_client_channel_factory *cc_factory,
    const char *target, grpc_client_channel_type type,
    const grpc_channel_args *args) {
  client_channel_factory *f = (client_channel_factory *)cc_factory;
  grpc_channel *channel =
      grpc_channel_create(exec_ctx, target, args, GRPC_CLIENT_CHANNEL, NULL);
  grpc_resolver *resolver = grpc_resolver_create(target, args);
  if (resolver != NULL) {
    grpc_client_channel_finish_initialization(
        exec_ctx, grpc_channel_get_channel_stack(channel), resolver, &f->base);
    GRPC_RESOLVER_UNREF(exec_ctx, resolver, "create");
  } else {
    GRPC_CHANNEL_INTERNAL_UNREF(exec_ctx, channel,
                                "client_channel_factory_create_channel");
    channel = NULL;
  }
  return channel;
}

static const grpc_client_channel_factory_vtable client_channel_factory_vtable =
    {client_channel_factory_ref, client_channel_factory_unref,
     client_channel_factory_create_subchannel,
     client_channel_factory_create_channel};

/* Create a secure client channel:
   Asynchronously: - resolve target
                   - connect to it (trying alternatives as presented)
                   - perform handshakes */
grpc_channel *grpc_secure_channel_create(grpc_channel_credentials *creds,
                                         const char *target,
                                         const grpc_channel_args *args,
                                         void *reserved) {
  grpc_exec_ctx exec_ctx = GRPC_EXEC_CTX_INIT;
  GRPC_API_TRACE(
      "grpc_secure_channel_create(creds=%p, target=%s, args=%p, "
      "reserved=%p)",
      4, (creds, target, args, reserved));
  GPR_ASSERT(reserved == NULL);
  // Make sure security connector does not already exist in args.
  if (grpc_find_security_connector_in_args(args) != NULL) {
    gpr_log(GPR_ERROR, "Cannot set security context in channel args.");
    grpc_exec_ctx_finish(&exec_ctx);
    return grpc_lame_client_channel_create(
        target, GRPC_STATUS_INTERNAL,
        "Security connector exists in channel args.");
  }
  // Create security connector and construct new channel args.
  grpc_channel_security_connector *security_connector;
  grpc_channel_args *new_args_from_connector;
  if (grpc_channel_credentials_create_security_connector(
          creds, target, args, &security_connector, &new_args_from_connector) !=
      GRPC_SECURITY_OK) {
    grpc_exec_ctx_finish(&exec_ctx);
    return grpc_lame_client_channel_create(
        target, GRPC_STATUS_INTERNAL, "Failed to create security connector.");
  }
  grpc_arg connector_arg =
      grpc_security_connector_to_arg(&security_connector->base);
  grpc_channel_args *new_args = grpc_channel_args_copy_and_add(
      new_args_from_connector != NULL ? new_args_from_connector : args,
      &connector_arg, 1);
  if (new_args_from_connector != NULL) {
    grpc_channel_args_destroy(new_args_from_connector);
  }
  // Create client channel factory.
  client_channel_factory *f = gpr_malloc(sizeof(*f));
  memset(f, 0, sizeof(*f));
  f->base.vtable = &client_channel_factory_vtable;
  gpr_ref_init(&f->refs, 1);
  GRPC_SECURITY_CONNECTOR_REF(&security_connector->base,
                              "grpc_secure_channel_create");
  f->security_connector = security_connector;
  // Create channel.
  grpc_channel *channel = client_channel_factory_create_channel(
      &exec_ctx, &f->base, target, GRPC_CLIENT_CHANNEL_TYPE_REGULAR, new_args);
  // Clean up.
  GRPC_SECURITY_CONNECTOR_UNREF(&f->security_connector->base,
                                "secure_client_channel_factory_create_channel");
  grpc_channel_args_destroy(new_args);
  grpc_client_channel_factory_unref(&exec_ctx, &f->base);
  grpc_exec_ctx_finish(&exec_ctx);
  return channel; /* may be NULL */
}
