/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef GRPC_CORE_EXT_FILTERS_CLIENT_CHANNEL_CHANNEL_CONNECTIVITY_INTERNAL_H
#define GRPC_CORE_EXT_FILTERS_CLIENT_CHANNEL_CHANNEL_CONNECTIVITY_INTERNAL_H

#include <grpc/grpc.h>
#include "src/core/lib/channel/channel_stack.h"
#include "src/core/lib/iomgr/exec_ctx.h"

void grpc_channel_watch_connectivity_state_internal(
    grpc_exec_ctx *exec_ctx, grpc_channel_element *client_channel_elem,
    grpc_channel_stack *channel_stack,
    grpc_connectivity_state last_observed_state, gpr_timespec deadline,
    grpc_completion_queue *cq, void *tag);

#endif /* GRPC_CORE_EXT_FILTERS_CLIENT_CHANNEL_CHANNEL_CONNECTIVITY_INTERNAL_H \
        */
