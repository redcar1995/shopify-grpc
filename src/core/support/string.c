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

#include "src/core/support/string.h"

#include <ctype.h>
#include <stddef.h>
#include <string.h>

#include <grpc/support/alloc.h>
#include <grpc/support/port_platform.h>
#include <grpc/support/useful.h>

char *gpr_strdup(const char *src) {
  char *dst;
  size_t len;

  if (!src) {
    return NULL;
  }

  len = strlen(src) + 1;
  dst = gpr_malloc(len);

  memcpy(dst, src, len);

  return dst;
}

typedef struct {
  size_t capacity;
  size_t length;
  char *data;
} hexout;

static hexout hexout_create(void) {
  hexout r = {0, 0, NULL};
  return r;
}

static void hexout_append(hexout *out, char c) {
  if (out->length == out->capacity) {
    out->capacity = GPR_MAX(8, 2 * out->capacity);
    out->data = gpr_realloc(out->data, out->capacity);
  }
  out->data[out->length++] = c;
}

char *gpr_hexdump(const char *buf, size_t len, gpr_uint32 flags) {
  static const char hex[16] = "0123456789abcdef";
  hexout out = hexout_create();

  const gpr_uint8 *const beg = (const gpr_uint8 *)buf;
  const gpr_uint8 *const end = beg + len;
  const gpr_uint8 *cur;

  for (cur = beg; cur != end; ++cur) {
    if (cur != beg) hexout_append(&out, ' ');
    hexout_append(&out, hex[*cur >> 4]);
    hexout_append(&out, hex[*cur & 0xf]);
  }

  if (flags & GPR_HEXDUMP_PLAINTEXT) {
    cur = beg;
    if (len) hexout_append(&out, ' ');
    hexout_append(&out, '\'');
    for (cur = beg; cur != end; ++cur) {
      hexout_append(&out, isprint(*cur) ? *cur : '.');
    }
    hexout_append(&out, '\'');
  }

  hexout_append(&out, 0);

  return out.data;
}

int gpr_parse_bytes_to_uint32(const char *buf, size_t len, gpr_uint32 *result) {
  gpr_uint32 out = 0;
  gpr_uint32 new;
  size_t i;

  if (len == 0) return 0; /* must have some bytes */

  for (i = 0; i < len; i++) {
    if (buf[i] < '0' || buf[i] > '9') return 0; /* bad char */
    new = 10 * out + (buf[i] - '0');
    if (new < out) return 0; /* overflow */
    out = new;
  }

  *result = out;
  return 1;
}

void gpr_reverse_bytes(char *str, int len) {
  char *p1, *p2;
  for (p1 = str, p2 = str + len - 1; p2 > p1; ++p1, --p2) {
    char temp = *p1;
    *p1 = *p2;
    *p2 = temp;
  }
}

int gpr_ltoa(long value, char *string) {
  int i = 0;
  int neg = value < 0;

  if (value == 0) {
    string[0] = '0';
    string[1] = 0;
    return 1;
  }

  if (neg) value = -value;
  while (value) {
    string[i++] = '0' + value % 10;
    value /= 10;
  }
  if (neg) string[i++] = '-';
  gpr_reverse_bytes(string, i);
  string[i] = 0;
  return i;
}

char *gpr_strjoin(const char **strs, size_t nstrs, size_t *final_length) {
  size_t out_length = 0;
  size_t i;
  char *out;
  for (i = 0; i < nstrs; i++) {
    out_length += strlen(strs[i]);
  }
  out_length += 1;  /* null terminator */
  out = gpr_malloc(out_length);
  out_length = 0;
  for (i = 0; i < nstrs; i++) {
    size_t slen = strlen(strs[i]);
    memcpy(out + out_length, strs[i], slen);
    out_length += slen;
  }
  out[out_length] = 0;
  if (final_length != NULL) {
    *final_length = out_length;
  }
  return out;
}

void gpr_strvec_init(gpr_strvec *sv) {
  memset(sv, 0, sizeof(*sv));
}

void gpr_strvec_destroy(gpr_strvec *sv) {
  size_t i;
  for (i = 0; i < sv->count; i++) {
    gpr_free(sv->strs[i]);
  }
  gpr_free(sv->strs);
}

void gpr_strvec_add(gpr_strvec *sv, char *str) {
  if (sv->count == sv->capacity) {
    sv->capacity = GPR_MAX(sv->capacity + 8, sv->capacity * 2);
    sv->strs = gpr_realloc(sv->strs, sizeof(char*) * sv->capacity);
  }
  sv->strs[sv->count++] = str;
}

char *gpr_strvec_flatten(gpr_strvec *sv, size_t *final_length) {
  return gpr_strjoin((const char**)sv->strs, sv->count, final_length);
}
