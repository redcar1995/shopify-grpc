# Copyright 2020 The gRPC authors.
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
"""Tests for Simple Stubs."""

# TODO(https://github.com/grpc/grpc/issues/21965): Run under setuptools.

import os

_MAXIMUM_CHANNELS = 10

os.environ["GRPC_PYTHON_MANAGED_CHANNEL_EVICTION_SECONDS"] = "1"
os.environ["GRPC_PYTHON_MANAGED_CHANNEL_MAXIMUM"] = str(_MAXIMUM_CHANNELS)

import contextlib
import datetime
import inspect
import logging
import unittest
import sys
import time
from typing import Callable, Optional

from tests.unit import test_common
from tests.unit import resources
import grpc
import grpc.experimental

_REQUEST = b"0000"

_CACHE_EPOCHS = 8
_CACHE_TRIALS = 6

_SERVER_RESPONSE_COUNT = 10
_CLIENT_REQUEST_COUNT = _SERVER_RESPONSE_COUNT

_STRESS_EPOCHS = _MAXIMUM_CHANNELS * 10

_UNARY_UNARY = "/test/UnaryUnary"
_UNARY_STREAM = "/test/UnaryStream"
_STREAM_UNARY = "/test/StreamUnary"
_STREAM_STREAM = "/test/StreamStream"


@contextlib.contextmanager
def _env(key: str, value: str):
    os.environ[key] = value
    yield
    del os.environ[key]


def _unary_unary_handler(request, context):
    return request


def _unary_stream_handler(request, context):
    for _ in range(_SERVER_RESPONSE_COUNT):
        yield request


def _stream_unary_handler(request_iterator, context):
    request = None
    for single_request in request_iterator:
        request = single_request
    return request


def _stream_stream_handler(request_iterator, context):
    for request in request_iterator:
        yield request


class _GenericHandler(grpc.GenericRpcHandler):

    def service(self, handler_call_details):
        if handler_call_details.method == _UNARY_UNARY:
            return grpc.unary_unary_rpc_method_handler(_unary_unary_handler)
        elif handler_call_details.method == _UNARY_STREAM:
            return grpc.unary_stream_rpc_method_handler(_unary_stream_handler)
        elif handler_call_details.method == _STREAM_UNARY:
            return grpc.stream_unary_rpc_method_handler(_stream_unary_handler)
        elif handler_call_details.method == _STREAM_STREAM:
            return grpc.stream_stream_rpc_method_handler(_stream_stream_handler)
        else:
            raise NotImplementedError()


def _time_invocation(to_time: Callable[[], None]) -> datetime.timedelta:
    start = datetime.datetime.now()
    to_time()
    return datetime.datetime.now() - start


@contextlib.contextmanager
def _server(credentials: Optional[grpc.ServerCredentials]):
    try:
        server = test_common.test_server()
        target = '[::]:0'
        if credentials is None:
            port = server.add_insecure_port(target)
        else:
            port = server.add_secure_port(target, credentials)
        server.add_generic_rpc_handlers((_GenericHandler(),))
        server.start()
        yield port
    finally:
        server.stop(None)


