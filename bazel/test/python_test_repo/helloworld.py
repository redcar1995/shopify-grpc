# Copyright 2019 the gRPC authors.
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
"""The Python implementation of the GRPC helloworld.Greeter client."""

import contextlib
import datetime
import logging
import unittest

import grpc

import duration_pb2
import helloworld_pb2
import helloworld_pb2_grpc

_HOST = 'localhost'
_SERVER_ADDRESS = '{}:0'.format(_HOST)


class Greeter(helloworld_pb2_grpc.GreeterServicer):

    def SayHello(self, request, context):
        request_in_flight = datetime.now() - request.request_initation.ToDatetime()
        request_duration = duration_pb2.Duration()
        request_duration.FromTimedelta(request_in_flight)
        return helloworld_pb2.HelloReply(
                message='Hello, %s!' % request.name,
                request_duration=request_duration,
        )


@contextlib.contextmanager
def _listening_server():
    server = grpc.server(futures.ThreadPoolExecutor())
    helloworld_pb2_grpc.add_GreeterServicer_to_server(Greeter(), server)
    port = server.add_insecure_port(_SERVER_ADDRESS)
    server.start()
    try:
        yield port
    finally:
        server.stop(0)


class ImportTest(unittest.TestCase):
    def run():
        with _listening_server() as port:
            with grpc.insecure_channel('{}:{}'.format(_HOST, port)) as channel:
                stub = helloworld_pb2_grpc.GreeterStub(channel)
                request_timestamp = timestamp_pb2.Timestamp()
                request_timestamp.GetCurrentTime()
                response = stub.SayHello(helloworld_pb2.HelloRequest(
                                            name='you',
                                            request_initiation=request_timestamp,
                                        ),
                                         wait_for_ready=True)
                self.assertEqual(response.message, "Hello, you!")
                self.assertGreater(response.request_duration.microseconds, 0)


if __name__ == '__main__':
    logging.basicConfig()
    unittest.main()
