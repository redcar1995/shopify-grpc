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

var _ = require('lodash');

var grpc = require('bindings')('grpc.node');

var common = require('./common');

var stream = require('stream');

var Readable = stream.Readable;
var Writable = stream.Writable;
var Duplex = stream.Duplex;
var util = require('util');

var EventEmitter = require('events').EventEmitter;

/**
 * Handle an error on a call by sending it as a status
 * @param {grpc.Call} call The call to send the error on
 * @param {Object} error The error object
 */
function handleError(call, error) {
  var status = {
    code: grpc.status.UNKNOWN,
    details: 'Unknown Error',
    metadata: {}
  };
  if (error.hasOwnProperty('message')) {
    status.details = error.message;
  }
  if (error.hasOwnProperty('code')) {
    status.code = error.code;
    if (error.hasOwnProperty('details')) {
      status.details = error.details;
    }
  }
  if (error.hasOwnProperty('metadata')) {
    status.metadata = error.metadata;
  }
  var error_batch = {};
  if (!call.metadataSent) {
    error_batch[grpc.opType.SEND_INITIAL_METADATA] = {};
  }
  error_batch[grpc.opType.SEND_STATUS_FROM_SERVER] = status;
  call.startBatch(error_batch, function(){});
}

/**
 * Wait for the client to close, then emit a cancelled event if the client
 * cancelled.
 * @param {grpc.Call} call The call object to wait on
 * @param {EventEmitter} emitter The event emitter to emit the cancelled event
 *     on
 */
function waitForCancel(call, emitter) {
  var cancel_batch = {};
  cancel_batch[grpc.opType.RECV_CLOSE_ON_SERVER] = true;
  call.startBatch(cancel_batch, function(err, result) {
    if (err) {
      emitter.emit('error', err);
    }
    if (result.cancelled) {
      emitter.cancelled = true;
      emitter.emit('cancelled');
    }
  });
}

/**
 * Send a response to a unary or client streaming call.
 * @param {grpc.Call} call The call to respond on
 * @param {*} value The value to respond with
 * @param {function(*):Buffer=} serialize Serialization function for the
 *     response
 * @param {Object=} metadata Optional trailing metadata to send with status
 */
function sendUnaryResponse(call, value, serialize, metadata) {
  var end_batch = {};
  var status = {
    code: grpc.status.OK,
    details: 'OK',
    metadata: {}
  };
  if (metadata) {
    status.metadata = metadata;
  }
  if (!call.metadataSent) {
    end_batch[grpc.opType.SEND_INITIAL_METADATA] = {};
    call.metadataSent = true;
  }
  end_batch[grpc.opType.SEND_MESSAGE] = serialize(value);
  end_batch[grpc.opType.SEND_STATUS_FROM_SERVER] = status;
  call.startBatch(end_batch, function (){});
}

/**
 * Initialize a writable stream. This is used for both the writable and duplex
 * stream constructors.
 * @param {Writable} stream The stream to set up
 * @param {function(*):Buffer=} Serialization function for responses
 */
function setUpWritable(stream, serialize) {
  stream.finished = false;
  stream.status = {
    code : grpc.status.OK,
    details : 'OK',
    metadata : {}
  };
  stream.serialize = common.wrapIgnoreNull(serialize);
  function sendStatus() {
    var batch = {};
    if (!stream.call.metadataSent) {
      stream.call.metadataSent = true;
      batch[grpc.opType.SEND_INITIAL_METADATA] = {};
    }
    batch[grpc.opType.SEND_STATUS_FROM_SERVER] = stream.status;
    stream.call.startBatch(batch, function(){});
  }
  stream.on('finish', sendStatus);
  /**
   * Set the pending status to a given error status. If the error does not have
   * code or details properties, the code will be set to grpc.status.UNKNOWN
   * and the details will be set to 'Unknown Error'.
   * @param {Error} err The error object
   */
  function setStatus(err) {
    var code = grpc.status.UNKNOWN;
    var details = 'Unknown Error';
    var metadata = {};
    if (err.hasOwnProperty('message')) {
      details = err.message;
    }
    if (err.hasOwnProperty('code')) {
      code = err.code;
      if (err.hasOwnProperty('details')) {
        details = err.details;
      }
    }
    if (err.hasOwnProperty('metadata')) {
      metadata = err.metadata;
    }
    stream.status = {code: code, details: details, metadata: metadata};
  }
  /**
   * Terminate the call. This includes indicating that reads are done, draining
   * all pending writes, and sending the given error as a status
   * @param {Error} err The error object
   * @this GrpcServerStream
   */
  function terminateCall(err) {
    // Drain readable data
    setStatus(err);
    stream.end();
  }
  stream.on('error', terminateCall);
  /**
   * Override of Writable#end method that allows for sending metadata with a
   * success status.
   * @param {Object=} metadata Metadata to send with the status
   */
  stream.end = function(metadata) {
    if (metadata) {
      stream.status.metadata = metadata;
    }
    Writable.prototype.end.call(this);
  };
}

