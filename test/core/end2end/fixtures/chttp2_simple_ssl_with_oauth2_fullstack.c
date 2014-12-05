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

#include "test/core/end2end/end2end_tests.h"

#include <stdio.h>
#include <string.h>

#include "src/core/channel/channel_args.h"
#include "src/core/eventmanager/em.h"
#include "src/core/security/credentials.h"
#include "src/core/security/security_context.h"
#include <grpc/support/alloc.h>
#include <grpc/support/host_port.h>
#include <grpc/support/log.h>
#include "test/core/util/test_config.h"
#include "test/core/util/port.h"
#include "test/core/end2end/data/ssl_test_data.h"

static grpc_em em;

typedef struct fullstack_secure_fixture_data {
  char *localaddr;
} fullstack_secure_fixture_data;

static grpc_end2end_test_fixture chttp2_create_fixture_secure_fullstack(
    grpc_channel_args *client_args, grpc_channel_args *server_args) {
  grpc_end2end_test_fixture f;
  int port = grpc_pick_unused_port_or_die();
  fullstack_secure_fixture_data *ffd =
      gpr_malloc(sizeof(fullstack_secure_fixture_data));

  gpr_join_host_port(&ffd->localaddr, "localhost", port);

  f.fixture_data = ffd;
  f.client_cq = grpc_completion_queue_create();
  f.server_cq = grpc_completion_queue_create();

  return f;
}

static void chttp2_init_client_secure_fullstack(grpc_end2end_test_fixture *f,
                                                grpc_channel_args *client_args,
                                                grpc_credentials *creds) {
  fullstack_secure_fixture_data *ffd = f->fixture_data;
  f->client = grpc_secure_channel_create(creds, ffd->localaddr, client_args);
  GPR_ASSERT(f->client != NULL);
  grpc_credentials_release(creds);
}

static void chttp2_init_server_secure_fullstack(
    grpc_end2end_test_fixture *f, grpc_channel_args *server_args,
    grpc_server_credentials *server_creds) {
  fullstack_secure_fixture_data *ffd = f->fixture_data;
  f->server =
      grpc_secure_server_create(server_creds, f->server_cq, server_args);
  grpc_server_credentials_release(server_creds);
  grpc_server_add_secure_http2_port(f->server, ffd->localaddr);
  grpc_server_start(f->server);
}

void chttp2_tear_down_secure_fullstack(grpc_end2end_test_fixture *f) {
  fullstack_secure_fixture_data *ffd = f->fixture_data;
  gpr_free(ffd->localaddr);
  gpr_free(ffd);
}

static void chttp2_init_client_simple_ssl_with_oauth2_secure_fullstack(
    grpc_end2end_test_fixture *f, grpc_channel_args *client_args) {
  grpc_credentials *ssl_creds = grpc_ssl_credentials_create(
      test_ca_cert, test_ca_cert_size, NULL, 0, NULL, 0);
  grpc_credentials *oauth2_creds =
      grpc_fake_oauth2_credentials_create("Bearer aaslkfjs424535asdf", 1);
  grpc_credentials *ssl_oauth2_creds =
      grpc_composite_credentials_create(ssl_creds, oauth2_creds);
  grpc_arg ssl_name_override = {GRPC_ARG_STRING,
                                GRPC_SSL_TARGET_NAME_OVERRIDE_ARG,
                                {"foo.test.google.com"}};
  grpc_channel_args *new_client_args =
      grpc_channel_args_copy_and_add(client_args, &ssl_name_override);
  chttp2_init_client_secure_fullstack(f, new_client_args, ssl_oauth2_creds);
  grpc_channel_args_destroy(new_client_args);
  grpc_credentials_release(ssl_creds);
  grpc_credentials_release(oauth2_creds);
}

static void chttp2_init_server_simple_ssl_secure_fullstack(
    grpc_end2end_test_fixture *f, grpc_channel_args *server_args) {
  grpc_server_credentials *ssl_creds = grpc_ssl_server_credentials_create(
      NULL, 0, test_server1_key, test_server1_key_size, test_server1_cert,
      test_server1_cert_size);
  chttp2_init_server_secure_fullstack(f, server_args, ssl_creds);
}

/* All test configurations */

static grpc_end2end_test_config configs[] = {
    {"chttp2/simple_ssl_with_oauth2_fullstack",
     FEATURE_MASK_SUPPORTS_DELAYED_CONNECTION,
     chttp2_create_fixture_secure_fullstack,
     chttp2_init_client_simple_ssl_with_oauth2_secure_fullstack,
     chttp2_init_server_simple_ssl_secure_fullstack,
     chttp2_tear_down_secure_fullstack},
};

int main(int argc, char **argv) {
  size_t i;
  grpc_test_init(argc, argv);

  grpc_init();
  grpc_em_init(&em);

  for (i = 0; i < sizeof(configs) / sizeof(*configs); i++) {
    grpc_end2end_tests(configs[i]);
  }

  GPR_ASSERT(grpc_em_destroy(&em) == GRPC_EM_OK);
  grpc_shutdown();

  return 0;
}
