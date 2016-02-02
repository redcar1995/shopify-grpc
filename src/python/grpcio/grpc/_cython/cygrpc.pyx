# Copyright 2015, Google Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cimport cpython

import pkg_resources
import os.path

# TODO(atash): figure out why the coverage tool gets confused about the Cython
# coverage plugin when the following files don't have a '.pxi' suffix.
include "grpc/_cython/_cygrpc/call.pyx.pxi"
include "grpc/_cython/_cygrpc/channel.pyx.pxi"
include "grpc/_cython/_cygrpc/credentials.pyx.pxi"
include "grpc/_cython/_cygrpc/completion_queue.pyx.pxi"
include "grpc/_cython/_cygrpc/records.pyx.pxi"
include "grpc/_cython/_cygrpc/server.pyx.pxi"

#
# Global state
#

cdef class _ModuleState:

  def __cinit__(self):
    filename = pkg_resources.resource_filename(__name__, '_windows/grpc.def')
    directory = os.path.dirname(filename)
    if not pygrpc_load_core(directory):
      raise ImportError('failed to load core gRPC library')
    grpc_init()

  def __dealloc__(self):
    grpc_shutdown()

_module_state = _ModuleState()

