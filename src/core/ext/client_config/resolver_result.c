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

#include "src/core/ext/client_config/resolver_result.h"

#include <string.h>

#include <grpc/support/alloc.h>
#include <grpc/support/string_util.h>

grpc_addresses *grpc_addresses_create(size_t num_addresses) {
  grpc_addresses *addresses = gpr_malloc(sizeof(grpc_addresses));
  addresses->num_addresses = num_addresses;
  const size_t addresses_size = sizeof(grpc_address) * num_addresses;
  addresses->addresses = gpr_malloc(addresses_size);
  memset(addresses->addresses, 0, addresses_size);
  return addresses;
}

grpc_addresses *grpc_addresses_copy(grpc_addresses* addresses) {
  grpc_addresses *new = grpc_addresses_create(addresses->num_addresses);
  memcpy(new->addresses, addresses->addresses,
         sizeof(grpc_address) * addresses->num_addresses);
  return new;
}

void grpc_addresses_set_address(grpc_addresses *addresses, size_t index,
                                void *address, size_t address_len,
                                bool is_balancer) {
  GPR_ASSERT(index < addresses->num_addresses);
  grpc_address *target = &addresses->addresses[index];
  memcpy(target->address.addr, address, address_len);
  target->address.len = address_len;
  target->is_balancer = is_balancer;
}

void grpc_addresses_destroy(grpc_addresses *addresses) {
  gpr_free(addresses->addresses);
  gpr_free(addresses);
}

struct grpc_resolver_result {
  gpr_refcount refs;
  grpc_addresses *addresses;
  char *lb_policy_name;
};

grpc_resolver_result *grpc_resolver_result_create(grpc_addresses *addresses,
                                                  const char *lb_policy_name) {
  grpc_resolver_result *result = gpr_malloc(sizeof(*result));
  memset(result, 0, sizeof(*result));
  gpr_ref_init(&result->refs, 1);
  result->addresses = addresses;
  result->lb_policy_name = gpr_strdup(lb_policy_name);
  return result;
}

void grpc_resolver_result_ref(grpc_resolver_result *result) {
  gpr_ref(&result->refs);
}

void grpc_resolver_result_unref(grpc_exec_ctx *exec_ctx,
                                grpc_resolver_result *result) {
  if (gpr_unref(&result->refs)) {
    grpc_addresses_destroy(result->addresses);
    gpr_free(result->lb_policy_name);
    gpr_free(result);
  }
}

grpc_addresses *grpc_resolver_result_get_addresses(
    grpc_resolver_result *result) {
  return result->addresses;
}

const char *grpc_resolver_result_get_lb_policy_name(
    grpc_resolver_result *result) {
  return result->lb_policy_name;
}