/**
 * Initialize a readable stream. This is used for both the readable and duplex
 * stream constructors.
 * @param {Readable} stream The stream to initialize
 * @param {function(Buffer):*=} deserialize Deserialization function for
 *     incoming data.
 */
function setUpReadable(stream, deserialize) {
  stream.deserialize = common.wrapIgnoreNull(deserialize);
  stream.finished = false;
  stream.reading = false;

  stream.terminate = function() {
    stream.finished = true;
    stream.on('data', function() {});
  };

  stream.on('cancelled', function() {
    stream.terminate();
  });
}

util.inherits(ServerWritableStream, Writable);

/**
 * A stream that the server can write to. Used for calls that are streaming from
 * the server side.
 * @constructor
 * @param {grpc.Call} call The call object to send data with
 * @param {function(*):Buffer=} serialize Serialization function for writes
 */
function ServerWritableStream(call, serialize) {
  Writable.call(this, {objectMode: true});
  this.call = call;

  this.finished = false;
  setUpWritable(this, serialize);
}

/**
 * Start writing a chunk of data. This is an implementation of a method required
 * for implementing stream.Writable.
 * @param {Buffer} chunk The chunk of data to write
 * @param {string} encoding Ignored
 * @param {function(Error=)} callback Callback to indicate that the write is
 *     complete
 */
function _write(chunk, encoding, callback) {
  /* jshint validthis: true */
  var batch = {};
  if (!this.call.metadataSent) {
    batch[grpc.opType.SEND_INITIAL_METADATA] = {};
    this.call.metadataSent = true;
  }
  batch[grpc.opType.SEND_MESSAGE] = this.serialize(chunk);
  this.call.startBatch(batch, function(err, value) {
    if (err) {
      this.emit('error', err);
      return;
    }
    callback();
  });
}

ServerWritableStream.prototype._write = _write;

function sendMetadata(responseMetadata) {
  /* jshint validthis: true */
  if (!this.call.metadataSent) {
    this.call.metadataSent = true;
    var batch = [];
    batch[grpc.opType.SEND_INITIAL_METADATA] = responseMetadata;
    this.call.startBatch(batch, function(err) {
      if (err) {
        this.emit('error', err);
        return;
      }
    });
  }
}

ServerWritableStream.prototype.sendMetadata = sendMetadata;

util.inherits(ServerReadableStream, Readable);

/**
 * A stream that the server can read from. Used for calls that are streaming
 * from the client side.
 * @constructor
 * @param {grpc.Call} call The call object to read data with
 * @param {function(Buffer):*=} deserialize Deserialization function for reads
 */
function ServerReadableStream(call, deserialize) {
  Readable.call(this, {objectMode: true});
  this.call = call;
  setUpReadable(this, deserialize);
}

/**
 * Start reading from the gRPC data source. This is an implementation of a
 * method required for implementing stream.Readable
 * @param {number} size Ignored
 */
function _read(size) {
  /* jshint validthis: true */
  var self = this;
  /**
   * Callback to be called when a READ event is received. Pushes the data onto
   * the read queue and starts reading again if applicable
   * @param {grpc.Event} event READ event object
   */
  function readCallback(err, event) {
    if (err) {
      self.terminate();
      return;
    }
    if (self.finished) {
      self.push(null);
      return;
    }
    var data = event.read;
    var deserialized;
    try {
      deserialized = self.deserialize(data);
    } catch (e) {
      e.code = grpc.status.INVALID_ARGUMENT;
      self.emit('error', e);
      return;
    }
    if (self.push(deserialized) && data !== null) {
      var read_batch = {};
      read_batch[grpc.opType.RECV_MESSAGE] = true;
      self.call.startBatch(read_batch, readCallback);
    } else {
      self.reading = false;
    }
  }
  if (self.finished) {
    self.push(null);
  } else {
    if (!self.reading) {
      self.reading = true;
      var batch = {};
      batch[grpc.opType.RECV_MESSAGE] = true;
      self.call.startBatch(batch, readCallback);
    }
  }
}

ServerReadableStream.prototype._read = _read;

util.inherits(ServerDuplexStream, Duplex);

