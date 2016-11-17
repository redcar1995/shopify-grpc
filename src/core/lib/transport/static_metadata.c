/*
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
 */

/*
 * WARNING: Auto-generated code.
 *
 * To make changes to this file, change
 * tools/codegen/core/gen_static_metadata.py, and then re-run it.
 *
 * See metadata.h for an explanation of the interface here, and metadata.c for
 * an explanation of what's going on.
 */

#include "src/core/lib/transport/static_metadata.h"

static uint8_t g_raw_bytes[] = {
    48,  49,  50,  50,  48,  48,  50,  48,  52,  50,  48,  54,  51,  48,  52,
    52,  48,  48,  52,  48,  52,  53,  48,  48,  97,  99,  99,  101, 112, 116,
    97,  99,  99,  101, 112, 116, 45,  99,  104, 97,  114, 115, 101, 116, 97,
    99,  99,  101, 112, 116, 45,  101, 110, 99,  111, 100, 105, 110, 103, 97,
    99,  99,  101, 112, 116, 45,  108, 97,  110, 103, 117, 97,  103, 101, 97,
    99,  99,  101, 112, 116, 45,  114, 97,  110, 103, 101, 115, 97,  99,  99,
    101, 115, 115, 45,  99,  111, 110, 116, 114, 111, 108, 45,  97,  108, 108,
    111, 119, 45,  111, 114, 105, 103, 105, 110, 97,  103, 101, 97,  108, 108,
    111, 119, 97,  112, 112, 108, 105, 99,  97,  116, 105, 111, 110, 47,  103,
    114, 112, 99,  58,  97,  117, 116, 104, 111, 114, 105, 116, 121, 97,  117,
    116, 104, 111, 114, 105, 122, 97,  116, 105, 111, 110, 99,  97,  99,  104,
    101, 45,  99,  111, 110, 116, 114, 111, 108, 99,  111, 110, 116, 101, 110,
    116, 45,  100, 105, 115, 112, 111, 115, 105, 116, 105, 111, 110, 99,  111,
    110, 116, 101, 110, 116, 45,  101, 110, 99,  111, 100, 105, 110, 103, 99,
    111, 110, 116, 101, 110, 116, 45,  108, 97,  110, 103, 117, 97,  103, 101,
    99,  111, 110, 116, 101, 110, 116, 45,  108, 101, 110, 103, 116, 104, 99,
    111, 110, 116, 101, 110, 116, 45,  108, 111, 99,  97,  116, 105, 111, 110,
    99,  111, 110, 116, 101, 110, 116, 45,  114, 97,  110, 103, 101, 99,  111,
    110, 116, 101, 110, 116, 45,  116, 121, 112, 101, 99,  111, 111, 107, 105,
    101, 100, 97,  116, 101, 100, 101, 102, 108, 97,  116, 101, 100, 101, 102,
    108, 97,  116, 101, 44,  103, 122, 105, 112, 101, 116, 97,  103, 101, 120,
    112, 101, 99,  116, 101, 120, 112, 105, 114, 101, 115, 102, 114, 111, 109,
    71,  69,  84,  103, 114, 112, 99,  103, 114, 112, 99,  45,  97,  99,  99,
    101, 112, 116, 45,  101, 110, 99,  111, 100, 105, 110, 103, 103, 114, 112,
    99,  46,  109, 97,  120, 95,  114, 101, 113, 117, 101, 115, 116, 95,  109,
    101, 115, 115, 97,  103, 101, 95,  98,  121, 116, 101, 115, 103, 114, 112,
    99,  46,  109, 97,  120, 95,  114, 101, 115, 112, 111, 110, 115, 101, 95,
    109, 101, 115, 115, 97,  103, 101, 95,  98,  121, 116, 101, 115, 103, 114,
    112, 99,  46,  116, 105, 109, 101, 111, 117, 116, 103, 114, 112, 99,  46,
    119, 97,  105, 116, 95,  102, 111, 114, 95,  114, 101, 97,  100, 121, 103,
    114, 112, 99,  45,  101, 110, 99,  111, 100, 105, 110, 103, 103, 114, 112,
    99,  45,  105, 110, 116, 101, 114, 110, 97,  108, 45,  101, 110, 99,  111,
    100, 105, 110, 103, 45,  114, 101, 113, 117, 101, 115, 116, 103, 114, 112,
    99,  45,  109, 101, 115, 115, 97,  103, 101, 103, 114, 112, 99,  45,  112,
    97,  121, 108, 111, 97,  100, 45,  98,  105, 110, 103, 114, 112, 99,  45,
    115, 116, 97,  116, 115, 45,  98,  105, 110, 103, 114, 112, 99,  45,  115,
    116, 97,  116, 117, 115, 103, 114, 112, 99,  45,  116, 105, 109, 101, 111,
    117, 116, 103, 114, 112, 99,  45,  116, 114, 97,  99,  105, 110, 103, 45,
    98,  105, 110, 103, 122, 105, 112, 103, 122, 105, 112, 44,  32,  100, 101,
    102, 108, 97,  116, 101, 104, 111, 115, 116, 104, 116, 116, 112, 104, 116,
    116, 112, 115, 105, 100, 101, 110, 116, 105, 116, 121, 105, 100, 101, 110,
    116, 105, 116, 121, 44,  100, 101, 102, 108, 97,  116, 101, 105, 100, 101,
    110, 116, 105, 116, 121, 44,  100, 101, 102, 108, 97,  116, 101, 44,  103,
    122, 105, 112, 105, 100, 101, 110, 116, 105, 116, 121, 44,  103, 122, 105,
    112, 105, 102, 45,  109, 97,  116, 99,  104, 105, 102, 45,  109, 111, 100,
    105, 102, 105, 101, 100, 45,  115, 105, 110, 99,  101, 105, 102, 45,  110,
    111, 110, 101, 45,  109, 97,  116, 99,  104, 105, 102, 45,  114, 97,  110,
    103, 101, 105, 102, 45,  117, 110, 109, 111, 100, 105, 102, 105, 101, 100,
    45,  115, 105, 110, 99,  101, 108, 97,  115, 116, 45,  109, 111, 100, 105,
    102, 105, 101, 100, 108, 98,  45,  99,  111, 115, 116, 45,  98,  105, 110,
    108, 98,  45,  116, 111, 107, 101, 110, 108, 105, 110, 107, 108, 111, 99,
    97,  116, 105, 111, 110, 109, 97,  120, 45,  102, 111, 114, 119, 97,  114,
    100, 115, 58,  109, 101, 116, 104, 111, 100, 58,  112, 97,  116, 104, 80,
    79,  83,  84,  112, 114, 111, 120, 121, 45,  97,  117, 116, 104, 101, 110,
    116, 105, 99,  97,  116, 101, 112, 114, 111, 120, 121, 45,  97,  117, 116,
    104, 111, 114, 105, 122, 97,  116, 105, 111, 110, 80,  85,  84,  114, 97,
    110, 103, 101, 114, 101, 102, 101, 114, 101, 114, 114, 101, 102, 114, 101,
    115, 104, 114, 101, 116, 114, 121, 45,  97,  102, 116, 101, 114, 58,  115,
    99,  104, 101, 109, 101, 115, 101, 114, 118, 101, 114, 115, 101, 116, 45,
    99,  111, 111, 107, 105, 101, 47,  47,  103, 114, 112, 99,  46,  108, 98,
    46,  118, 49,  46,  76,  111, 97,  100, 66,  97,  108, 97,  110, 99,  101,
    114, 47,  66,  97,  108, 97,  110, 99,  101, 76,  111, 97,  100, 47,  105,
    110, 100, 101, 120, 46,  104, 116, 109, 108, 58,  115, 116, 97,  116, 117,
    115, 115, 116, 114, 105, 99,  116, 45,  116, 114, 97,  110, 115, 112, 111,
    114, 116, 45,  115, 101, 99,  117, 114, 105, 116, 121, 116, 101, 116, 114,
    97,  105, 108, 101, 114, 115, 116, 114, 97,  110, 115, 102, 101, 114, 45,
    101, 110, 99,  111, 100, 105, 110, 103, 117, 115, 101, 114, 45,  97,  103,
    101, 110, 116, 118, 97,  114, 121, 118, 105, 97,  119, 119, 119, 45,  97,
    117, 116, 104, 101, 110, 116, 105, 99,  97,  116, 101};

