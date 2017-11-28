/*
 *
 * Copyright 2016 gRPC authors.
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

#ifndef GRPC_CORE_LIB_SLICE_SLICE_INTERNAL_H
#define GRPC_CORE_LIB_SLICE_SLICE_INTERNAL_H

#include <grpc/slice.h>
#include <grpc/slice_buffer.h>

#include "src/core/lib/iomgr/exec_ctx.h"

#ifdef __cplusplus
extern "C" {
#endif

grpc_slice grpc_slice_ref_internal(grpc_slice slice);
void grpc_slice_unref_internal(grpc_exec_ctx* exec_ctx, grpc_slice slice);
void grpc_slice_buffer_reset_and_unref_internal(grpc_exec_ctx* exec_ctx,
                                                grpc_slice_buffer* sb);
void grpc_slice_buffer_partial_unref_internal(grpc_exec_ctx* exec_ctx,
                                              grpc_slice_buffer* sb,
                                              size_t idx);
void grpc_slice_buffer_destroy_internal(grpc_exec_ctx* exec_ctx,
                                        grpc_slice_buffer* sb);

/* Check if a slice is interned */
bool grpc_slice_is_interned(grpc_slice slice);

void grpc_slice_intern_init(void);
void grpc_slice_intern_shutdown(void);
void grpc_test_only_set_slice_hash_seed(uint32_t key);
// if slice matches a static slice, returns the static slice
// otherwise returns the passed in slice (without reffing it)
// used for surface boundaries where we might receive an un-interned static
// string
grpc_slice grpc_slice_maybe_static_intern(grpc_slice slice,
                                          bool* returned_slice_is_different);
uint32_t grpc_static_slice_hash(grpc_slice s);
int grpc_static_slice_eq(grpc_slice a, grpc_slice b);

#ifdef __cplusplus
}
#endif

#endif /* GRPC_CORE_LIB_SLICE_SLICE_INTERNAL_H */