/**
 * A stream that the server can read from or write to. Used for calls with
 * duplex streaming.
 * @constructor
 * @param {grpc.Call} call Call object to proxy
 * @param {function(*):Buffer=} serialize Serialization function for requests
 * @param {function(Buffer):*=} deserialize Deserialization function for
 *     responses
 */
function ServerDuplexStream(call, serialize, deserialize) {
  Duplex.call(this, {objectMode: true});
  this.call = call;
  setUpWritable(this, serialize);
  setUpReadable(this, deserialize);
}

ServerDuplexStream.prototype._read = _read;
ServerDuplexStream.prototype._write = _write;
ServerDuplexStream.prototype.sendMetadata = sendMetadata;

/**
 * Fully handle a unary call
 * @param {grpc.Call} call The call to handle
 * @param {Object} handler Request handler object for the method that was called
 * @param {Object} metadata Metadata from the client
 */
function handleUnary(call, handler, metadata) {
  var emitter = new EventEmitter();
  emitter.sendMetadata = function(responseMetadata) {
    if (!call.metadataSent) {
      call.metadataSent = true;
      var batch = {};
      batch[grpc.opType.SEND_INITIAL_METADATA] = responseMetadata;
      call.startBatch(batch, function() {});
    }
  };
  emitter.on('error', function(error) {
    handleError(call, error);
  });
  emitter.metadata = metadata;
  waitForCancel(call, emitter);
  var batch = {};
  batch[grpc.opType.RECV_MESSAGE] = true;
  call.startBatch(batch, function(err, result) {
    if (err) {
      handleError(call, err);
      return;
    }
    try {
      emitter.request = handler.deserialize(result.read);
    } catch (e) {
      e.code = grpc.status.INVALID_ARGUMENT;
      handleError(call, e);
      return;
    }
    if (emitter.cancelled) {
      return;
    }
    handler.func(emitter, function sendUnaryData(err, value, trailer) {
      if (err) {
        if (trailer) {
          err.metadata = trailer;
        }
        handleError(call, err);
      } else {
        sendUnaryResponse(call, value, handler.serialize, trailer);
      }
    });
  });
}

/**
 * Fully handle a server streaming call
 * @param {grpc.Call} call The call to handle
 * @param {Object} handler Request handler object for the method that was called
 * @param {Object} metadata Metadata from the client
 */
function handleServerStreaming(call, handler, metadata) {
  var stream = new ServerWritableStream(call, handler.serialize);
  waitForCancel(call, stream);
  stream.metadata = metadata;
  var batch = {};
  batch[grpc.opType.RECV_MESSAGE] = true;
  call.startBatch(batch, function(err, result) {
    if (err) {
      stream.emit('error', err);
      return;
    }
    try {
      stream.request = handler.deserialize(result.read);
    } catch (e) {
      e.code = grpc.status.INVALID_ARGUMENT;
      stream.emit('error', e);
      return;
    }
    handler.func(stream);
  });
}

/**
 * Fully handle a client streaming call
 * @param {grpc.Call} call The call to handle
 * @param {Object} handler Request handler object for the method that was called
 * @param {Object} metadata Metadata from the client
 */
function handleClientStreaming(call, handler, metadata) {
  var stream = new ServerReadableStream(call, handler.deserialize);
  stream.sendMetadata = function(responseMetadata) {
    if (!call.metadataSent) {
      call.metadataSent = true;
      var batch = {};
      batch[grpc.opType.SEND_INITIAL_METADATA] = responseMetadata;
      call.startBatch(batch, function() {});
    }
  };
  stream.on('error', function(error) {
    handleError(call, error);
  });
  waitForCancel(call, stream);
  stream.metadata = metadata;
  handler.func(stream, function(err, value, trailer) {
    stream.terminate();
    if (err) {
      if (trailer) {
        err.metadata = trailer;
      }
      handleError(call, err);
    } else {
      sendUnaryResponse(call, value, handler.serialize, trailer);
    }
  });
}

/**
 * Fully handle a bidirectional streaming call
 * @param {grpc.Call} call The call to handle
 * @param {Object} handler Request handler object for the method that was called
 * @param {Object} metadata Metadata from the client
 */
function handleBidiStreaming(call, handler, metadata) {
  var stream = new ServerDuplexStream(call, handler.serialize,
                                      handler.deserialize);
  waitForCancel(call, stream);
  stream.metadata = metadata;
  handler.func(stream);
}

var streamHandlers = {
  unary: handleUnary,
  server_stream: handleServerStreaming,
  client_stream: handleClientStreaming,
  bidi: handleBidiStreaming
};

