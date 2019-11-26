"""Tests for protoc."""

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import unittest
import grpc_tools



class ProtocTest(unittest.TestCase):

    # def test_import_protos(self):
    #     protos, services = grpc_tools.import_protos("grpc_tools/simple.proto", "tools/distrib/python/grpcio_tools/")
    #     print(dir(protos))
    #     print(dir(services))

    # # TODO: Ensure that we don't pollute STDOUT by invoking protoc.
    # def test_stdout_pollution(self):
    #     pass

    def test_protoc_in_memory(self):
        print("Running test_protoc_in_memory")
        from grpc_tools import protoc
        import os
        original_dir = os.getcwd()
        # TODO: Completely get rid of this chdir stuff.
        os.chdir(os.path.join(original_dir, "tools/distrib/python/grpcio_tools/"))
        protoc.run_protoc_in_memory("grpc_tools/simple.proto", "")
        os.chdir(original_dir)
        import sys; sys.stdout.flush()
        print("Got to end of test.")


if __name__ == '__main__':
    unittest.main()
