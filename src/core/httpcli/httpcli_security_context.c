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

#include "src/core/httpcli/httpcli_security_context.h"

#include <string.h>

#include "src/core/security/secure_transport_setup.h"
#include "src/core/support/string.h"
#include <grpc/support/alloc.h>
#include <grpc/support/log.h>
#include "src/core/tsi/ssl_transport_security.h"

typedef struct {
  grpc_channel_security_context base;
  tsi_ssl_handshaker_factory *handshaker_factory;
  char *secure_peer_name;
} grpc_httpcli_ssl_channel_security_context;

static void httpcli_ssl_destroy(grpc_security_context *ctx) {
  grpc_httpcli_ssl_channel_security_context *c =
      (grpc_httpcli_ssl_channel_security_context *)ctx;
  if (c->handshaker_factory != NULL) {
    tsi_ssl_handshaker_factory_destroy(c->handshaker_factory);
  }
  if (c->secure_peer_name != NULL) gpr_free(c->secure_peer_name);
  gpr_free(ctx);
}

static grpc_security_status httpcli_ssl_create_handshaker(
    grpc_security_context *ctx, tsi_handshaker **handshaker) {
  grpc_httpcli_ssl_channel_security_context *c =
      (grpc_httpcli_ssl_channel_security_context *)ctx;
  tsi_result result = TSI_OK;
  if (c->handshaker_factory == NULL) return GRPC_SECURITY_ERROR;
  result = tsi_ssl_handshaker_factory_create_handshaker(
      c->handshaker_factory, c->secure_peer_name, handshaker);
  if (result != TSI_OK) {
    gpr_log(GPR_ERROR, "Handshaker creation failed with error %s.",
            tsi_result_to_string(result));
    return GRPC_SECURITY_ERROR;
  }
  return GRPC_SECURITY_OK;
}

static grpc_security_status httpcli_ssl_check_peer(grpc_security_context *ctx,
                                                   tsi_peer peer,
                                                   grpc_security_check_cb cb,
                                                   void *user_data) {
  grpc_httpcli_ssl_channel_security_context *c =
      (grpc_httpcli_ssl_channel_security_context *)ctx;
  grpc_security_status status = GRPC_SECURITY_OK;

  /* Check the peer name. */
  if (c->secure_peer_name != NULL &&
      !tsi_ssl_peer_matches_name(&peer, c->secure_peer_name)) {
    gpr_log(GPR_ERROR, "Peer name %s is not in peer certificate",
            c->secure_peer_name);
    status = GRPC_SECURITY_ERROR;
  }
  tsi_peer_destruct(&peer);
  return status;
}

static grpc_security_context_vtable httpcli_ssl_vtable = {
    httpcli_ssl_destroy, httpcli_ssl_create_handshaker, httpcli_ssl_check_peer};

grpc_security_status grpc_httpcli_ssl_channel_security_context_create(
    const unsigned char *pem_root_certs, size_t pem_root_certs_size,
    const char *secure_peer_name, grpc_channel_security_context **ctx) {
  tsi_result result = TSI_OK;
  grpc_httpcli_ssl_channel_security_context *c;

  if (secure_peer_name != NULL && pem_root_certs == NULL) {
    gpr_log(GPR_ERROR,
            "Cannot assert a secure peer name without a trust root.");
    return GRPC_SECURITY_ERROR;
  }

  c = gpr_malloc(sizeof(grpc_httpcli_ssl_channel_security_context));
  memset(c, 0, sizeof(grpc_httpcli_ssl_channel_security_context));

  gpr_ref_init(&c->base.base.refcount, 1);
  c->base.base.is_client_side = 1;
  c->base.base.vtable = &httpcli_ssl_vtable;
  if (secure_peer_name != NULL) {
    c->secure_peer_name = gpr_strdup(secure_peer_name);
  }
  result = tsi_create_ssl_client_handshaker_factory(
      NULL, 0, NULL, 0, pem_root_certs, pem_root_certs_size, NULL, NULL, NULL,
      0, &c->handshaker_factory);
  if (result != TSI_OK) {
    gpr_log(GPR_ERROR, "Handshaker factory creation failed with %s.",
            tsi_result_to_string(result));
    httpcli_ssl_destroy(&c->base.base);
    *ctx = NULL;
    return GRPC_SECURITY_ERROR;
  }
  *ctx = &c->base;
  return GRPC_SECURITY_OK;
}