/**
 * Constructs a server object that stores request handlers and delegates
 * incoming requests to those handlers
 * @constructor
 * @param {Object=} options Options that should be passed to the internal server
 *     implementation
 */
function Server(options) {
  this.handlers = {};
  var handlers = this.handlers;
  var server = new grpc.Server(options);
  this._server = server;
  this.started = false;
  /**
   * Start the server and begin handling requests
   * @this Server
   */
  this.start = function() {
    if (this.started) {
      throw new Error('Server is already running');
    }
    this.started = true;
    console.log('Server starting');
    _.each(handlers, function(handler, handler_name) {
      console.log('Serving', handler_name);
    });
    server.start();
    /**
     * Handles the SERVER_RPC_NEW event. If there is a handler associated with
     * the requested method, use that handler to respond to the request. Then
     * wait for the next request
     * @param {grpc.Event} event The event to handle with tag SERVER_RPC_NEW
     */
    function handleNewCall(err, event) {
      if (err) {
        return;
      }
      var details = event.new_call;
      var call = details.call;
      var method = details.method;
      var metadata = details.metadata;
      if (method === null) {
        return;
      }
      server.requestCall(handleNewCall);
      var handler;
      if (handlers.hasOwnProperty(method)) {
        handler = handlers[method];
      } else {
        var batch = {};
        batch[grpc.opType.SEND_INITIAL_METADATA] = {};
        batch[grpc.opType.SEND_STATUS_FROM_SERVER] = {
          code: grpc.status.UNIMPLEMENTED,
          details: 'This method is not available on this server.',
          metadata: {}
        };
        batch[grpc.opType.RECV_CLOSE_ON_SERVER] = true;
        call.startBatch(batch, function() {});
        return;
      }
      streamHandlers[handler.type](call, handler, metadata);
    }
    server.requestCall(handleNewCall);
  };
  /** Shuts down the server.
   */
  this.shutdown = function() {
    server.shutdown();
  };
}

/**
 * Registers a handler to handle the named method. Fails if there already is
 * a handler for the given method. Returns true on success
 * @param {string} name The name of the method that the provided function should
 *     handle/respond to.
 * @param {function} handler Function that takes a stream of request values and
 *     returns a stream of response values
 * @param {function(*):Buffer} serialize Serialization function for responses
 * @param {function(Buffer):*} deserialize Deserialization function for requests
 * @param {string} type The streaming type of method that this handles
 * @return {boolean} True if the handler was set. False if a handler was already
 *     set for that name.
 */
Server.prototype.register = function(name, handler, serialize, deserialize,
                                     type) {
  if (this.handlers.hasOwnProperty(name)) {
    return false;
  }
  this.handlers[name] = {
    func: handler,
    serialize: serialize,
    deserialize: deserialize,
    type: type
  };
  return true;
};

Server.prototype.addService = function(service, implementation) {
  if (this.started) {
    throw new Error('Can\'t add a service to a started server.');
  }
  var self = this;
  _.each(service, function(attrs, name) {
    var method_type;
    if (attrs.requestStream) {
      if (attrs.responseStream) {
        method_type = 'bidi';
      } else {
        method_type = 'client_stream';
      }
    } else {
      if (attrs.responseStream) {
        method_type = 'server_stream';
      } else {
        method_type = 'unary';
      }
    }
    if (implementation[name] === undefined) {
      throw new Error('Method handler for ' + attrs.path +
          ' not provided.');
    }
    var serialize = attrs.responseSerialize;
    var deserialize = attrs.requestDeserialize;
    var register_success = self.register(attrs.path,
                                         _.bind(implementation[name],
                                                implementation),
                                         serialize, deserialize, method_type);
    if (!register_success) {
      throw new Error('Method handler for ' + attrs.path +
          ' already provided.');
    }
  });
};

Server.prototype.addProtoService = function(service, implementation) {
  this.addService(common.getProtobufServiceAttrs(service), implementation);
};

/**
 * Binds the server to the given port, with SSL enabled if creds is given
 * @param {string} port The port that the server should bind on, in the format
 *     "address:port"
 * @param {boolean=} creds Server credential object to be used for SSL. Pass
 *     nothing for an insecure port
 */
Server.prototype.bind = function(port, creds) {
  if (this.started) {
    throw new Error('Can\'t bind an already running server to an address');
  }
  if (creds) {
    return this._server.addSecureHttp2Port(port, creds);
  } else {
    return this._server.addHttp2Port(port);
  }
};

/**
 * See documentation for Server
 */
exports.Server = Server;
