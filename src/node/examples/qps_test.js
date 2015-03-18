/*
 *
 * Copyright 2015, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

'use strict';

var async = require('async');
var parseArgs = require('minimist');

var grpc = require('..');
var testProto = grpc.load(__dirname + '/../interop/test.proto').grpc.testing;
var interop_server = require('../interop/interop_server.js');

function runTest(concurrent, seconds, callback) {
  var testServer = interop_server.getServer(0, false);
  testServer.server.listen();
  var client = new testProto.TestService('localhost:' + testServer.port);

  var warmup_num = 100;

  async.waterfall([
    function warmUp(callback) {
      var pending = warmup_num;
      function startCall() {
        client.emptyCall({}, function(err, resp) {
          pending--;
          if (pending === 0) {
            callback(null);
          }
        });
      }
      for (var i = 0; i < warmup_num; i++) {
        startCall();
      }
    }, function run(callback) {
      var running = 0;
      var count = 0;
      var start = process.hrtime();
      function responseCallback(err, resp) {
        if (process.hrtime(start)[0] < seconds) {
          count += 1;
          client.emptyCall({}, responseCallback);
        } else {
          running -= 1;
          if (running <= 0) {
            callback(null, count);
          }
        }
      }
      for (var i = 0; i < concurrent; i++) {
        running += 1;
        client.emptyCall({}, responseCallback);
      }
    }], function(err, count) {
      testServer.server.shutdown();
      callback(err, count);
    });
}

if (require.main === module) {
  var argv = parseArgs(process.argv.slice(2), {
    default: {'concurrent': 100,
              'time': 10}
  });
  runTest(argv.concurrent, argv.time, function(err, count) {
    if (err) {
      throw err;
    }
    console.log('Concurrent calls:', argv.concurrent);
    console.log('Time:', argv.time, 'seconds');
    console.log('QPS:', (count/argv.time));
  });
}
