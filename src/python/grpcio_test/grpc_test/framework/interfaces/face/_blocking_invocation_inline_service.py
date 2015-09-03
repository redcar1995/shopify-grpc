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

"""Test code for the Face layer of RPC Framework."""

import abc
import unittest

# test_interfaces is referenced from specification in this module.
from grpc.framework.interfaces.face import face
from grpc_test.framework.common import test_constants
from grpc_test.framework.common import test_control
from grpc_test.framework.common import test_coverage
from grpc_test.framework.interfaces.face import _3069_test_constant
from grpc_test.framework.interfaces.face import _digest
from grpc_test.framework.interfaces.face import _stock_service
from grpc_test.framework.interfaces.face import test_interfaces  # pylint: disable=unused-import


class TestCase(test_coverage.Coverage, unittest.TestCase):
  """A test of the Face layer of RPC Framework.

  Concrete subclasses must have an "implementation" attribute of type
  test_interfaces.Implementation and an "invoker_constructor" attribute of type
  _invocation.InvokerConstructor.
  """
  __metaclass__ = abc.ABCMeta

  NAME = 'BlockingInvocationInlineServiceTest'

  def setUp(self):
    """See unittest.TestCase.setUp for full specification.

    Overriding implementations must call this implementation.
    """
    self._control = test_control.PauseFailControl()
    self._digest = _digest.digest(
        _stock_service.STOCK_TEST_SERVICE, self._control, None)

    generic_stub, dynamic_stubs, self._memo = self.implementation.instantiate(
        self._digest.methods, self._digest.inline_method_implementations, None)
    self._invoker = self.invoker_constructor.construct_invoker(
        generic_stub, dynamic_stubs, self._digest.methods)

  def tearDown(self):
    """See unittest.TestCase.tearDown for full specification.

    Overriding implementations must call this implementation.
    """
    self._invoker = None
    self.implementation.destantiate(self._memo)

  def testSuccessfulUnaryRequestUnaryResponse(self):
    for (group, method), test_messages_sequence in (
        self._digest.unary_unary_messages_sequences.iteritems()):
      for test_messages in test_messages_sequence:
        request = test_messages.request()

        response, call = self._invoker.blocking(group, method)(
            request, test_constants.LONG_TIMEOUT, with_call=True)

        test_messages.verify(request, response, self)

  def testSuccessfulUnaryRequestStreamResponse(self):
    for (group, method), test_messages_sequence in (
        self._digest.unary_stream_messages_sequences.iteritems()):
      for test_messages in test_messages_sequence:
        request = test_messages.request()

        response_iterator = self._invoker.blocking(group, method)(
            request, test_constants.LONG_TIMEOUT)
        responses = list(response_iterator)

        test_messages.verify(request, responses, self)

  def testSuccessfulStreamRequestUnaryResponse(self):
    for (group, method), test_messages_sequence in (
        self._digest.stream_unary_messages_sequences.iteritems()):
      for test_messages in test_messages_sequence:
        requests = test_messages.requests()

        response, call = self._invoker.blocking(group, method)(
            iter(requests), test_constants.LONG_TIMEOUT, with_call=True)

        test_messages.verify(requests, response, self)

  def testSuccessfulStreamRequestStreamResponse(self):
    for (group, method), test_messages_sequence in (
        self._digest.stream_stream_messages_sequences.iteritems()):
      for test_messages in test_messages_sequence:
        requests = test_messages.requests()

        response_iterator = self._invoker.blocking(group, method)(
            iter(requests), test_constants.LONG_TIMEOUT)
        responses = list(response_iterator)

        test_messages.verify(requests, responses, self)

  def testSequentialInvocations(self):
    for (group, method), test_messages_sequence in (
        self._digest.unary_unary_messages_sequences.iteritems()):
      for test_messages in test_messages_sequence:
        first_request = test_messages.request()
        second_request = test_messages.request()

        first_response = self._invoker.blocking(group, method)(
            first_request, test_constants.LONG_TIMEOUT)

        test_messages.verify(first_request, first_response, self)

        second_response = self._invoker.blocking(group, method)(
            second_request, test_constants.LONG_TIMEOUT)

        test_messages.verify(second_request, second_response, self)

  @unittest.skip('Parallel invocations impossible with blocking control flow!')
  def testParallelInvocations(self):
    raise NotImplementedError()

  @unittest.skip('Parallel invocations impossible with blocking control flow!')
  def testWaitingForSomeButNotAllParallelInvocations(self):
    raise NotImplementedError()

  @unittest.skip('Cancellation impossible with blocking control flow!')
  def testCancelledUnaryRequestUnaryResponse(self):
    raise NotImplementedError()

  @unittest.skip('Cancellation impossible with blocking control flow!')
  def testCancelledUnaryRequestStreamResponse(self):
    raise NotImplementedError()

  @unittest.skip('Cancellation impossible with blocking control flow!')
  def testCancelledStreamRequestUnaryResponse(self):
    raise NotImplementedError()

  @unittest.skip('Cancellation impossible with blocking control flow!')
  def testCancelledStreamRequestStreamResponse(self):
    raise NotImplementedError()

  def testExpiredUnaryRequestUnaryResponse(self):
    for (group, method), test_messages_sequence in (
        self._digest.unary_unary_messages_sequences.iteritems()):
      for test_messages in test_messages_sequence:
        request = test_messages.request()

        with self._control.pause(), self.assertRaises(
            face.ExpirationError):
          self._invoker.blocking(group, method)(
              request, _3069_test_constant.REALLY_SHORT_TIMEOUT)

  def testExpiredUnaryRequestStreamResponse(self):
    for (group, method), test_messages_sequence in (
        self._digest.unary_stream_messages_sequences.iteritems()):
      for test_messages in test_messages_sequence:
        request = test_messages.request()

        with self._control.pause(), self.assertRaises(
            face.ExpirationError):
          response_iterator = self._invoker.blocking(group, method)(
              request, _3069_test_constant.REALLY_SHORT_TIMEOUT)
          list(response_iterator)

  def testExpiredStreamRequestUnaryResponse(self):
    for (group, method), test_messages_sequence in (
        self._digest.stream_unary_messages_sequences.iteritems()):
      for test_messages in test_messages_sequence:
        requests = test_messages.requests()

        with self._control.pause(), self.assertRaises(
            face.ExpirationError):
          self._invoker.blocking(group, method)(
              iter(requests), _3069_test_constant.REALLY_SHORT_TIMEOUT)

  def testExpiredStreamRequestStreamResponse(self):
    for (group, method), test_messages_sequence in (
        self._digest.stream_stream_messages_sequences.iteritems()):
      for test_messages in test_messages_sequence:
        requests = test_messages.requests()

        with self._control.pause(), self.assertRaises(
            face.ExpirationError):
          response_iterator = self._invoker.blocking(group, method)(
              iter(requests), _3069_test_constant.REALLY_SHORT_TIMEOUT)
          list(response_iterator)

  def testFailedUnaryRequestUnaryResponse(self):
    for (group, method), test_messages_sequence in (
        self._digest.unary_unary_messages_sequences.iteritems()):
      for test_messages in test_messages_sequence:
        request = test_messages.request()

        with self._control.fail(), self.assertRaises(face.RemoteError):
          self._invoker.blocking(group, method)(
              request, test_constants.LONG_TIMEOUT)

  def testFailedUnaryRequestStreamResponse(self):
    for (group, method), test_messages_sequence in (
        self._digest.unary_stream_messages_sequences.iteritems()):
      for test_messages in test_messages_sequence:
        request = test_messages.request()

        with self._control.fail(), self.assertRaises(face.RemoteError):
          response_iterator = self._invoker.blocking(group, method)(
              request, test_constants.LONG_TIMEOUT)
          list(response_iterator)

  def testFailedStreamRequestUnaryResponse(self):
    for (group, method), test_messages_sequence in (
        self._digest.stream_unary_messages_sequences.iteritems()):
      for test_messages in test_messages_sequence:
        requests = test_messages.requests()

        with self._control.fail(), self.assertRaises(face.RemoteError):
          self._invoker.blocking(group, method)(
              iter(requests), test_constants.LONG_TIMEOUT)

  def testFailedStreamRequestStreamResponse(self):
    for (group, method), test_messages_sequence in (
        self._digest.stream_stream_messages_sequences.iteritems()):
      for test_messages in test_messages_sequence:
        requests = test_messages.requests()

        with self._control.fail(), self.assertRaises(face.RemoteError):
          response_iterator = self._invoker.blocking(group, method)(
              iter(requests), test_constants.LONG_TIMEOUT)
          list(response_iterator)
