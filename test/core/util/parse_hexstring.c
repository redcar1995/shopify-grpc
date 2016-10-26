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

#include "test/core/util/parse_hexstring.h"
#include <grpc/support/log.h>

grpc_slice parse_hexstring(const char *hexstring) {
  size_t nibbles = 0;
  const char *p = 0;
  uint8_t *out;
  uint8_t temp;
  grpc_slice slice;

  for (p = hexstring; *p; p++) {
    nibbles += (*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'f');
  }

  GPR_ASSERT((nibbles & 1) == 0);

  slice = grpc_slice_malloc(nibbles / 2);
  out = GPR_SLICE_START_PTR(slice);

  nibbles = 0;
  temp = 0;
  for (p = hexstring; *p; p++) {
    if (*p >= '0' && *p <= '9') {
      temp = (uint8_t)(temp << 4) | (uint8_t)(*p - '0');
      nibbles++;
    } else if (*p >= 'a' && *p <= 'f') {
      temp = (uint8_t)(temp << 4) | (uint8_t)(*p - 'a' + 10);
      nibbles++;
    }
    if (nibbles == 2) {
      *out++ = temp;
      nibbles = 0;
    }
  }

  return slice;
}
