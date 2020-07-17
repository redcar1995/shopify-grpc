# Copyright 2018 The gRPC Authors
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
"""Example gRPC client that gets/sets metadata (HTTP2 headers)"""

from __future__ import print_function
import logging
import os
import sys

import grpc

sys.path.insert(0, os.path.join(os.path.dirname(__file__), "../.."))
protos, services = grpc.protos_and_services("protos/helloworld.proto")


def run():
    # NOTE(gRPC Python Team): .close() is possible on a channel and should be
    # used in circumstances in which the with statement does not fit the needs
    # of the code.
    with grpc.insecure_channel('localhost:50051') as channel:
        stub = services.GreeterStub(channel)
        response, call = stub.SayHello.with_call(
            protos.HelloRequest(name='you'),
            metadata=(
                ('initial-metadata-1', 'The value should be str'),
                ('binary-metadata-bin',
                 b'With -bin surffix, the value can be bytes'),
                ('accesstoken', 'gRPC Python is great'),
            ))

    print("Greeter client received: " + response.message)
    for key, value in call.trailing_metadata():
        print('Greeter client received trailing metadata: key=%s value=%s' %
              (key, value))


if __name__ == '__main__':
    logging.basicConfig()
    run()