static void static_ref(void *unused) {}
static void static_unref(grpc_exec_ctx *exec_ctx, void *unused) {}
static grpc_slice_refcount g_refcnt = {static_ref, static_unref};

bool grpc_is_static_metadata_string(grpc_slice slice) {
  return slice.refcount != NULL && slice.refcount->ref == static_ref;
}

const grpc_slice grpc_static_slice_table[GRPC_STATIC_MDSTR_COUNT] = {
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 0, .length = 1}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 1, .length = 1}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 2, .length = 1}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 3, .length = 3}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 6, .length = 3}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 9, .length = 3}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 12, .length = 3}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 15, .length = 3}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 18, .length = 3}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 21, .length = 3}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 24, .length = 6}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 30, .length = 14}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 44, .length = 15}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 59, .length = 15}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 74, .length = 13}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 87, .length = 27}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 114, .length = 3}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 117, .length = 5}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 122, .length = 16}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 138, .length = 10}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 148, .length = 13}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 161, .length = 13}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 174, .length = 19}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 193, .length = 16}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 209, .length = 16}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 225, .length = 14}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 239, .length = 16}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 255, .length = 13}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 268, .length = 12}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 280, .length = 6}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 286, .length = 4}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 290, .length = 7}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 297, .length = 12}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 309, .length = 0}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 309, .length = 4}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 313, .length = 6}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 319, .length = 7}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 326, .length = 4}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 330, .length = 3}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 333, .length = 4}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 337, .length = 20}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 357, .length = 30}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 387, .length = 31}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 418, .length = 12}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 430, .length = 19}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 449, .length = 13}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 462, .length = 30}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 492, .length = 12}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 504, .length = 16}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 520, .length = 14}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 534, .length = 11}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 545, .length = 12}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 557, .length = 16}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 573, .length = 4}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 577, .length = 13}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 590, .length = 4}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 594, .length = 4}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 598, .length = 5}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 603, .length = 8}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 611, .length = 16}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 627, .length = 21}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 648, .length = 13}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 661, .length = 8}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 669, .length = 17}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 686, .length = 13}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 699, .length = 8}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 707, .length = 19}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 726, .length = 13}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 739, .length = 11}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 750, .length = 8}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 758, .length = 4}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 762, .length = 8}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 770, .length = 12}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 782, .length = 7}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 789, .length = 5}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 794, .length = 4}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 798, .length = 18}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 816, .length = 19}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 835, .length = 3}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 838, .length = 5}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 843, .length = 7}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 850, .length = 7}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 857, .length = 11}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 868, .length = 7}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 875, .length = 6}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 881, .length = 10}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 891, .length = 1}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 892, .length = 36}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 928, .length = 11}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 939, .length = 7}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 946, .length = 25}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 971, .length = 2}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 973, .length = 8}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 981, .length = 17}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 998, .length = 10}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 1008, .length = 4}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 1012, .length = 3}},
    {.refcount = &g_refcnt,
     .data.refcounted = {.bytes = g_raw_bytes + 1015, .length = 16}},
};

