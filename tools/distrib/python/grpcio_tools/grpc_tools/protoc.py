#!/usr/bin/env python

# Copyright 2016 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import pkg_resources
import sys

# TODO: Figure out how to add this dependency to setuptools.
import six
import imp

from grpc_tools import _protoc_compiler

def main(command_arguments):
    """Run the protocol buffer compiler with the given command-line arguments.

  Args:
    command_arguments: a list of strings representing command line arguments to
        `protoc`.
  """
    command_arguments = [argument.encode() for argument in command_arguments]
    return _protoc_compiler.run_main(command_arguments)

def get_protos(protobuf_path, include_path):
  files = _protoc_compiler.get_protos(protobuf_path.encode('ascii'), include_path.encode('ascii'))
  modules = []
  # TODO: Ensure pointer equality between two invocations of this function.
  for filename, code in six.iteritems(files):
    module = imp.new_module(filename.decode('ascii'))
    six.exec_(code, module.__dict__)
    modules.append(module)
  return tuple(modules)



if __name__ == '__main__':
    proto_include = pkg_resources.resource_filename('grpc_tools', '_proto')
    sys.exit(main(sys.argv + ['-I{}'.format(proto_include)]))
