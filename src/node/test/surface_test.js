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

var assert = require('assert');

var surface_client = require('../src/client.js');

var ProtoBuf = require('protobufjs');

var grpc = require('..');

var math_proto = ProtoBuf.loadProtoFile(__dirname + '/../examples/math.proto');

var mathService = math_proto.lookup('math.Math');

var capitalize = require('underscore.string/capitalize');

describe('Surface server constructor', function() {
  it('Should fail with conflicting method names', function() {
    assert.throws(function() {
      grpc.buildServer([mathService, mathService]);
    });
  });
  it('Should succeed with a single service', function() {
    assert.doesNotThrow(function() {
      grpc.buildServer([mathService]);
    });
  });
  it('Should fail with missing handlers', function() {
    var Server = grpc.buildServer([mathService]);
    assert.throws(function() {
      new Server({
        'math.Math': {
          'div': function() {},
          'divMany': function() {},
          'fib': function() {}
        }
      });
    }, /math.Math.Sum/);
  });
  it('Should fail with no handlers for the service', function() {
    var Server = grpc.buildServer([mathService]);
    assert.throws(function() {
      new Server({});
    }, /math.Math/);
  });
});
describe('Generic client and server', function() {
  function toString(val) {
    return val.toString();
  }
  function toBuffer(str) {
    return new Buffer(str);
  }
  var string_service_attrs = {
    'capitalize' : {
      path: '/string/capitalize',
      requestStream: false,
      responseStream: false,
      requestSerialize: toBuffer,
      requestDeserialize: toString,
      responseSerialize: toBuffer,
      responseDeserialize: toString
    }
  };
  describe('String client and server', function() {
    var client;
    var server;
    before(function() {
      var Server = grpc.makeGenericServerConstructor({
        string: string_service_attrs
      });
      server = new Server({
        string: {
          capitalize: function(call, callback) {
            callback(null, capitalize(call.request));
          }
        }
      });
      var port = server.bind('localhost:0');
      server.listen();
      var Client = grpc.makeGenericClientConstructor(string_service_attrs);
      client = new Client('localhost:' + port);
    });
    after(function() {
      server.shutdown();
    });
    it('Should respond with a capitalized string', function(done) {
      client.capitalize('abc', function(err, response) {
        assert.ifError(err);
        assert.strictEqual(response, 'Abc');
        done();
      });
    });
  });
});
describe('Trailing metadata', function() {
  var client;
  var server;
  before(function() {
    var test_proto = ProtoBuf.loadProtoFile(__dirname + '/test_service.proto');
    var test_service = test_proto.lookup('TestService');
    var Server = grpc.buildServer([test_service]);
    server = new Server({
      TestService: {
        unary: function(call, cb) {
          var req = call.request;
          if (req.error) {
            cb(new Error('Requested error'), null, {metadata: ['yes']});
          } else {
            cb(null, {count: 1}, {metadata: ['yes']});
          }
        },
        clientStream: function(stream, cb){
          var count = 0;
          var errored;
          stream.on('data', function(data) {
            if (data.error) {
              errored = true;
              cb(new Error('Requested error'), null, {metadata: ['yes']});
            } else {
              count += 1;
            }
          });
          stream.on('end', function() {
            if (!errored) {
              cb(null, {count: count}, {metadata: ['yes']});
            }
          });
        },
        serverStream: function(stream) {
          var req = stream.request;
          if (req.error) {
            var err = new Error('Requested error');
            err.metadata = {metadata: ['yes']};
            stream.emit('error', err);
          } else {
            for (var i = 0; i < 5; i++) {
              stream.write({count: i});
            }
            stream.end({metadata: ['yes']});
          }
        },
        bidiStream: function(stream) {
          var count = 0;
          stream.on('data', function(data) {
            if (data.error) {
              var err = new Error('Requested error');
              err.metadata = {
                metadata: ['yes'],
                count: ['' + count]
              };
              stream.emit('error', err);
            } else {
              stream.write({count: count});
              count += 1;
            }
          });
          stream.on('end', function() {
            stream.end({metadata: ['yes']});
          });
        }
      }
    });
    var port = server.bind('localhost:0');
    var Client = surface_client.makeProtobufClientConstructor(test_service);
    client = new Client('localhost:' + port);
    server.listen();
  });
  after(function() {
    server.shutdown();
  });
  it('when a unary call succeeds', function(done) {
    var call = client.unary({error: false}, function(err, data) {
      assert.ifError(err);
    });
    call.on('status', function(status) {
      assert.deepEqual(status.metadata.metadata, ['yes']);
      done();
    });
  });
  it('when a unary call fails', function(done) {
    var call = client.unary({error: true}, function(err, data) {
      assert(err);
    });
    call.on('status', function(status) {
      assert.deepEqual(status.metadata.metadata, ['yes']);
      done();
    });
  });
  it('when a client stream call succeeds', function(done) {
    var call = client.clientStream(function(err, data) {
      assert.ifError(err);
    });
    call.write({error: false});
    call.write({error: false});
    call.end();
    call.on('status', function(status) {
      assert.deepEqual(status.metadata.metadata, ['yes']);
      done();
    });
  });
  it('when a client stream call fails', function(done) {
    var call = client.clientStream(function(err, data) {
      assert(err);
    });
    call.write({error: false});
    call.write({error: true});
    call.end();
    call.on('status', function(status) {
      assert.deepEqual(status.metadata.metadata, ['yes']);
      done();
    });
  });
  it('when a server stream call succeeds', function(done) {
    var call = client.serverStream({error: false});
    call.on('data', function(){});
    call.on('status', function(status) {
      assert.strictEqual(status.code, grpc.status.OK);
      assert.deepEqual(status.metadata.metadata, ['yes']);
      done();
    });
  });
  it('when a server stream call fails', function(done) {
    var call = client.serverStream({error: true});
    call.on('data', function(){});
    call.on('status', function(status) {
      assert.notStrictEqual(status.code, grpc.status.OK);
      assert.deepEqual(status.metadata.metadata, ['yes']);
      done();
    });
  });
  it('when a bidi stream succeeds', function(done) {
    var call = client.bidiStream();
    call.write({error: false});
    call.write({error: false});
    call.end();
    call.on('data', function(){});
    call.on('status', function(status) {
      assert.strictEqual(status.code, grpc.status.OK);
      assert.deepEqual(status.metadata.metadata, ['yes']);
      done();
    });
  });
  it('when a bidi stream fails', function(done) {
    var call = client.bidiStream();
    call.write({error: false});
    call.write({error: true});
    call.end();
    call.on('data', function(){});
    call.on('status', function(status) {
      assert.notStrictEqual(status.code, grpc.status.OK);
      assert.deepEqual(status.metadata.metadata, ['yes']);
      done();
    });
  });
});
describe('Cancelling surface client', function() {
  var client;
  var server;
  before(function() {
    var Server = grpc.buildServer([mathService]);
    server = new Server({
      'math.Math': {
        'div': function(stream) {},
        'divMany': function(stream) {},
        'fib': function(stream) {},
        'sum': function(stream) {}
      }
    });
    var port = server.bind('localhost:0');
    var Client = surface_client.makeProtobufClientConstructor(mathService);
    client = new Client('localhost:' + port);
  });
  after(function() {
    server.shutdown();
  });
  it('Should correctly cancel a unary call', function(done) {
    var call = client.div({'divisor': 0, 'dividend': 0}, function(err, resp) {
      assert.strictEqual(err.code, surface_client.status.CANCELLED);
      done();
    });
    call.cancel();
  });
  it('Should correctly cancel a client stream call', function(done) {
    var call = client.sum(function(err, resp) {
      assert.strictEqual(err.code, surface_client.status.CANCELLED);
      done();
    });
    call.cancel();
  });
  it('Should correctly cancel a server stream call', function(done) {
    var call = client.fib({'limit': 5});
    call.on('status', function(status) {
      assert.strictEqual(status.code, surface_client.status.CANCELLED);
      done();
    });
    call.cancel();
  });
  it('Should correctly cancel a bidi stream call', function(done) {
    var call = client.divMany();
    call.on('status', function(status) {
      assert.strictEqual(status.code, surface_client.status.CANCELLED);
      done();
    });
    call.cancel();
  });
});
