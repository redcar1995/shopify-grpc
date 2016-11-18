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

#include "src/core/lib/transport/metadata.h"

#include <stdio.h>
#include <string.h>

#include <grpc/grpc.h>
#include <grpc/support/alloc.h>
#include <grpc/support/log.h>
#include <grpc/support/string_util.h>

#include "src/core/ext/transport/chttp2/transport/bin_encoder.h"
#include "src/core/lib/slice/slice_internal.h"
#include "src/core/lib/support/string.h"
#include "src/core/lib/transport/static_metadata.h"
#include "test/core/util/test_config.h"

/* a large number */
#define MANY 10000

static void test_no_op(void) {
  gpr_log(GPR_INFO, "test_no_op");
  grpc_init();
  grpc_shutdown();
}

static grpc_slice maybe_intern(grpc_slice in, bool intern) {
  grpc_slice out = intern ? grpc_slice_intern(in) : grpc_slice_ref(in);
  grpc_slice_unref(in);
  return out;
}

static grpc_slice maybe_dup(grpc_slice in, bool dup) {
  grpc_slice out = dup ? grpc_slice_dup(in) : grpc_slice_ref(in);
  grpc_slice_unref(in);
  return out;
}

static void test_create_metadata(bool intern_keys, bool intern_values) {
  grpc_mdelem m1, *m2, *m3;

  gpr_log(GPR_INFO, "test_create_metadata: intern_keys=%d intern_values=%d",
          intern_keys, intern_values);

  grpc_init();
  grpc_exec_ctx exec_ctx = GRPC_EXEC_CTX_INIT;
  m1 = grpc_mdelem_from_slices(
      &exec_ctx, maybe_intern(grpc_slice_from_static_string("a"), intern_keys),
      maybe_intern(grpc_slice_from_static_string("b"), intern_values));
  m2 = grpc_mdelem_from_slices(
      &exec_ctx, maybe_intern(grpc_slice_from_static_string("a"), intern_keys),
      maybe_intern(grpc_slice_from_static_string("b"), intern_values));
  m3 = grpc_mdelem_from_slices(
      &exec_ctx, maybe_intern(grpc_slice_from_static_string("a"), intern_keys),
      maybe_intern(grpc_slice_from_static_string("c"), intern_values));
  GPR_ASSERT(m1 == m2);
  GPR_ASSERT(m3 != m1);
  GPR_ASSERT(grpc_slice_cmp(m3->key, m1->key) == 0);
  GPR_ASSERT(grpc_slice_cmp(m3->value, m1->value) != 0);
  GPR_ASSERT(grpc_slice_str_cmp(m1->key, "a") == 0);
  GPR_ASSERT(grpc_slice_str_cmp(m1->value, "b") == 0);
  GPR_ASSERT(grpc_slice_str_cmp(m3->value, "c") == 0);
  GRPC_MDELEM_UNREF(&exec_ctx, m1);
  GRPC_MDELEM_UNREF(&exec_ctx, m2);
  GRPC_MDELEM_UNREF(&exec_ctx, m3);
  grpc_exec_ctx_finish(&exec_ctx);
  grpc_shutdown();
}

static void test_create_many_ephemeral_metadata(bool intern_keys,
                                                bool intern_values) {
  char buffer[GPR_LTOA_MIN_BUFSIZE];
  long i;

  gpr_log(
      GPR_INFO,
      "test_create_many_ephemeral_metadata: intern_keys=%d intern_values=%d",
      intern_keys, intern_values);

  grpc_init();
  grpc_exec_ctx exec_ctx = GRPC_EXEC_CTX_INIT;
  /* add, and immediately delete a bunch of different elements */
  for (i = 0; i < MANY; i++) {
    gpr_ltoa(i, buffer);
    GRPC_MDELEM_UNREF(
        &exec_ctx,
        grpc_mdelem_from_slices(
            &exec_ctx,
            maybe_intern(grpc_slice_from_static_string("a"), intern_keys),
            maybe_intern(grpc_slice_from_copied_string(buffer),
                         intern_values)));
  }
  grpc_exec_ctx_finish(&exec_ctx);
  grpc_shutdown();
}

static void test_create_many_persistant_metadata(void) {
  char buffer[GPR_LTOA_MIN_BUFSIZE];
  long i;
  grpc_mdelem *created = gpr_malloc(sizeof(grpc_mdelem) * MANY);
  grpc_mdelem md;

  gpr_log(GPR_INFO, "test_create_many_persistant_metadata");

  grpc_init();
  grpc_exec_ctx exec_ctx = GRPC_EXEC_CTX_INIT;
  /* add phase */
  for (i = 0; i < MANY; i++) {
    gpr_ltoa(i, buffer);
    created[i] = grpc_mdelem_from_slices(
        &exec_ctx, grpc_slice_intern(grpc_slice_from_static_string("a")),
        grpc_slice_intern(grpc_slice_from_copied_string(buffer)));
  }
  /* verify phase */
  for (i = 0; i < MANY; i++) {
    gpr_ltoa(i, buffer);
    md = grpc_mdelem_from_slices(
        &exec_ctx, grpc_slice_intern(grpc_slice_from_static_string("a")),
        grpc_slice_intern(grpc_slice_from_copied_string(buffer)));
    GPR_ASSERT(md == created[i]);
    GRPC_MDELEM_UNREF(&exec_ctx, md);
  }
  /* cleanup phase */
  for (i = 0; i < MANY; i++) {
    GRPC_MDELEM_UNREF(&exec_ctx, created[i]);
  }
  grpc_exec_ctx_finish(&exec_ctx);
  grpc_shutdown();

  gpr_free(created);
}

