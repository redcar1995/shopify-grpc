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

"""HTTP2 Test Server"""

import argparse
import logging
import twisted
import twisted.internet
import twisted.internet.endpoints
import twisted.internet.reactor

import http2_base_server
import test_goaway
import test_max_streams
import test_ping
import test_rst_after_data
import test_rst_after_header
import test_rst_during_data

_TEST_CASE_MAPPING = {
  'rst_after_header': test_rst_after_header.TestcaseRstStreamAfterHeader,
  'rst_after_data': test_rst_after_data.TestcaseRstStreamAfterData,
  'rst_during_data': test_rst_during_data.TestcaseRstStreamDuringData,
  'goaway': test_goaway.TestcaseGoaway,
  'ping': test_ping.TestcasePing,
  'max_streams': test_max_streams.TestcaseSettingsMaxStreams,
}

class H2Factory(twisted.internet.protocol.Factory):
  def __init__(self, testcase):
    logging.info('Creating H2Factory for new connection.')
    self._num_streams = 0
    self._testcase = testcase

  def buildProtocol(self, addr):
    self._num_streams += 1
    logging.info('New Connection: %d' % self._num_streams)
    if not _TEST_CASE_MAPPING.has_key(self._testcase):
      logging.error('Unknown test case: %s' % self._testcase)
      assert(0)
    else:
      t = _TEST_CASE_MAPPING[self._testcase]

    if self._testcase == 'goaway':
      return t(self._num_streams).get_base_server()
    else:
      return t().get_base_server()

if __name__ == '__main__':
  logging.basicConfig(
    format='%(levelname) -10s %(asctime)s %(module)s:%(lineno)s | %(message)s',
    level=logging.INFO)
  parser = argparse.ArgumentParser()
  parser.add_argument('--test_case', choices=sorted(_TEST_CASE_MAPPING.keys()),
    help='test case to run', required=True)
  parser.add_argument('--port', type=int, default=8080,
    help='port to run the server (default: 8080)')
  args = parser.parse_args()
  logging.info('Running test case %s on port %d' % (args.test_case, args.port))
  endpoint = twisted.internet.endpoints.TCP4ServerEndpoint(
    twisted.internet.reactor, args.port, backlog=128)
  endpoint.listen(H2Factory(args.test_case))
  twisted.internet.reactor.run()
