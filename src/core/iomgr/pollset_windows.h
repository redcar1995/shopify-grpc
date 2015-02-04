/*
 *
 * Copyright 2014, Google Inc.
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

#ifndef __GRPC_INTERNAL_IOMGR_POLLSET_WINDOWS_H_
#define __GRPC_INTERNAL_IOMGR_POLLSET_WINDOWS_H_

#include <windows.h>
#include <grpc/support/sync.h>

#include "src/core/iomgr/pollset_kick.h"
#include "src/core/iomgr/socket_windows.h"

/* forward declare only in this file to avoid leaking impl details via
   pollset.h; real users of grpc_fd should always include 'fd_posix.h' and not
   use the struct tag */
struct grpc_fd;

typedef struct grpc_pollset {
  HANDLE iocp;
} grpc_pollset;

#define GRPC_POLLSET_MU(pollset) (NULL)
#define GRPC_POLLSET_CV(pollset) (NULL)

void grpc_pollset_add_handle(grpc_pollset *, grpc_winsocket *);

grpc_pollset *grpc_global_pollset(void);

void grpc_handle_notify_on_write(grpc_winsocket *, void(*cb)(void *, int success),
                                 void *opaque);

void grpc_handle_notify_on_read(grpc_winsocket *, void(*cb)(void *, int success),
                                void *opaque);

#endif /* __GRPC_INTERNAL_IOMGR_POLLSET_WINDOWS_H_ */