static void test_spin_creating_the_same_thing(bool intern_keys,
                                              bool intern_values) {
  gpr_log(GPR_INFO,
          "test_spin_creating_the_same_thing: intern_keys=%d intern_values=%d",
          intern_keys, intern_values);

  grpc_init();
  grpc_exec_ctx exec_ctx = GRPC_EXEC_CTX_INIT;
  grpc_mdelem a, *b, *c;
  GRPC_MDELEM_UNREF(
      &exec_ctx,
      a = grpc_mdelem_from_slices(
          &exec_ctx,
          maybe_intern(grpc_slice_from_static_string("a"), intern_keys),
          maybe_intern(grpc_slice_from_static_string("b"), intern_values)));
  GRPC_MDELEM_UNREF(
      &exec_ctx,
      b = grpc_mdelem_from_slices(
          &exec_ctx,
          maybe_intern(grpc_slice_from_static_string("a"), intern_keys),
          maybe_intern(grpc_slice_from_static_string("b"), intern_values)));
  GRPC_MDELEM_UNREF(
      &exec_ctx,
      c = grpc_mdelem_from_slices(
          &exec_ctx,
          maybe_intern(grpc_slice_from_static_string("a"), intern_keys),
          maybe_intern(grpc_slice_from_static_string("b"), intern_values)));
  if (intern_keys && intern_values) {
    GPR_ASSERT(a == b);
    GPR_ASSERT(a == c);
  } else {
    // TODO(ctiller): make this true
    // GPR_ASSERT(a != b);
    // GPR_ASSERT(a != c);
    // GPR_ASSERT(b != c);
  }
  grpc_exec_ctx_finish(&exec_ctx);
  grpc_shutdown();
}

