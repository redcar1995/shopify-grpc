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

#ifndef GRPC_CORE_LIB_TRANSPORT_METADATA_BATCH_H
#define GRPC_CORE_LIB_TRANSPORT_METADATA_BATCH_H

#include <grpc/support/port_platform.h>

#include <stdbool.h>

#include <grpc/grpc.h>
#include <grpc/slice.h>
#include <grpc/support/time.h>
#include "src/core/lib/iomgr/exec_ctx.h"
#include "src/core/lib/transport/metadata.h"
#include "src/core/lib/transport/static_metadata.h"

/** Each grpc_linked_mdelem refers to a particular metadata element by either:
    1) grpc_mdelem md
    2) uint8_t md_index
    md_index is an optional optimization. It can only be used for metadata in
    the hpack static table and is equivalent to the metadata's index in the
    table. If a metadata elem is not contained in the hpack static table, then
    md must be used instead. */
typedef struct grpc_linked_mdelem {
  uint8_t md_index;  // If 0, not used. Else, use this field instead of md.
  grpc_mdelem md;    // If md_index is 0, use this field instead of md_index.
  struct grpc_linked_mdelem* next;
  struct grpc_linked_mdelem* prev;
  void* reserved;
} grpc_linked_mdelem;

typedef struct grpc_mdelem_list {
  size_t count;
  size_t default_count;  // Number of default keys.
  grpc_linked_mdelem* head;
  grpc_linked_mdelem* tail;
} grpc_mdelem_list;

typedef struct grpc_metadata_batch {
  /** Metadata elements in this batch */
  grpc_mdelem_list list;
  grpc_metadata_batch_callouts idx;
  /** Used to calculate grpc-timeout at the point of sending,
      or GRPC_MILLIS_INF_FUTURE if this batch does not need to send a
      grpc-timeout. */
  grpc_millis deadline;
} grpc_metadata_batch;

void grpc_metadata_batch_init(grpc_metadata_batch* batch);
void grpc_metadata_batch_destroy(grpc_metadata_batch* batch);
void grpc_metadata_batch_clear(grpc_metadata_batch* batch);
bool grpc_metadata_batch_is_empty(grpc_metadata_batch* batch);

/* Returns the transport size of the batch. */
size_t grpc_metadata_batch_size(grpc_metadata_batch* batch);

/** Remove \a storage from the batch, unreffing the mdelem contained */
void grpc_metadata_batch_remove(grpc_metadata_batch* batch,
                                grpc_linked_mdelem* storage);

/** Substitute a new mdelem for an old value */
grpc_error* grpc_metadata_batch_substitute(grpc_metadata_batch* batch,
                                           grpc_linked_mdelem* storage,
                                           grpc_mdelem new_value);

void grpc_metadata_batch_set_value(grpc_linked_mdelem* storage,
                                   grpc_slice value);

/** Add \a storage to the beginning of \a batch. storage->md is
    assumed to be valid.
    \a storage is owned by the caller and must survive for the
    lifetime of batch. This usually means it should be around
    for the lifetime of the call. */
grpc_error* grpc_metadata_batch_link_head(grpc_metadata_batch* batch,
                                          grpc_linked_mdelem* storage)
    GRPC_MUST_USE_RESULT;

/** Add \a storage to the end of \a batch. storage->md is
    assumed to be valid.
    \a storage is owned by the caller and must survive for the
    lifetime of batch. This usually means it should be around
    for the lifetime of the call. */
grpc_error* grpc_metadata_batch_link_tail(grpc_metadata_batch* batch,
                                          grpc_linked_mdelem* storage)
    GRPC_MUST_USE_RESULT;

/** Add \a elem_to_add as the first element in \a batch, using
    \a storage as backing storage for the linked list element.
    \a storage is owned by the caller and must survive for the
    lifetime of batch. This usually means it should be around
    for the lifetime of the call.
    Takes ownership of \a elem_to_add */
grpc_error* grpc_metadata_batch_add_head(
    grpc_metadata_batch* batch, grpc_linked_mdelem* storage,
    grpc_mdelem elem_to_add) GRPC_MUST_USE_RESULT;

