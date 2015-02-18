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

var _ = require('underscore');

var ProtoBuf = require('protobufjs');

var client = require('./src/client.js');

var server = require('./src/server.js');

var grpc = require('bindings')('grpc');

/**
 * Load a gRPC object from an existing ProtoBuf.Reflect object.
 * @param {ProtoBuf.Reflect.Namespace} value The ProtoBuf object to load.
 * @return {Object<string, *>} The resulting gRPC object
 */
function loadObject(value) {
  var result = {};
  if (value.className === 'Namespace') {
    _.each(value.children, function(child) {
      result[child.name] = loadObject(child);
    });
    return result;
  } else if (value.className === 'Service') {
    return client.makeClientConstructor(value);
  } else if (value.className === 'Message' || value.className === 'Enum') {
    return value.build();
  } else {
    return value;
  }
}

/**
 * Load a gRPC object from a .proto file.
 * @param {string} filename The file to load
 * @return {Object<string, *>} The resulting gRPC object
 */
function load(filename) {
  var builder = ProtoBuf.loadProtoFile(filename);

  return loadObject(builder.ns);
}

/**
 * Get a function that a client can use to update metadata with authentication
 * information from a Google Auth credential object.
 * @param {Object} credential The credential object to use
 * @return {function(Object, callback)} Metadata updater function
 */
function getGoogleAuthDelegate(credential) {
  /**
   * Update a metadata object with authentication information.
   * @param {Object} metadata Metadata object
   * @param {function(Error, Object)} callback
   */
  return function updateMetadata(metadata, callback) {
    metadata = _.clone(metadata);
    if (metadata.Authorization) {
      metadata.Authorization = _.clone(metadata.Authorization);
    } else {
      metadata.Authorization = [];
    }
    credential.getAccessToken(function(err, token) {
      if (err) {
        callback(err);
        return;
      }
      metadata.Authorization.push('Bearer ' + token);
      callback(null, metadata);
    });
  };
}

/**
 * See docs for loadObject
 */
exports.loadObject = loadObject;

/**
 * See docs for load
 */
exports.load = load;

/**
 * See docs for server.makeServerConstructor
 */
exports.buildServer = server.makeServerConstructor;

/**
 * Status name to code number mapping
 */
exports.status = grpc.status;
/**
 * Call error name to code number mapping
 */
exports.callError = grpc.callError;

/**
 * Credentials factories
 */
exports.Credentials = grpc.Credentials;

/**
 * ServerCredentials factories
 */
exports.ServerCredentials = grpc.ServerCredentials;

exports.getGoogleAuthDelegate = getGoogleAuthDelegate;
