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

#ifndef __GRPC_INTERNAL_TRANSPORT_CHTTP2_VARINT_H__
#define __GRPC_INTERNAL_TRANSPORT_CHTTP2_VARINT_H__

#include <grpc/support/port_platform.h>

/* Helpers for hpack varint encoding */

/* length of a value that needs varint tail encoding (it's bigger than can be
   bitpacked into the opcode byte) - returned value includes the length of the
   opcode byte */
int grpc_chttp2_hpack_varint_length(gpr_uint32 tail_value);

void grpc_chttp2_hpack_write_varint_tail(gpr_uint32 tail_value,
                                         gpr_uint8* target, int tail_length);

/* maximum value that can be bitpacked with the opcode if the opcode has a
   prefix
   of length prefix_bits */
#define GRPC_CHTTP2_MAX_IN_PREFIX(prefix_bits) ((1 << (8 - (prefix_bits))) - 1)

/* length required to bitpack a value */
#define GRPC_CHTTP2_VARINT_LENGTH(n, prefix_bits) \
  ((n) < GRPC_CHTTP2_MAX_IN_PREFIX(prefix_bits)   \
       ? 1                                        \
       : grpc_chttp2_hpack_varint_length(         \
             (n) - GRPC_CHTTP2_MAX_IN_PREFIX(prefix_bits)))

#define GRPC_CHTTP2_WRITE_VARINT(n, prefix_bits, prefix_or, target, length) \
  do {                                                                      \
    gpr_uint8* tgt = target;                                                \
    if ((length) == 1) {                                                    \
      (tgt)[0] = (prefix_or) | (n);                                         \
    } else {                                                                \
      (tgt)[0] = (prefix_or) | GRPC_CHTTP2_MAX_IN_PREFIX(prefix_bits);      \
      grpc_chttp2_hpack_write_varint_tail(                                  \
          (n) - GRPC_CHTTP2_MAX_IN_PREFIX(prefix_bits), (tgt) + 1,          \
          (length) - 1);                                                    \
    }                                                                       \
  } while (0)

#endif /* __GRPC_INTERNAL_TRANSPORT_CHTTP2_VARINT_H__ */
