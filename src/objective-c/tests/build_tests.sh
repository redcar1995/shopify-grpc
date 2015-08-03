#!/bin/bash
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

set -e

cd $(dirname $0)

hash pod 2>/dev/null || { echo >&2 "Cocoapods needs to be installed."; exit 1; }
hash xcodebuild 2>/dev/null || { echo >&2 "XCode command-line tools need to be installed."; exit 1; }

BINDIR=../../../bins/$CONFIG

if [ ! -f $BINDIR/protobuf/protoc ]; then
    hash protoc 2>/dev/null || {
        echo >&2 "Can't find protoc. Make sure run_tests.py is making" \
                 "grpc_objective_c_plugin before calling this script."
        exit 1
    }
    # When protoc is already installed, make doesn't compile one. Put a link
    # there so the podspecs can do codegen using that path.
    mkdir -p $BINDIR/protobuf
    ln -s `which protoc` $BINDIR/protobuf/protoc
fi

[ -f $BINDIR/interop_server ] || {
    echo >&2 "Can't find the test server. Make sure run_tests.py is making" \
             "interop_server before calling this script. It needs to be done" \
             "before because pod install of gRPC renames some C gRPC files" \
             "and not the server's code references to them."
    exit 1
}

pod install
