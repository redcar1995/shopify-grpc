# Proto Compiler CocoaPods podspec

# Copyright 2016, Google Inc.
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

Pod::Spec.new do |s|
  # This pod is only a utility that will be used by other pods _at install time_ (not at compile
  # time). Other pods can access it in their `prepare_command` script, under <pods_root>/<pod name>.
  # Because CocoaPods installs pods in alphabetical order, beginning this pod's name with an
  # exclamation mark ensures that other "regular" pods will be able to find it as it'll be installed
  # before them.
  s.name     = '!ProtoCompiler'
  v = '3.2.0'
  s.version  = v
  s.summary  = 'The Protobuf Compiler (protoc) generates Objective-C files from .proto files'
  s.description = <<-DESC
    This podspec only downloads protoc so that local pods generating protos can execute it as part
    of their prepare_command.
    The generated code will have a dependency on the Protobuf Objective-C runtime of the same
    version. The runtime can be obtained as the "Protobuf" pod.
  DESC
  s.homepage = 'https://github.com/google/protobuf'
  s.license  = {
    :type => 'New BSD',
    :text => <<-LICENSE
      This license applies to all parts of Protocol Buffers except the following:

      - Atomicops support for generic gcc, located in
        src/google/protobuf/stubs/atomicops_internals_generic_gcc.h.
        This file is copyrighted by Red Hat Inc.

      - Atomicops support for AIX/POWER, located in
        src/google/protobuf/stubs/atomicops_internals_power.h.
        This file is copyrighted by Bloomberg Finance LP.

      Copyright 2014, Google Inc.  All rights reserved.

      Redistribution and use in source and binary forms, with or without
      modification, are permitted provided that the following conditions are
      met:

          * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
          * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following disclaimer
      in the documentation and/or other materials provided with the
      distribution.
          * Neither the name of Google Inc. nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
      LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
      A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
      OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
      SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
      LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
      DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
      THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
      (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
      OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

      Code generated by the Protocol Buffer compiler is owned by the owner
      of the input file used when generating it.  This code is not
      standalone and requires a support library to be linked with it.  This
      support library is itself covered by the above license.
    LICENSE
  }
  # "The name and email addresses of the library maintainers, not the Podspec maintainer."
  s.authors  = { 'The Protocol Buffers contributors' => 'protobuf@googlegroups.com' }

  repo = 'google/protobuf'
  file = "protoc-#{v}-osx-x86_64.zip"
  s.source = {
    :http => "https://github.com/#{repo}/releases/download/v#{v}/#{file}",
    # TODO(jcanizales): Add sha1 or sha256
    # :sha1 => '??',
  }

  s.preserve_paths = 'protoc',
                     'google/**/*.proto' # Well-known protobuf types

  # Restrict the protobuf runtime version to the one supported by this version of protoc.
  s.dependency 'Protobuf', '~> 3.0'
  # For the Protobuf dependency not to complain:
  s.ios.deployment_target = '7.0'
  s.osx.deployment_target = '10.9'

  # This is only for local development of protoc: If the Podfile brings this pod from a local
  # directory using `:path`, CocoaPods won't download the zip file and so the compiler won't be
  # present in this pod's directory. We use that knowledge to check for the existence of the file
  # and, if absent, build it from the local sources.
  repo_root = '../..'
  plugin = 'grpc_objective_c_plugin'
  s.prepare_command = <<-CMD
    if [ ! -f bin/protoc ]; then
      cd #{repo_root}
      # This will build protoc from the Protobuf submodule of gRPC, and put it in
      # #{repo_root}/bins/opt/protobuf.
      #
      # TODO(jcanizales): Make won't build protoc from sources if one's locally installed, which
      # _we do not want_. Find a way for this to always build from source.
      make #{plugin}
      cd -
    else
      mv bin/protoc .
      mv include/google .
    fi
  CMD
end
