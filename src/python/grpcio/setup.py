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

"""A setup module for the GRPC Python package."""

import os
import os.path
import sys

from distutils import core as _core
import setuptools

# Ensure we're in the proper directory whether or not we're being used by pip.
os.chdir(os.path.dirname(os.path.abspath(__file__)))

# Break import-style to ensure we can actually find our commands module.
import commands

# Use environment variables to determine whether or not the Cython extension
# should *use* Cython or use the generated C files. Note that this requires the
# C files to have been generated by building first *with* Cython support.
_BUILD_WITH_CYTHON = os.environ.get('GRPC_PYTHON_BUILD_WITH_CYTHON', False)

_C_EXTENSION_SOURCES = (
    'grpc/_adapter/_c/module.c',
    'grpc/_adapter/_c/types.c',
    'grpc/_adapter/_c/utility.c',
    'grpc/_adapter/_c/types/client_credentials.c',
    'grpc/_adapter/_c/types/server_credentials.c',
    'grpc/_adapter/_c/types/completion_queue.c',
    'grpc/_adapter/_c/types/call.c',
    'grpc/_adapter/_c/types/channel.c',
    'grpc/_adapter/_c/types/server.c',
)

_EXTENSION_INCLUDE_DIRECTORIES = (
    '.',
)

_EXTENSION_LIBRARIES = (
    'grpc',
    'gpr',
)
if not "darwin" in sys.platform:
    _EXTENSION_LIBRARIES += ('rt',)


_C_EXTENSION_MODULE = _core.Extension(
    'grpc._adapter._c', sources=list(_C_EXTENSION_SOURCES),
    include_dirs=list(_EXTENSION_INCLUDE_DIRECTORIES),
    libraries=list(_EXTENSION_LIBRARIES),
)
_EXTENSION_MODULES = [_C_EXTENSION_MODULE]

_PACKAGES = (
    setuptools.find_packages('.', exclude=['*._cython', '*._cython.*'])
)

_PACKAGE_DIRECTORIES = {
    '': '.',
}

_INSTALL_REQUIRES = (
    'enum34==1.0.4',
    'futures==2.2.0',
    'protobuf==3.0.0a3',
)

_SETUP_REQUIRES = (
    'sphinx>=1.3',
) + _INSTALL_REQUIRES

_COMMAND_CLASS = {
    'doc': commands.SphinxDocumentation,
}

setuptools.setup(
    name='grpcio',
    version='0.10.0a0',
    ext_modules=_EXTENSION_MODULES,
    packages=list(_PACKAGES),
    package_dir=_PACKAGE_DIRECTORIES,
    install_requires=_INSTALL_REQUIRES,
    setup_requires=_SETUP_REQUIRES,
    cmdclass=_COMMAND_CLASS
)
