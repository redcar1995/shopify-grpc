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

#include <stdint.h>
#include <string.h>

#include <grpc/grpc.h>
#include <grpc/support/alloc.h>
#include <grpc/support/log.h>

#include "src/core/ext/transport/chttp2/transport/hpack_parser.h"
#include "src/core/lib/iomgr/exec_ctx.h"
#include "src/core/lib/slice/slice_internal.h"

bool squelch = true;
bool leak_check = true;

static grpc_error_handle onhdr(grpc_mdelem md) {
  GRPC_MDELEM_UNREF(md);
  return GRPC_ERROR_NONE;
}
static void dont_log(gpr_log_func_args* /*args*/) {}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  grpc_test_only_set_slice_hash_seed(0);
  if (squelch) gpr_set_log_function(dont_log);
  grpc_init();
  {
    grpc_core::ExecCtx exec_ctx;
    grpc_core::HPackParser parser;
    parser.BeginFrame(onhdr, grpc_core::HPackParser::Boundary::None,
                      grpc_core::HPackParser::Priority::None);
    GRPC_ERROR_UNREF(parser.Parse(grpc_slice_from_static_buffer(data, size)));
  }
  grpc_shutdown();
  return 0;
}
