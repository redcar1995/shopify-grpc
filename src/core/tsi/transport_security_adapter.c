/*
 *
 * Copyright 2017, Google Inc.
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

#include "src/core/tsi/transport_security_adapter.h"

#include <string.h>

#include <grpc/support/alloc.h>
#include <grpc/support/log.h>
#include "src/core/tsi/transport_security.h"

#define TSI_ADAPTER_INITIAL_BUFFER_SIZE 256

/* --- tsi_adapter_handshaker_result implementation ---*/

typedef struct {
  tsi_handshaker_result base;
  tsi_handshaker *handshaker;
  unsigned char *unused_bytes;
  size_t unused_bytes_size;
} tsi_adapter_handshaker_result;

static tsi_result tsi_adapter_result_extract_peer(tsi_handshaker_result *self,
                                                  tsi_peer *peer) {
  tsi_adapter_handshaker_result *impl = (tsi_adapter_handshaker_result *)self;
  return tsi_handshaker_extract_peer(impl->handshaker, peer);
}

static tsi_result tsi_adapter_result_create_frame_protector(
    tsi_handshaker_result *self, size_t *max_output_protected_frame_size,
    tsi_frame_protector **protector) {
  tsi_adapter_handshaker_result *impl = (tsi_adapter_handshaker_result *)self;
  return tsi_handshaker_create_frame_protector(
      impl->handshaker, max_output_protected_frame_size, protector);
}

static tsi_result tsi_adapter_result_get_unused_bytes(
    tsi_handshaker_result *self, unsigned char **bytes, size_t *byte_size) {
  tsi_adapter_handshaker_result *impl = (tsi_adapter_handshaker_result *)self;
  *bytes = impl->unused_bytes;
  *byte_size = impl->unused_bytes_size;
  return TSI_OK;
}

static void tsi_adapter_result_destroy(tsi_handshaker_result *self) {
  tsi_adapter_handshaker_result *impl = (tsi_adapter_handshaker_result *)self;
  if (impl->unused_bytes != NULL) {
    gpr_free(impl->unused_bytes);
  }
  gpr_free(self);
}

static const tsi_handshaker_result_vtable result_vtable = {
    tsi_adapter_result_extract_peer, tsi_adapter_result_create_frame_protector,
    tsi_adapter_result_get_unused_bytes, tsi_adapter_result_destroy,
};

tsi_result tsi_adapter_create_handshaker_result(
    tsi_handshaker *handshaker, const unsigned char *unused_bytes,
    size_t unused_bytes_size, tsi_handshaker_result **handshaker_result) {
  if (handshaker == NULL || (unused_bytes_size > 0 && unused_bytes == NULL)) {
    return TSI_INVALID_ARGUMENT;
  }
  tsi_adapter_handshaker_result *impl = gpr_zalloc(sizeof(*impl));
  impl->base.vtable = &result_vtable;
  impl->handshaker = handshaker;
  impl->unused_bytes_size = unused_bytes_size;
  if (unused_bytes_size > 0) {
    impl->unused_bytes = gpr_malloc(unused_bytes_size);
    memcpy(impl->unused_bytes, unused_bytes, unused_bytes_size);
  } else {
    impl->unused_bytes = NULL;
  }
  *handshaker_result = &impl->base;
  return TSI_OK;
}

/* --- tsi_adapter_handshaker implementation ---*/

typedef struct {
  tsi_handshaker base;
  tsi_handshaker *wrapped;
  unsigned char *adapter_buffer;
  size_t adapter_buffer_size;
} tsi_adapter_handshaker;

tsi_result tsi_adapter_get_bytes_to_send_to_peer(tsi_handshaker *self,
                                                 unsigned char *bytes,
                                                 size_t *bytes_size) {
  return tsi_handshaker_get_bytes_to_send_to_peer(
      tsi_adapter_handshaker_get_wrapped(self), bytes, bytes_size);
}

tsi_result tsi_adapter_process_bytes_from_peer(tsi_handshaker *self,
                                               const unsigned char *bytes,
                                               size_t *bytes_size) {
  return tsi_handshaker_process_bytes_from_peer(
      tsi_adapter_handshaker_get_wrapped(self), bytes, bytes_size);
}

tsi_result tsi_adapter_get_result(tsi_handshaker *self) {
  return tsi_handshaker_get_result(tsi_adapter_handshaker_get_wrapped(self));
}