static void test_things_stick_around(void) {
  size_t i, j;
  char *buffer;
  size_t nstrs = 1000;
  grpc_slice *strs = gpr_malloc(sizeof(grpc_slice) * nstrs);
  size_t *shuf = gpr_malloc(sizeof(size_t) * nstrs);
  grpc_slice test;

  gpr_log(GPR_INFO, "test_things_stick_around");

  grpc_init();
  grpc_exec_ctx exec_ctx = GRPC_EXEC_CTX_INIT;

  for (i = 0; i < nstrs; i++) {
    gpr_asprintf(&buffer, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx%" PRIuPTR "x", i);
    strs[i] = grpc_slice_intern(grpc_slice_from_static_string(buffer));
    shuf[i] = i;
    gpr_free(buffer);
  }

  for (i = 0; i < nstrs; i++) {
    grpc_slice_ref_internal(strs[i]);
    grpc_slice_unref_internal(&exec_ctx, strs[i]);
  }

  for (i = 0; i < nstrs; i++) {
    size_t p = (size_t)rand() % nstrs;
    size_t q = (size_t)rand() % nstrs;
    size_t temp = shuf[p];
    shuf[p] = shuf[q];
    shuf[q] = temp;
  }

  for (i = 0; i < nstrs; i++) {
    grpc_slice_unref_internal(&exec_ctx, strs[shuf[i]]);
    for (j = i + 1; j < nstrs; j++) {
      gpr_asprintf(&buffer, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx%" PRIuPTR "x",
                   shuf[j]);
      test = grpc_slice_intern(grpc_slice_from_static_string(buffer));
      GPR_ASSERT(grpc_slice_is_equivalent(test, strs[shuf[j]]));
      grpc_slice_unref_internal(&exec_ctx, test);
      gpr_free(buffer);
    }
  }

  grpc_exec_ctx_finish(&exec_ctx);
  grpc_shutdown();
  gpr_free(strs);
  gpr_free(shuf);
}

static void test_user_data_works(void) {
  int *ud1;
  int *ud2;
  grpc_mdelem md;
  gpr_log(GPR_INFO, "test_user_data_works");

  grpc_init();
  grpc_exec_ctx exec_ctx = GRPC_EXEC_CTX_INIT;
  ud1 = gpr_malloc(sizeof(int));
  *ud1 = 1;
  ud2 = gpr_malloc(sizeof(int));
  *ud2 = 2;
  md = grpc_mdelem_from_slices(
      &exec_ctx, grpc_slice_intern(grpc_slice_from_static_string("abc")),
      grpc_slice_intern(grpc_slice_from_static_string("123")));
  grpc_mdelem_set_user_data(md, gpr_free, ud1);
  grpc_mdelem_set_user_data(md, gpr_free, ud2);
  GPR_ASSERT(grpc_mdelem_get_user_data(md, gpr_free) == ud1);
  GRPC_MDELEM_UNREF(&exec_ctx, md);
  grpc_exec_ctx_finish(&exec_ctx);
  grpc_shutdown();
}

static void verify_ascii_header_size(grpc_exec_ctx *exec_ctx, const char *key,
                                     const char *value, bool intern_key,
                                     bool intern_value) {
  grpc_mdelem elem = grpc_mdelem_from_slices(
      exec_ctx, maybe_intern(grpc_slice_from_static_string(key), intern_key),
      maybe_intern(grpc_slice_from_static_string(value), intern_value));
  size_t elem_size = grpc_mdelem_get_size_in_hpack_table(elem);
  size_t expected_size = 32 + strlen(key) + strlen(value);
  GPR_ASSERT(expected_size == elem_size);
  GRPC_MDELEM_UNREF(exec_ctx, elem);
}

static void verify_binary_header_size(grpc_exec_ctx *exec_ctx, const char *key,
                                      const uint8_t *value, size_t value_len,
                                      bool intern_key, bool intern_value) {
  grpc_mdelem elem = grpc_mdelem_from_slices(
      exec_ctx, maybe_intern(grpc_slice_from_static_string(key), intern_key),
      maybe_intern(grpc_slice_from_static_buffer(value, value_len),
                   intern_value));
  GPR_ASSERT(grpc_is_binary_header(elem->key));
  size_t elem_size = grpc_mdelem_get_size_in_hpack_table(elem);
  grpc_slice value_slice =
      grpc_slice_from_copied_buffer((const char *)value, value_len);
  grpc_slice base64_encoded = grpc_chttp2_base64_encode(value_slice);
  size_t expected_size = 32 + strlen(key) + GRPC_SLICE_LENGTH(base64_encoded);
  GPR_ASSERT(expected_size == elem_size);
  grpc_slice_unref_internal(exec_ctx, value_slice);
  grpc_slice_unref_internal(exec_ctx, base64_encoded);
  GRPC_MDELEM_UNREF(exec_ctx, elem);
}

#define BUFFER_SIZE 64
static void test_mdelem_sizes_in_hpack(bool intern_key, bool intern_value) {
  gpr_log(GPR_INFO, "test_mdelem_size: intern_key=%d intern_value=%d",
          intern_key, intern_value);
  grpc_init();
  grpc_exec_ctx exec_ctx = GRPC_EXEC_CTX_INIT;

  uint8_t binary_value[BUFFER_SIZE] = {0};
  for (uint8_t i = 0; i < BUFFER_SIZE; i++) {
    binary_value[i] = i;
  }

  verify_ascii_header_size(&exec_ctx, "hello", "world", intern_key,
                           intern_value);
  verify_ascii_header_size(&exec_ctx, "hello",
                           "worldxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", intern_key,
                           intern_value);
  verify_ascii_header_size(&exec_ctx, ":scheme", "http", intern_key,
                           intern_value);

  for (uint8_t i = 0; i < BUFFER_SIZE; i++) {
    verify_binary_header_size(&exec_ctx, "hello-bin", binary_value, i,
                              intern_key, intern_value);
  }

  grpc_exec_ctx_finish(&exec_ctx);
  grpc_shutdown();
}

static void test_copied_static_metadata(bool dup_key, bool dup_value) {
  gpr_log(GPR_INFO, "test_static_metadata: dup_key=%d dup_value=%d", dup_key,
          dup_value);
  grpc_init();
  grpc_exec_ctx exec_ctx = GRPC_EXEC_CTX_INIT;

  for (size_t i = 0; i < GRPC_STATIC_MDELEM_COUNT; i++) {
    grpc_mdelem p = &grpc_static_mdelem_table[i];
    grpc_mdelem q = grpc_mdelem_from_slices(
        &exec_ctx, maybe_dup(p->key, dup_key), maybe_dup(p->value, dup_value));
    GPR_ASSERT(p == q);
  }

  grpc_exec_ctx_finish(&exec_ctx);
  grpc_shutdown();
}

int main(int argc, char **argv) {
  grpc_test_init(argc, argv);
  test_no_op();
  for (int k = 0; k <= 1; k++) {
    for (int v = 0; v <= 1; v++) {
      test_create_metadata(k, v);
      test_create_many_ephemeral_metadata(k, v);
      test_spin_creating_the_same_thing(k, v);
      test_mdelem_sizes_in_hpack(k, v);
      test_copied_static_metadata(k, v);
    }
  }
  test_create_many_persistant_metadata();
  test_things_stick_around();
  test_user_data_works();
  return 0;
}
