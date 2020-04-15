# Copyright 2020 gRPC authors.
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
"""The example of using ALTS credentials to setup gRPC client."""

import grpc

import client
import demo_pb2_grpc

SERVER_ADDRESS = "localhost:23333"

def main():
    with grpc.secure_channel(SERVER_ADDRESS, credentials=grpc.alts_channel_credentials()) as channel:
        stub = demo_pb2_grpc.GRPCDemoStub(channel)
        client.simple_method(stub)
        client.client_streaming_method(stub)
        client.server_streaming_method(stub)
        client.bidirectional_streaming_method(stub)


if __name__ == '__main__':
    main()
