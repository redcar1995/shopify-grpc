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

#include "src/core/client_config/uri_parser.h"

#include <string.h>

#include <grpc/support/log.h>

#include "test/core/util/test_config.h"

static void test_succeeds(const char *uri_text, const char *scheme,
                          const char *authority, const char *path) {
  grpc_uri *uri = grpc_uri_parse(uri_text);
  GPR_ASSERT(uri);
  GPR_ASSERT(0 == strcmp(scheme, uri->scheme));
  GPR_ASSERT(0 == strcmp(authority, uri->authority));
  GPR_ASSERT(0 == strcmp(path, uri->path));
  grpc_uri_destroy(uri);
}

static void test_fails(const char *uri_text) {
  GPR_ASSERT(NULL == grpc_uri_parse(uri_text));
}

int main(int argc, char **argv) {
  grpc_test_init(argc, argv);
  test_succeeds("http://www.google.com", "http", "www.google.com", "");
  test_succeeds("dns:///foo", "dns", "", "/foo");
  test_succeeds("http://www.google.com:90", "http", "www.google.com:90", "");
  test_fails("xyz");
  test_fails("http://www.google.com?why-are-you-using-queries");

  GPR_ASSERT(grpc_has_scheme("http:adfhadf"));
  GPR_ASSERT(grpc_has_scheme("http://adfhadf"));
  GPR_ASSERT(!grpc_has_scheme("adfhadf"));
  return 0;
}
