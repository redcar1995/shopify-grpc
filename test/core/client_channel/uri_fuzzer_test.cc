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

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <grpc/support/alloc.h>

#include "src/core/ext/filters/client_channel/uri_parser.h"
#include "src/core/lib/iomgr/exec_ctx.h"

bool squelch = true;
bool leak_check = true;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  char* s = static_cast<char*>(gpr_malloc(size + 1));
  memcpy(s, data, size);
  s[size] = 0;

  grpc_core::ExecCtx _local_exec_ctx;
  grpc_uri* x;
  if ((x = grpc_uri_parse(s, 1))) {
    grpc_uri_destroy(x);
  }

  gpr_free(s);
  return 0;
}
