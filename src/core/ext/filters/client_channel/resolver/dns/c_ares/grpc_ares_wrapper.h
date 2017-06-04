/*
 *
 * Copyright 2016, Google Inc.
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

#ifndef GRPC_CORE_EXT_FILTERS_CLIENT_CHANNEL_RESOLVER_DNS_C_ARES_GRPC_ARES_WRAPPER_H
#define GRPC_CORE_EXT_FILTERS_CLIENT_CHANNEL_RESOLVER_DNS_C_ARES_GRPC_ARES_WRAPPER_H

#include "src/core/ext/filters/client_channel/lb_policy_factory.h"
#include "src/core/lib/iomgr/exec_ctx.h"
#include "src/core/lib/iomgr/iomgr.h"
#include "src/core/lib/iomgr/polling_entity.h"
#include "src/core/lib/iomgr/resolve_address.h"

typedef struct grpc_ares_request grpc_ares_request;

/* Asynchronously resolve addr. Use \a default_port if a port isn't designated
   in addr, otherwise use the port in addr. grpc_ares_init() must be called at
   least once before this function. \a on_done may be called directly in this
   function without being scheduled with \a exec_ctx, it must not try to acquire
   locks that are being held by the caller. */
extern void (*grpc_resolve_address_ares)(grpc_exec_ctx *exec_ctx,
                                         const char *addr,
                                         const char *default_port,
                                         grpc_pollset_set *interested_parties,
                                         grpc_closure *on_done,
                                         grpc_resolved_addresses **addresses);

/* Asynchronously resolve addr. It will try to resolve grpclb SRV records in
  addition to the normal address records. For normal address records, it uses
  \a default_port if a port isn't designated in \a addr, otherwise it uses the
  port in \a addr. grpc_ares_init() must be called at least once before this
  function. \a on_done may be called directly in this function without being
  scheduled with \a exec_ctx, it must not try to acquire locks that are being
  held by the caller. */
extern grpc_ares_request *(*grpc_dns_lookup_ares)(
    grpc_exec_ctx *exec_ctx, const char *dns_server, const char *addr,
    const char *default_port, grpc_pollset_set *interested_parties,
    grpc_closure *on_done, grpc_lb_addresses **addresses, bool check_grpclb);

/* Cancel the pending grpc_ares_request \a request */
void grpc_cancel_ares_request(grpc_exec_ctx *exec_ctx,
                              grpc_ares_request *request);

/* Initialize gRPC ares wrapper. Must be called at least once before
   grpc_resolve_address_ares(). */
grpc_error *grpc_ares_init(void);

/* Uninitialized gRPC ares wrapper. If there was more than one previous call to
   grpc_ares_init(), this function uninitializes the gRPC ares wrapper only if
   it has been called the same number of times as grpc_ares_init(). */
void grpc_ares_cleanup(void);

#endif /* GRPC_CORE_EXT_FILTERS_CLIENT_CHANNEL_RESOLVER_DNS_C_ARES_GRPC_ARES_WRAPPER_H \
          */