tsi_result tsi_adapter_extract_peer(tsi_handshaker *self, tsi_peer *peer) {
  return tsi_handshaker_extract_peer(tsi_adapter_handshaker_get_wrapped(self),
                                     peer);
}

tsi_result tsi_adapter_create_frame_protector(tsi_handshaker *self,
                                              size_t *max_protected_frame_size,
                                              tsi_frame_protector **protector) {
  return tsi_handshaker_create_frame_protector(
      tsi_adapter_handshaker_get_wrapped(self), max_protected_frame_size,
      protector);
}

void tsi_adapter_destroy(tsi_handshaker *self) {
  tsi_adapter_handshaker *impl = (tsi_adapter_handshaker *)self;
  tsi_handshaker_destroy(impl->wrapped);
  gpr_free(impl->adapter_buffer);
  gpr_free(self);
}

tsi_result tsi_adapter_next(
    tsi_handshaker *self, const unsigned char *received_bytes,
    size_t received_bytes_size, unsigned char **bytes_to_send,
    size_t *bytes_to_send_size, tsi_handshaker_result **handshaker_result,
    tsi_handshaker_on_next_done_cb *cb, void *user_data) {
  /* Input sanity check.  */
  if ((received_bytes_size > 0 && received_bytes == NULL) ||
      bytes_to_send == NULL || bytes_to_send_size == NULL ||
      handshaker_result == NULL) {
    return TSI_INVALID_ARGUMENT;
  }

  /* If there are received bytes, process them first.  */
  tsi_adapter_handshaker *impl = (tsi_adapter_handshaker *)self;
  tsi_result status = TSI_OK;
  size_t bytes_consumed = received_bytes_size;
  if (received_bytes_size > 0) {
    status = tsi_handshaker_process_bytes_from_peer(
        impl->wrapped, received_bytes, &bytes_consumed);
    if (status != TSI_OK) return status;
  }

  /* Get bytes to send to the peer, if available.  */
  size_t offset = 0;
  do {
    size_t to_send_size = impl->adapter_buffer_size - offset;
    status = tsi_handshaker_get_bytes_to_send_to_peer(
        impl->wrapped, impl->adapter_buffer + offset, &to_send_size);
    offset += to_send_size;
    if (status == TSI_INCOMPLETE_DATA) {
      impl->adapter_buffer_size *= 2;
      impl->adapter_buffer =
          gpr_realloc(impl->adapter_buffer, impl->adapter_buffer_size);
    }
  } while (status == TSI_INCOMPLETE_DATA);
  if (status != TSI_OK) return status;
  *bytes_to_send = impl->adapter_buffer;
  *bytes_to_send_size = offset;

  /* If handshake completes, create tsi_handshaker_result.  */
  if (!tsi_handshaker_is_in_progress(impl->wrapped)) {
    size_t unused_bytes_size = received_bytes_size - bytes_consumed;
    const unsigned char *unused_bytes =
        unused_bytes_size == 0 ? NULL : received_bytes + bytes_consumed;
    return tsi_adapter_create_handshaker_result(
        impl->wrapped, unused_bytes, unused_bytes_size, handshaker_result);
  }
  *handshaker_result = NULL;
  return TSI_OK;
}

static const tsi_handshaker_vtable handshaker_vtable = {
    tsi_adapter_get_bytes_to_send_to_peer,
    tsi_adapter_process_bytes_from_peer,
    tsi_adapter_get_result,
    tsi_adapter_extract_peer,
    tsi_adapter_create_frame_protector,
    tsi_adapter_destroy,
    tsi_adapter_next,
};

tsi_handshaker *tsi_create_adapter_handshaker(tsi_handshaker *wrapped) {
  GPR_ASSERT(wrapped != NULL);
  tsi_adapter_handshaker *impl = gpr_zalloc(sizeof(*impl));
  impl->base.vtable = &handshaker_vtable;
  impl->wrapped = wrapped;
  impl->adapter_buffer_size = TSI_ADAPTER_INITIAL_BUFFER_SIZE;
  impl->adapter_buffer = gpr_malloc(impl->adapter_buffer_size);
  return &impl->base;
}

tsi_handshaker *tsi_adapter_handshaker_get_wrapped(tsi_handshaker *adapter) {
  if (adapter == NULL) return NULL;
  tsi_adapter_handshaker *impl = (tsi_adapter_handshaker *)adapter;
  return impl->wrapped;
}
