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

#include "src/core/ext/transport/chttp2/transport/hpack_encoder.h"

#include <stdio.h>
#include <string.h>

#include <grpc/support/alloc.h>
#include <grpc/support/log.h>
#include <grpc/support/string_util.h>

#include "src/core/ext/transport/chttp2/transport/hpack_parser.h"
#include "src/core/lib/slice/slice_internal.h"
#include "src/core/lib/slice/slice_string_helpers.h"
#include "src/core/lib/support/string.h"
#include "src/core/lib/transport/metadata.h"
#include "test/core/util/parse_hexstring.h"
#include "test/core/util/slice_splitter.h"
#include "test/core/util/test_config.h"

#define TEST(x) run_test(x, #x)

grpc_chttp2_hpack_compressor g_compressor;
int g_failure = 0;

void **to_delete = NULL;
size_t num_to_delete = 0;
size_t cap_to_delete = 0;

/* verify that the output generated by encoding the stream matches the
   hexstring passed in */
static void verify(grpc_exec_ctx *exec_ctx, size_t window_available, int eof,
                   size_t expect_window_used, const char *expected,
                   size_t nheaders, ...) {
  grpc_slice_buffer output;
  grpc_slice merged;
  grpc_slice expect = parse_hexstring(expected);
  size_t i;
  va_list l;
  grpc_linked_mdelem *e = gpr_malloc(sizeof(*e) * nheaders);
  grpc_metadata_batch b;

  grpc_metadata_batch_init(&b);

  va_start(l, nheaders);
  for (i = 0; i < nheaders; i++) {
    char *key = va_arg(l, char *);
    char *value = va_arg(l, char *);
    if (i) {
      e[i - 1].next = &e[i];
      e[i].prev = &e[i - 1];
    }
    e[i].md =
        grpc_mdelem_from_slices(exec_ctx, grpc_slice_from_copied_string(key),
                                grpc_slice_from_copied_string(value));
  }
  e[0].prev = NULL;
  e[nheaders - 1].next = NULL;
  va_end(l);

  b.list.head = &e[0];
  b.list.tail = &e[nheaders - 1];

  if (cap_to_delete == num_to_delete) {
    cap_to_delete = GPR_MAX(2 * cap_to_delete, 1000);
    to_delete = gpr_realloc(to_delete, sizeof(*to_delete) * cap_to_delete);
  }
  to_delete[num_to_delete++] = e;

  grpc_slice_buffer_init(&output);

  grpc_transport_one_way_stats stats;
  memset(&stats, 0, sizeof(stats));
  grpc_chttp2_encode_header(exec_ctx, &g_compressor, 0xdeadbeef, &b, eof, 16384,
                            &stats, &output);
  merged = grpc_slice_merge(output.slices, output.count);
  grpc_slice_buffer_destroy_internal(exec_ctx, &output);
  grpc_metadata_batch_destroy(exec_ctx, &b);

  if (0 != grpc_slice_cmp(merged, expect)) {
    char *expect_str = grpc_dump_slice(expect, GPR_DUMP_HEX | GPR_DUMP_ASCII);
    char *got_str = grpc_dump_slice(merged, GPR_DUMP_HEX | GPR_DUMP_ASCII);
    gpr_log(GPR_ERROR, "mismatched output for %s", expected);
    gpr_log(GPR_ERROR, "EXPECT: %s", expect_str);
    gpr_log(GPR_ERROR, "GOT:    %s", got_str);
    gpr_free(expect_str);
    gpr_free(got_str);
    g_failure = 1;
  }

  grpc_slice_unref_internal(exec_ctx, merged);
  grpc_slice_unref_internal(exec_ctx, expect);
}

static void test_basic_headers(grpc_exec_ctx *exec_ctx) {
  int i;

  verify(exec_ctx, 0, 0, 0, "000005 0104 deadbeef 40 0161 0161", 1, "a", "a");
  verify(exec_ctx, 0, 0, 0, "000001 0104 deadbeef be", 1, "a", "a");
  verify(exec_ctx, 0, 0, 0, "000001 0104 deadbeef be", 1, "a", "a");
  verify(exec_ctx, 0, 0, 0, "000006 0104 deadbeef be 40 0162 0163", 2, "a", "a",
         "b", "c");
  verify(exec_ctx, 0, 0, 0, "000002 0104 deadbeef bf be", 2, "a", "a", "b",
         "c");
  verify(exec_ctx, 0, 0, 0, "000004 0104 deadbeef 7f 00 0164", 1, "a", "d");

  /* flush out what's there to make a few values look very popular */
  for (i = 0; i < 350; i++) {
    verify(exec_ctx, 0, 0, 0, "000003 0104 deadbeef c0 bf be", 3, "a", "a", "b",
           "c", "a", "d");
  }

  verify(exec_ctx, 0, 0, 0, "000006 0104 deadbeef c0 00 016b 0176", 2, "a", "a",
         "k", "v");
  /* this could be 000004 0104 deadbeef 0f 30 0176 also */
  verify(exec_ctx, 0, 0, 0, "000004 0104 deadbeef 0f 2f 0176", 1, "a", "v");
}

