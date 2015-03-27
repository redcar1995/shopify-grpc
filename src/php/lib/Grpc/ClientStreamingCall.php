<?php
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
namespace Grpc;

/**
 * Represents an active call that sends a stream of messages and then gets a
 * single response.
 */
class ClientStreamingCall extends AbstractCall {
  /**
   * Start the call.
   * @param Traversable $arg_iter The iterator of arguments to send
   * @param array $metadata Metadata to send with the call, if applicable
   */
  public function start($arg_iter, $metadata = array()) {
    $event = $this->call->start_batch([
        OP_SEND_INITIAL_METADATA => $metadata,
        OP_RECV_INITIAL_METADATA => true]);
    $this->metadata = $event->metadata;
    foreach($arg_iter as $arg) {
      $this->call->start_batch([OP_SEND_MESSAGE => $arg->serialize()]);
    }
    $this->call->start_batch([OP_SEND_CLOSE_FROM_CLIENT => true]);
  }

  /**
   * Wait for the server to respond with data and a status
   * @return [response data, status]
   */
  public function wait() {
    $event = $this->call->start_batch([
        OP_RECV_MESSAGE => true,
        OP_RECV_STATUS_ON_CLIENT => true]);
    return array($this->deserializeResponse($event->message), $event->status);
  }
}