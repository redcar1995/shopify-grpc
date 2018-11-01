# Copyright 2018 gRPC authors.
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
"""Test of gRPC Python's interaction with the python logging module"""

import unittest
import six
from six.moves import reload_module
import logging
import grpc
import functools
import sys


def patch_stderr(f):

    @functools.wraps(f)
    def _impl(*args, **kwargs):
        old_stderr = sys.stderr
        sys.stderr = six.StringIO()
        try:
            f(*args, **kwargs)
        finally:
            sys.stderr = old_stderr

    return _impl


class LoggingTest(unittest.TestCase):

    def test_logger_not_occupied(self):
        self.assertEqual(0, len(logging.getLogger().handlers))

    @patch_stderr
    def test_handler_found(self):
        try:
            reload_module(logging)
            reload_module(grpc)
            self.assertFalse(
                "No handlers could be found" in sys.stderr.getvalue())
        finally:
            reload_module(logging)

    def test_can_configure_logger(self):
        reload_module(logging)
        reload_module(grpc)
        try:
            intended_stream = six.StringIO()
            logging.basicConfig(stream=intended_stream)
            self.assertEqual(1, len(logging.getLogger().handlers))
            self.assertTrue(
                logging.getLogger().handlers[0].stream is intended_stream)
        finally:
            reload_module(logging)


if __name__ == '__main__':
    unittest.main(verbosity=2)
