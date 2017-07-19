/*
 *
 * Copyright 2016 gRPC authors.
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

#include "test/core/end2end/end2end_tests.h"

#include <string.h>

#include <grpc/support/alloc.h>
#include <grpc/support/host_port.h>
#include <grpc/support/log.h>
#include <grpc/support/string_util.h>
#include <grpc/support/sync.h>
#include <grpc/support/thd.h>
#include <grpc/support/useful.h>
#include "src/core/ext/filters/client_channel/client_channel.h"
#include "src/core/ext/filters/http/server/http_server_filter.h"
#include "src/core/ext/transport/chttp2/transport/chttp2_transport.h"
#include "src/core/lib/channel/connected_channel.h"
#include "src/core/lib/support/env.h"
#include "src/core/lib/surface/channel.h"
#include "src/core/lib/surface/server.h"
#include "test/core/end2end/fixtures/http_proxy_fixture.h"
#include "test/core/util/port.h"
#include "test/core/util/test_config.h"

typedef struct fullstack_fixture_data {
  char *server_addr;
  grpc_end2end_http_proxy *proxy;
} fullstack_fixture_data;

static grpc_end2end_test_fixture chttp2_create_fixture_fullstack(
    grpc_channel_args *client_args, grpc_channel_args *server_args) {
  grpc_end2end_test_fixture f;
  memset(&f, 0, sizeof(f));
  grpc_exec_ctx exec_ctx = GRPC_EXEC_CTX_INIT;

  fullstack_fixture_data *ffd = gpr_malloc(sizeof(fullstack_fixture_data));
  const int server_port = grpc_pick_unused_port_or_die();
  gpr_join_host_port(&ffd->server_addr, "localhost", server_port);

  /* If we are testing proxy auth, add the proxy auth arg to proxy channel args
   */
  grpc_channel_args *proxy_args = NULL;
  const grpc_arg *proxy_auth_arg = grpc_channel_args_find(
          client_args, GRPC_END2END_HTTP_PROXY_TEST_CONNECT_AUTH_PRESENT);
  if(proxy_auth_arg != NULL) {
    proxy_args = grpc_channel_args_copy_and_add(NULL, proxy_auth_arg, 1);
  }
  ffd->proxy = grpc_end2end_http_proxy_create(proxy_args);
  grpc_channel_args_destroy(&exec_ctx, proxy_args);

  grpc_exec_ctx_finish(&exec_ctx);

  f.fixture_data = ffd;
  f.cq = grpc_completion_queue_create_for_next(NULL);
  f.shutdown_cq = grpc_completion_queue_create_for_pluck(NULL);

  return f;
}

void chttp2_init_client_fullstack(grpc_end2end_test_fixture *f,
                                  grpc_channel_args *client_args) {
  fullstack_fixture_data *ffd = f->fixture_data;
  char *proxy_uri;

  /* If testing for proxy auth, add credentials to proxy uri */
  if(grpc_channel_args_find(
      client_args, GRPC_END2END_HTTP_PROXY_TEST_CONNECT_AUTH_PRESENT) == NULL) {
    gpr_asprintf(&proxy_uri, "http://%s",
                 grpc_end2end_http_proxy_get_proxy_name(ffd->proxy));
  } else {
    gpr_asprintf(&proxy_uri, "http://%s@%s",
                 GRPC_END2END_HTTP_PROXY_TEST_CONNECT_CRED,
                 grpc_end2end_http_proxy_get_proxy_name(ffd->proxy));
  }
  gpr_setenv("http_proxy", proxy_uri);
  gpr_free(proxy_uri);
  f->client = grpc_insecure_channel_create(ffd->server_addr, client_args, NULL);
  GPR_ASSERT(f->client);
}

void chttp2_init_server_fullstack(grpc_end2end_test_fixture *f,
                                  grpc_channel_args *server_args) {
  fullstack_fixture_data *ffd = f->fixture_data;
  if (f->server) {
    grpc_server_destroy(f->server);
  }
  f->server = grpc_server_create(server_args, NULL);
  grpc_server_register_completion_queue(f->server, f->cq, NULL);
  GPR_ASSERT(grpc_server_add_insecure_http2_port(f->server, ffd->server_addr));
  grpc_server_start(f->server);
}

void chttp2_tear_down_fullstack(grpc_end2end_test_fixture *f) {
  fullstack_fixture_data *ffd = f->fixture_data;
  gpr_free(ffd->server_addr);
  grpc_end2end_http_proxy_destroy(ffd->proxy);
  gpr_free(ffd);
}

/* All test configurations */
static grpc_end2end_test_config configs[] = {
    {"chttp2/fullstack", FEATURE_MASK_SUPPORTS_DELAYED_CONNECTION |
                             FEATURE_MASK_SUPPORTS_CLIENT_CHANNEL |
                             FEATURE_MASK_SUPPORTS_AUTHORITY_HEADER,
     chttp2_create_fixture_fullstack, chttp2_init_client_fullstack,
     chttp2_init_server_fullstack, chttp2_tear_down_fullstack},
};

int main(int argc, char **argv) {
  size_t i;

  grpc_test_init(argc, argv);
  grpc_end2end_tests_pre_init();
  grpc_init();

  for (i = 0; i < sizeof(configs) / sizeof(*configs); i++) {
    grpc_end2end_tests(argc, argv, configs[i]);
  }

  grpc_shutdown();

  return 0;
}