static const uint8_t g_revmap[] = {
    0,   1,   2,   3,   255, 255, 4,   255, 255, 5,   255, 255, 6,   255, 255,
    7,   255, 255, 8,   255, 255, 9,   255, 255, 10,  255, 255, 255, 255, 255,
    11,  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 12,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 13,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 14,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 15,  255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 16,  255, 255, 17,  255, 255,
    255, 255, 18,  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 19,  255, 255, 255, 255, 255, 255, 255, 255, 255, 20,  255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 21,  255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 22,  255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 23,  255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 24,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    25,  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 26,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    27,  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 28,  255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 29,  255, 255, 255, 255,
    255, 30,  255, 255, 255, 31,  255, 255, 255, 255, 255, 255, 32,  255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 34,  255, 255, 255, 35,  255,
    255, 255, 255, 255, 36,  255, 255, 255, 255, 255, 255, 37,  255, 255, 255,
    38,  255, 255, 39,  255, 255, 255, 40,  255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 41,  255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 42,  255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 43,  255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 44,  255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 45,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 46,  255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 47,  255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 48,  255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 49,  255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 50,  255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 51,  255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 52,  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 53,  255, 255, 255, 54,  255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 55,  255, 255, 255, 56,  255, 255, 255, 57,  255,
    255, 255, 255, 58,  255, 255, 255, 255, 255, 255, 255, 59,  255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 60,  255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 61,  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 62,  255, 255, 255, 255, 255, 255, 255, 63,  255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 64,  255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 65,  255, 255, 255, 255, 255,
    255, 255, 66,  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 67,  255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 68,  255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    69,  255, 255, 255, 255, 255, 255, 255, 70,  255, 255, 255, 71,  255, 255,
    255, 255, 255, 255, 255, 72,  255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 73,  255, 255, 255, 255, 255, 255, 74,  255, 255, 255, 255, 75,
    255, 255, 255, 76,  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 77,  255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 78,  255, 255, 79,  255,
    255, 255, 255, 80,  255, 255, 255, 255, 255, 255, 81,  255, 255, 255, 255,
    255, 255, 82,  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 83,  255,
    255, 255, 255, 255, 255, 84,  255, 255, 255, 255, 255, 85,  255, 255, 255,
    255, 255, 255, 255, 255, 255, 86,  87,  255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 88,  255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 89,  255, 255, 255, 255, 255,
    255, 90,  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 91,  255, 92,  255,
    255, 255, 255, 255, 255, 255, 93,  255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 94,  255, 255, 255, 255, 255, 255,
    255, 255, 255, 95,  255, 255, 255, 96,  255, 255, 97,  255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};

int grpc_static_metadata_index(grpc_slice slice) {
  if (GRPC_SLICE_LENGTH(slice) == 0) return 33;
  size_t ofs = (size_t)(GRPC_SLICE_START_PTR(slice) - g_raw_bytes);
  if (ofs > sizeof(g_revmap)) return -1;
  uint8_t id = g_revmap[ofs];
  return id == 255 ? -1 : id;
}

