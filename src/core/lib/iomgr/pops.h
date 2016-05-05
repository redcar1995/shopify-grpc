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

#ifndef GRPC_CORE_LIB_IOMGR_POPS_H
#define GRPC_CORE_LIB_IOMGR_POPS_H

#include "src/core/lib/iomgr/pollset.h"
#include "src/core/lib/iomgr/pollset_set.h"

/* A grpc_pops is a pollset-or-pollset_set container. It allows functions that
 * accept a pollset XOR a pollset_set to do so through an abstract interface.
 * No ownership is taken. */

typedef struct grpc_pops {
  union {
    grpc_pollset *pollset;
    grpc_pollset_set *pollset_set;
  } pops;
  enum pops_tag { POPS_NONE, POPS_POLLSET, POPS_POLLSET_SET } tag;
} grpc_pops;

grpc_pops grpc_pops_create_from_pollset_set(grpc_pollset_set *pollset_set);
grpc_pops grpc_pops_create_from_pollset(grpc_pollset *pollset);

/** If \a pops contains a pollset, return it. Otherwise, return NULL */
grpc_pollset *grpc_pops_pollset(grpc_pops *pops);

/** If \a pops contains a pollset_set, return it. Otherwise, return NULL */
grpc_pollset_set *grpc_pops_pollset_set(grpc_pops *pops);

bool grpc_pops_is_empty(const grpc_pops *pops);

/** Add the pollset or pollset_set in \a pops to the destination pollset_set \a
 * pss_dst */
void grpc_pops_add_to_pollset_set(grpc_exec_ctx *exec_ctx, grpc_pops *pops,
                                  grpc_pollset_set *pss_dst);

/** Delete the pollset or pollset_set in \a pops from the destination
 * pollset_set \a
 * pss_dst */
void grpc_pops_del_to_pollset_set(grpc_exec_ctx *exec_ctx, grpc_pops *pops,
                                  grpc_pollset_set *pss_dst);
/* pollset_set specific */

#endif /* GRPC_CORE_LIB_IOMGR_POPS_H */
