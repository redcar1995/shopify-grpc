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

#ifndef GRPC_CORE_LIB_IOMGR_EV_EPOLLEX_LINUX_H
#define GRPC_CORE_LIB_IOMGR_EV_EPOLLEX_LINUX_H

#include "src/core/lib/iomgr/ev_posix.h"
#include "src/core/lib/iomgr/port.h"

#ifdef __cplusplus
extern "C" {
#endif

const grpc_event_engine_vtable* grpc_init_epollex_linux(
    bool explicitly_requested);

#ifdef __cplusplus
}
#endif

#endif /* GRPC_CORE_LIB_IOMGR_EV_EPOLLEX_LINUX_H */