class SimpleStubsTest(unittest.TestCase):

    def assert_cached(self, to_check: Callable[[str], None]) -> None:
        """Asserts that a function caches intermediate data/state.

        To be specific, given a function whose caching behavior is
        deterministic in the value of a supplied string, this function asserts
        that, on average, subsequent invocations of the function for a specific
        string are faster than first invocations with that same string.

        Args:
          to_check: A function returning nothing, that caches values based on
            an arbitrary supplied string.
        """
        initial_runs = []
        cached_runs = []
        for epoch in range(_CACHE_EPOCHS):
            runs = []
            text = str(epoch)
            for trial in range(_CACHE_TRIALS):
                runs.append(_time_invocation(lambda: to_check(text)))
            initial_runs.append(runs[0])
            cached_runs.extend(runs[1:])
        average_cold = sum((run for run in initial_runs),
                           datetime.timedelta()) / len(initial_runs)
        average_warm = sum((run for run in cached_runs),
                           datetime.timedelta()) / len(cached_runs)
        self.assertLess(average_warm, average_cold)

    def assert_eventually(self,
                          predicate: Callable[[], bool],
                          *,
                          timeout: Optional[datetime.timedelta] = None,
                          message: Optional[Callable[[], str]] = None) -> None:
        message = message or (lambda: "Proposition did not evaluate to true")
        timeout = timeout or datetime.timedelta(seconds=10)
        end = datetime.datetime.now() + timeout
        while datetime.datetime.now() < end:
            if predicate():
                break
            time.sleep(0.5)
        else:
            self.fail(message() + " after " + str(timeout))

    def test_unary_unary_insecure(self):
        with _server(None) as port:
            target = f'localhost:{port}'
            response = grpc.experimental.unary_unary(
                _REQUEST,
                target,
                _UNARY_UNARY,
                channel_credentials=grpc.experimental.
                insecure_channel_credentials())
            self.assertEqual(_REQUEST, response)

    def test_unary_unary_secure(self):
        with _server(grpc.local_server_credentials()) as port:
            target = f'localhost:{port}'
            response = grpc.experimental.unary_unary(
                _REQUEST,
                target,
                _UNARY_UNARY,
                channel_credentials=grpc.local_channel_credentials())
            self.assertEqual(_REQUEST, response)

    def test_channels_cached(self):
        with _server(grpc.local_server_credentials()) as port:
            target = f'localhost:{port}'
            test_name = inspect.stack()[0][3]
            args = (_REQUEST, target, _UNARY_UNARY)
            kwargs = {"channel_credentials": grpc.local_channel_credentials()}

            def _invoke(seed: str):
                run_kwargs = dict(kwargs)
                run_kwargs["options"] = ((test_name + seed, ""),)
                grpc.experimental.unary_unary(*args, **run_kwargs)

            self.assert_cached(_invoke)

    def test_channels_evicted(self):
        with _server(grpc.local_server_credentials()) as port:
            target = f'localhost:{port}'
            response = grpc.experimental.unary_unary(
                _REQUEST,
                target,
                _UNARY_UNARY,
                channel_credentials=grpc.local_channel_credentials())
            self.assert_eventually(
                lambda: grpc._simple_stubs.ChannelCache.get(
                )._test_only_channel_count() == 0,
                message=lambda:
                f"{grpc._simple_stubs.ChannelCache.get()._test_only_channel_count()} remain"
            )

    def test_total_channels_enforced(self):
        with _server(grpc.local_server_credentials()) as port:
            target = f'localhost:{port}'
            for i in range(_STRESS_EPOCHS):
                # Ensure we get a new channel each time.
                options = (("foo", str(i)),)
                # Send messages at full blast.
                grpc.experimental.unary_unary(
                    _REQUEST,
                    target,
                    _UNARY_UNARY,
                    options=options,
                    channel_credentials=grpc.local_channel_credentials())
                self.assert_eventually(
                    lambda: grpc._simple_stubs.ChannelCache.get(
                    )._test_only_channel_count() <= _MAXIMUM_CHANNELS + 1,
                    message=lambda:
                    f"{grpc._simple_stubs.ChannelCache.get()._test_only_channel_count()} channels remain"
                )

    def test_unary_stream(self):
        with _server(grpc.local_server_credentials()) as port:
            target = f'localhost:{port}'
            for response in grpc.experimental.unary_stream(
                    _REQUEST,
                    target,
                    _UNARY_STREAM,
                    channel_credentials=grpc.local_channel_credentials()):
                self.assertEqual(_REQUEST, response)

    def test_stream_unary(self):

        def request_iter():
            for _ in range(_CLIENT_REQUEST_COUNT):
                yield _REQUEST

        with _server(grpc.local_server_credentials()) as port:
            target = f'localhost:{port}'
            response = grpc.experimental.stream_unary(
                request_iter(),
                target,
                _STREAM_UNARY,
                channel_credentials=grpc.local_channel_credentials())
            self.assertEqual(_REQUEST, response)

    def test_stream_stream(self):

        def request_iter():
            for _ in range(_CLIENT_REQUEST_COUNT):
                yield _REQUEST

        with _server(grpc.local_server_credentials()) as port:
            target = f'localhost:{port}'
            for response in grpc.experimental.stream_stream(
                    request_iter(),
                    target,
                    _STREAM_STREAM,
                    channel_credentials=grpc.local_channel_credentials()):
                self.assertEqual(_REQUEST, response)

    def test_default_ssl(self):
        _private_key = resources.private_key()
        _certificate_chain = resources.certificate_chain()
        _server_certs = ((_private_key, _certificate_chain),)
        _server_host_override = 'foo.test.google.fr'
        _test_root_certificates = resources.test_root_certificates()
        _property_options = ((
            'grpc.ssl_target_name_override',
            _server_host_override,
        ),)
        cert_dir = os.path.join(os.path.dirname(resources.__file__),
                                "credentials")
        cert_file = os.path.join(cert_dir, "ca.pem")
        with _env("GRPC_DEFAULT_SSL_ROOTS_FILE_PATH", cert_file):
            server_creds = grpc.ssl_server_credentials(_server_certs)
            with _server(server_creds) as port:
                target = f'localhost:{port}'
                response = grpc.experimental.unary_unary(
                    _REQUEST, target, _UNARY_UNARY, options=_property_options)


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    unittest.main(verbosity=2)
