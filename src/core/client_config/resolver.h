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

#ifndef GRPC_INTERNAL_CORE_CLIENT_CONFIG_RESOLVER_H
#define GRPC_INTERNAL_CORE_CLIENT_CONFIG_RESOLVER_H

typedef struct grpc_resolver grpc_resolver;
typedef struct grpc_resolver_vtable grpc_resolver_vtable;

/** grpc_resolver provides grpc_client_config objects to grpc_channel
    objects */
struct grpc_resolver {
  const grpc_resolver_vtable *vtable;
};

struct grpc_resolver_vtable {
  void (*ref)(grpc_resolver *resolver);
  void (*unref)(grpc_resolver *resolver);
  void (*shutdown)(grpc_resolver *resolver);
  void (*channel_saw_error)(grpc_resolver *resolver,
                            struct sockaddr *failing_address);
  void (*next)(grpc_resolver *resolver, grpc_client_config **target_config,
               grpc_iomgr_closure *on_complete);
};

void grpc_resolver_ref(grpc_resolver *resolver);
void grpc_resolver_unref(grpc_resolver *resolver);
void grpc_resolver_shutdown(grpc_resolver *resolver);

/** Notification that the channel has seen an error on some address.
    Can be used as a hint that re-resolution is desirable soon. */
void grpc_resolver_channel_saw_error(grpc_resolver *resolver,
                                     struct sockaddr *failing_address);

/** Get the next client config. Called by the channel to fetch a new
    configuration. Expected to set *target_config with a new configuration,
    and then schedule on_complete for execution.

    If resolution is fatally broken, set *target_config to NULL and
    schedule on_complete. */
void grpc_resolver_next(grpc_resolver *resolver,
                        grpc_client_config **target_config,
                        grpc_iomgr_closure *on_complete);

#endif /* GRPC_INTERNAL_CORE_CONFIG_RESOLVER_H */