/** Add the static metadata element associated with \a elem_to_add_index
    as the first element in \a batch, using \a storage as backing storage for 
    the linked list element. \a storage is owned by the caller and must survive 
    for the lifetime of batch. This usually means it should be around
    for the lifetime of the call. 
    Valid indices are in metadata.h (e.g., GRPC_MDELEM_STATUS_200_INDEX).
    This is an optimization in the case where chttp2 is used as the underlying 
    transport. */
grpc_error* grpc_metadata_batch_add_head_static(
    grpc_metadata_batch* batch, grpc_linked_mdelem* storage,
    uint8_t elem_to_add_index) GRPC_MUST_USE_RESULT;

/** Add \a elem_to_add as the last element in \a batch, using
    \a storage as backing storage for the linked list element.
    \a storage is owned by the caller and must survive for the
    lifetime of batch. This usually means it should be around
    for the lifetime of the call.
    Takes ownership of \a elem_to_add */
grpc_error* grpc_metadata_batch_add_tail(
    grpc_metadata_batch* batch, grpc_linked_mdelem* storage,
    grpc_mdelem elem_to_add) GRPC_MUST_USE_RESULT;

/** Add the static metadata element associated with \a elem_to_add_index
    as the last element in \a batch, using \a storage as backing storage for 
    the linked list element. \a storage is owned by the caller and must survive 
    for the lifetime of batch. This usually means it should be around
    for the lifetime of the call. 
    Valid indices are in metadata.h (e.g., GRPC_MDELEM_STATUS_200_INDEX).
    This is an optimization in the case where chttp2 is used as the underlying 
    transport. */
grpc_error* grpc_metadata_batch_add_tail_static(
    grpc_metadata_batch* batch, grpc_linked_mdelem* storage,
    uint8_t index_to_add) GRPC_MUST_USE_RESULT;

grpc_error* grpc_attach_md_to_error(grpc_error* src, grpc_mdelem md);

/** Returns if the index is a valid static hpack table index, and thus if the
    grpc_linked_mdelem stores the index rather than the actual grpc_mdelem **/
bool grpc_metadata_batch_is_valid_mdelem_index(uint8_t index);

/* Static hpack table metadata info */
typedef struct static_hpack_table_metadata_info {
  uint8_t index;  // Index in the static hpack table
  uint8_t size;   // Size of the metadata per RFC-7540 section 6.5.2., including
                  // 32 bytes of padding
  grpc_metadata_batch_callouts_index
      callouts_index;  // For duplicate metadata detection. If
                       // GRPC_BATCH_CALLOUTS_COUNT, then the metadata is not
                       // one of the callouts.
} static_hpack_table_metadata_info;

extern static_hpack_table_metadata_info static_hpack_table_metadata[];

typedef struct {
  grpc_error* error;
  grpc_mdelem md;
} grpc_filtered_mdelem;

#define GRPC_FILTERED_ERROR(error) \
  { (error), GRPC_MDNULL }
#define GRPC_FILTERED_MDELEM(md) \
  { GRPC_ERROR_NONE, (md) }
#define GRPC_FILTERED_REMOVE() \
  { GRPC_ERROR_NONE, GRPC_MDNULL }

typedef grpc_filtered_mdelem (*grpc_metadata_batch_filter_func)(
    void* user_data, grpc_mdelem elem);
grpc_error* grpc_metadata_batch_filter(
    grpc_metadata_batch* batch, grpc_metadata_batch_filter_func func,
    void* user_data, const char* composite_error_string) GRPC_MUST_USE_RESULT;

#ifndef NDEBUG
void grpc_metadata_batch_assert_ok(grpc_metadata_batch* comd);
#else
#define grpc_metadata_batch_assert_ok(comd) \
  do {                                      \
  } while (0)
#endif

/// Copies \a src to \a dst.  \a storage must point to an array of
/// \a grpc_linked_mdelem structs of at least the same size as \a src.
void grpc_metadata_batch_copy(grpc_metadata_batch* src,
                              grpc_metadata_batch* dst,
                              grpc_linked_mdelem* storage);

void grpc_metadata_batch_move(grpc_metadata_batch* src,
                              grpc_metadata_batch* dst);

#endif /* GRPC_CORE_LIB_TRANSPORT_METADATA_BATCH_H */