grpc_mdelem grpc_static_mdelem_table[GRPC_STATIC_MDELEM_COUNT];
uintptr_t grpc_static_mdelem_user_data[GRPC_STATIC_MDELEM_COUNT] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 4, 8, 6, 2, 4, 8, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

#define ELEMS_PHASHLEN 0x40
#define ELEMS_PHASHNKEYS 81
#define ELEMS_PHASHRANGE 128
#define ELEMS_PHASHSALT 0x9e3779b9

static const uint8_t elems_tab[] = {
    0,  17, 61, 28, 4,  12, 47, 0,  0,  0,  61, 0,  47, 0,  61, 76,
    61, 70, 76, 0,  0,  10, 4,  60, 0,  0,  0,  16, 88, 47, 1,  76,
    76, 0,  76, 0,  61, 0,  23, 0,  0,  51, 1,  92, 32, 0,  25, 0,
    34, 0,  37, 0,  76, 76, 32, 38, 70, 79, 81, 0,  64, 0,  0,  0,
};

static uint32_t elems_phash(uint32_t val) {
  val -= 917;

  uint32_t a, b, rsl;

  b = (val & 0x3f);
  a = ((val << 18) >> 26);
  rsl = (a ^ elems_tab[b]);
  return rsl;
}

static const uint16_t elem_keys[] = {
    2091, 1405, 8728, 2777, 7192, 2287, 2581, 2483, 2973, 4441, 3561, 3951,
    6403, 4463, 9441, 8726, 2875, 5423, 8730, 7338, 6109, 6207, 6697, 6893,
    7229, 8363, 8729, 3952, 8173, 8191, 8725, 8853, 9245, 9343, 1601, 8727,
    7481, 7340, 7971, 7775, 6501, 3973, 3659, 3979, 3463, 3980, 1307, 8190,
    9010, 8731, 4901, 6599, 3365, 7579, 6795, 9147, 9539, 8069, 6305, 7873,
    1209, 1111, 1699, 1503, 7089, 4468, 2189, 4900, 7232, 2385, 6991, 3978,
    1993, 4902, 2679, 2762, 1013, 3981, 1230, 1895, 8265, 0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0};
static const uint8_t elem_idxs[] = {
    11, 5,  70, 19, 51, 13, 16, 15, 21, 33, 24, 26, 43, 34, 79, 68, 20,
    39, 72, 54, 40, 41, 46, 48, 52, 66, 71, 27, 62, 64, 67, 74, 77, 78,
    7,  69, 56, 55, 60, 58, 44, 28, 25, 30, 23, 31, 4,  63, 75, 73, 37,
    45, 22, 57, 47, 76, 80, 61, 42, 59, 2,  0,  8,  6,  50, 35, 12, 36,
    53, 14, 49, 29, 10, 38, 17, 18, 1,  32, 3,  9,  65};

grpc_mdelem *grpc_static_mdelem_for_static_strings(int a, int b) {
  if (a == -1 || b == -1) return NULL;
  uint32_t k = (uint32_t)(a * 98 + b);
  uint32_t h = elems_phash(k);
  return elem_keys[h] == k ? &grpc_static_mdelem_table[elem_idxs[h]] : NULL;
}

const uint8_t grpc_static_metadata_elem_indices[GRPC_STATIC_MDELEM_COUNT * 2] =
    {11, 33, 10, 33, 12, 33, 12, 54, 13, 33, 14, 33, 15, 33, 16, 33, 17, 33,
     19, 33, 20, 33, 21, 33, 22, 33, 23, 33, 24, 33, 25, 33, 26, 33, 27, 33,
     28, 18, 28, 33, 29, 33, 30, 33, 34, 33, 35, 33, 36, 33, 37, 33, 40, 31,
     40, 32, 40, 53, 40, 58, 40, 59, 40, 60, 40, 61, 45, 31, 45, 53, 45, 58,
     50, 0,  50, 1,  50, 2,  55, 33, 62, 33, 63, 33, 64, 33, 65, 33, 66, 33,
     67, 33, 68, 33, 69, 33, 70, 33, 71, 33, 72, 33, 73, 38, 73, 75, 73, 78,
     74, 86, 74, 88, 76, 33, 77, 33, 79, 33, 80, 33, 81, 33, 82, 33, 83, 39,
     83, 56, 83, 57, 84, 33, 85, 33, 89, 3,  89, 4,  89, 5,  89, 6,  89, 7,
     89, 8,  89, 9,  90, 33, 91, 92, 93, 33, 94, 33, 95, 33, 96, 33, 97, 33};

const uint8_t grpc_static_accept_encoding_metadata[8] = {0,  29, 26, 30,
                                                         28, 32, 27, 31};
