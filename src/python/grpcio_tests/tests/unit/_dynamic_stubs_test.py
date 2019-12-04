# Copyright 2019 The gRPC authors.
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
"""Test of dynamic stub import API."""

import unittest
import logging
import contextlib
import sys
import multiprocessing
import functools

# TODO: Support setup.py as test runner.
# TODO: Support Blaze as test runner.

@contextlib.contextmanager
def _grpc_tools_unimportable():
    original_sys_path = sys.path
    sys.path = [path for path in sys.path if "grpcio_tools" not in path]
    try:
        yield
    finally:
        sys.path = original_sys_path


# TODO: Dedupe with grpc_tools test?
def _wrap_in_subprocess(error_queue, fn):
    @functools.wraps(fn)
    def _wrapped():
        try:
            fn()
        except Exception as e:
            error_queue.put(e)
            raise
    return _wrapped


def _run_in_subprocess(test_case):
    error_queue = multiprocessing.Queue()
    wrapped_case = _wrap_in_subprocess(error_queue, test_case)
    proc = multiprocessing.Process(target=wrapped_case)
    proc.start()
    proc.join()
    if not error_queue.empty():
        raise error_queue.get()
    assert proc.exitcode == 0, "Process exited with code {}".format(proc.exitcode)


def _test_sunny_day():
    import grpc
    protos, services = grpc.protos_and_services("tests/unit/data/foo/bar.proto")
    assert protos.BarMessage is not None
    assert services.BarStub is not None


def _test_grpc_tools_unimportable():
    with _grpc_tools_unimportable():
        import grpc
        try:
            protos, services = grpc.protos_and_services("tests/unit/data/foo/bar.proto")
        except NotImplementedError as e:
            assert "grpcio-tools" in str(e)
        else:
            assert False, "Did not raise NotImplementedError"


class DynamicStubTest(unittest.TestCase):

    def test_sunny_day(self):
        _run_in_subprocess(_test_sunny_day)

    def test_grpc_tools_unimportable(self):
        _run_in_subprocess(_test_grpc_tools_unimportable)

if __name__ == "__main__":
    logging.basicConfig()
    unittest.main(verbosity=2)
