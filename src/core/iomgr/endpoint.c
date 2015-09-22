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

#include "src/core/iomgr/endpoint.h"

void grpc_endpoint_read(grpc_endpoint *ep, gpr_slice_buffer *slices,
                        grpc_closure *cb, grpc_closure_list *closure_list) {
  ep->vtable->read(ep, slices, cb, closure_list);
}

void grpc_endpoint_write(grpc_endpoint *ep, gpr_slice_buffer *slices,
                         grpc_closure *cb, grpc_closure_list *closure_list) {
  ep->vtable->write(ep, slices, cb, closure_list);
}

void grpc_endpoint_add_to_pollset(grpc_endpoint *ep, grpc_pollset *pollset,
                                  grpc_closure_list *closure_list) {
  ep->vtable->add_to_pollset(ep, pollset, closure_list);
}

void grpc_endpoint_add_to_pollset_set(grpc_endpoint *ep,
                                      grpc_pollset_set *pollset_set,
                                      grpc_closure_list *closure_list) {
  ep->vtable->add_to_pollset_set(ep, pollset_set, closure_list);
}

void grpc_endpoint_shutdown(grpc_endpoint *ep,
                            grpc_closure_list *closure_list) {
  ep->vtable->shutdown(ep, closure_list);
}

void grpc_endpoint_destroy(grpc_endpoint *ep, grpc_closure_list *closure_list) {
  ep->vtable->destroy(ep, closure_list);
}

char *grpc_endpoint_get_peer(grpc_endpoint *ep) {
  return ep->vtable->get_peer(ep);
}