static void encode_int_to_str(int i, char *p) {
  p[0] = (char)('a' + i % 26);
  i /= 26;
  GPR_ASSERT(i < 26);
  p[1] = (char)('a' + i);
  p[2] = 0;
}

static void test_decode_table_overflow(grpc_exec_ctx *exec_ctx) {
  int i;
  char key[3], value[3];
  char *expect;

  for (i = 0; i < 114; i++) {
    encode_int_to_str(i, key);
    encode_int_to_str(i + 1, value);

    if (i + 61 >= 127) {
      gpr_asprintf(&expect,
                   "000009 0104 deadbeef ff%02x 40 02%02x%02x 02%02x%02x",
                   i + 61 - 127, key[0], key[1], value[0], value[1]);
    } else if (i > 0) {
      gpr_asprintf(&expect,
                   "000008 0104 deadbeef %02x 40 02%02x%02x 02%02x%02x",
                   0x80 + 61 + i, key[0], key[1], value[0], value[1]);
    } else {
      gpr_asprintf(&expect, "000007 0104 deadbeef 40 02%02x%02x 02%02x%02x",
                   key[0], key[1], value[0], value[1]);
    }

    if (i > 0) {
      verify(exec_ctx, 0, 0, 0, expect, 2, "aa", "ba", key, value);
    } else {
      verify(exec_ctx, 0, 0, 0, expect, 1, key, value);
    }
    gpr_free(expect);
  }

  /* if the above passes, then we must have just knocked this pair out of the
     decoder stack, and so we'll be forced to re-encode it */
  verify(exec_ctx, 0, 0, 0, "000007 0104 deadbeef 40 026161 026261", 1, "aa",
         "ba");
}

static void verify_table_size_change_match_elem_size(grpc_exec_ctx *exec_ctx,
                                                     const char *key,
                                                     const char *value) {
  grpc_slice_buffer output;
  grpc_mdelem *elem =
      grpc_mdelem_from_slices(exec_ctx, grpc_slice_from_copied_string(key),
                              grpc_slice_from_copied_string(value));
  size_t elem_size = grpc_mdelem_get_size_in_hpack_table(elem);
  size_t initial_table_size = g_compressor.table_size;
  grpc_linked_mdelem *e = gpr_malloc(sizeof(*e));
  grpc_metadata_batch b;
  grpc_metadata_batch_init(&b);
  e[0].md = elem;
  e[0].prev = NULL;
  e[0].next = NULL;
  b.list.head = &e[0];
  b.list.tail = &e[0];
  grpc_slice_buffer_init(&output);

  grpc_transport_one_way_stats stats;
  memset(&stats, 0, sizeof(stats));
  grpc_chttp2_encode_header(exec_ctx, &g_compressor, 0xdeadbeef, &b, 0, 16384,
                            &stats, &output);
  grpc_slice_buffer_destroy_internal(exec_ctx, &output);
  grpc_metadata_batch_destroy(exec_ctx, &b);

  GPR_ASSERT(g_compressor.table_size == elem_size + initial_table_size);
  gpr_free(e);
}

static void test_encode_header_size(grpc_exec_ctx *exec_ctx) {
  verify_table_size_change_match_elem_size(exec_ctx, "hello", "world");
  verify_table_size_change_match_elem_size(exec_ctx, "hello-bin", "world");
}

static void run_test(void (*test)(grpc_exec_ctx *exec_ctx), const char *name) {
  gpr_log(GPR_INFO, "RUN TEST: %s", name);
  grpc_exec_ctx exec_ctx = GRPC_EXEC_CTX_INIT;
  grpc_chttp2_hpack_compressor_init(&g_compressor);
  test(&exec_ctx);
  grpc_chttp2_hpack_compressor_destroy(&exec_ctx, &g_compressor);
  grpc_exec_ctx_finish(&exec_ctx);
}

int main(int argc, char **argv) {
  size_t i;
  grpc_test_only_set_slice_hash_seed(0);
  grpc_test_init(argc, argv);
  grpc_init();
  TEST(test_basic_headers);
  TEST(test_decode_table_overflow);
  TEST(test_encode_header_size);
  grpc_shutdown();
  for (i = 0; i < num_to_delete; i++) {
    gpr_free(to_delete[i]);
  }
  return g_failure;
}
