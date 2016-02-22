/*
 *
 * Copyright 2015-2016, Google Inc.
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

#ifndef GRPC_INTERNAL_CORE_SURFACE_CHANNEL_STACK_TYPE_H
#define GRPC_INTERNAL_CORE_SURFACE_CHANNEL_STACK_TYPE_H

#include <stdbool.h>

typedef enum {
  // normal top-half client channel with load-balancing, connection management
  GRPC_CLIENT_CHANNEL,
  // abbreviated top-half client channel bound to one subchannel - for internal
  // load balancing implementation
  GRPC_CLIENT_UCHANNEL,
  // bottom-half of a client channel: everything that happens post-load
  // balancing (bound to a specific transport)
  GRPC_CLIENT_SUBCHANNEL,
  // a permanently broken client channel
  GRPC_CLIENT_LAME_CHANNEL,
  // a directly connected client channel (without load-balancing, directly talks
  // to a transport)
  GRPC_CLIENT_DIRECT_CHANNEL,
  // server side channel
  GRPC_SERVER_CHANNEL,
  // must be last
  GRPC_NUM_CHANNEL_STACK_TYPES
} grpc_channel_stack_type;

bool grpc_channel_stack_type_is_client(grpc_channel_stack_type type);

#endif /* GRPC_INTERNAL_CORE_SURFACE_CHANNEL_STACK_TYPE_H */
