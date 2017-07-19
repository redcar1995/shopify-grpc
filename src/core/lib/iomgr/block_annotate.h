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

#ifndef GRPC_CORE_LIB_IOMGR_BLOCK_ANNOTATE_H
#define GRPC_CORE_LIB_IOMGR_BLOCK_ANNOTATE_H

/* These annotations identify the beginning and end of regions where
   the code may block for reasons other than synchronization functions.
   These include poll, epoll, and getaddrinfo. */

#define GRPC_SCHEDULING_START_BLOCKING_REGION \
  do {                                        \
  } while (0)
#define GRPC_SCHEDULING_END_BLOCKING_REGION_NO_EXEC_CTX \
  do {                                                  \
  } while (0)
#define GRPC_SCHEDULING_END_BLOCKING_REGION_WITH_EXEC_CTX(ec) \
  do {                                                        \
    grpc_exec_ctx_invalidate_now((ec));                       \
  } while (0)

#endif /* GRPC_CORE_LIB_IOMGR_BLOCK_ANNOTATE_H */
