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

"""The RPC-invocation-side bridge between RPC Framework and GRPC-on-the-wire."""

import enum
import logging
import threading
import time

from _adapter import _common
from _adapter import _low
from _framework.base.packets import interfaces as ticket_interfaces
from _framework.base.packets import null
from _framework.base.packets import packets as tickets

_INVOCATION_EVENT_KINDS = (
    _low.Event.Kind.METADATA_ACCEPTED,
    _low.Event.Kind.FINISH
)


@enum.unique
class _LowWrite(enum.Enum):
  """The possible categories of low-level write state."""

  OPEN = 'OPEN'
  ACTIVE = 'ACTIVE'
  CLOSED = 'CLOSED'


class _RPCState(object):
  """The full state of any tracked RPC.

  Attributes:
    call: The _low.Call object for the RPC.
    outstanding: The set of Event.Kind values describing expected future events
      for the RPC.
    active: A boolean indicating whether or not the RPC is active.
    common: An _common.RPCState describing additional state for the RPC.
  """

  def __init__(self, call, outstanding, active, common):
    self.call = call
    self.outstanding = outstanding
    self.active = active
    self.common = common


def _write(operation_id, call, outstanding, write_state, serialized_payload):
  if write_state.low is _LowWrite.OPEN:
    call.write(serialized_payload, operation_id)
    outstanding.add(_low.Event.Kind.WRITE_ACCEPTED)
    write_state.low = _LowWrite.ACTIVE
  elif write_state.low is _LowWrite.ACTIVE:
    write_state.pending.append(serialized_payload)
  else:
    raise ValueError('Write attempted after writes completed!')


class RearLink(ticket_interfaces.RearLink):
  """An invocation-side bridge between RPC Framework and the C-ish _low code."""

  def __init__(
      self, host, port, pool, request_serializers, response_deserializers):
    """Constructor.

    Args:
      host: The host to which to connect for RPC service.
      port: The port to which to connect for RPC service.
      pool: A thread pool.
      request_serializers: A dict from RPC method names to request object
        serializer behaviors.
      response_deserializers: A dict from RPC method names to response object
        deserializer behaviors.
    """
    self._condition = threading.Condition()
    self._host = host
    self._port = port
    self._pool = pool
    self._request_serializers = request_serializers
    self._response_deserializers = response_deserializers

    self._fore_link = null.NULL_FORE_LINK
    self._completion_queue = None
    self._channel = None
    self._rpc_states = {}
    self._spinning = False

  def _on_write_event(self, operation_id, event, rpc_state):
    if event.write_accepted:
      if rpc_state.common.write.pending:
        rpc_state.call.write(
            rpc_state.common.write.pending.pop(0), operation_id)
        rpc_state.outstanding.add(_low.Event.Kind.WRITE_ACCEPTED)
      elif rpc_state.common.write.high is _common.HighWrite.CLOSED:
        rpc_state.call.complete(operation_id)
        rpc_state.outstanding.add(_low.Event.Kind.COMPLETE_ACCEPTED)
        rpc_state.common.write.low = _LowWrite.CLOSED
      else:
        rpc_state.common.write.low = _LowWrite.OPEN
    else:
      logging.error('RPC write not accepted! Event: %s', (event,))
      rpc_state.active = False
      ticket = tickets.BackToFrontPacket(
          operation_id, rpc_state.common.sequence_number,
          tickets.Kind.TRANSMISSION_FAILURE, None)
      rpc_state.common.sequence_number += 1
      self._fore_link.accept_back_to_front_ticket(ticket)

  def _on_read_event(self, operation_id, event, rpc_state):
    if event.bytes is not None:
      rpc_state.call.read(operation_id)
      rpc_state.outstanding.add(_low.Event.Kind.READ_ACCEPTED)

      ticket = tickets.BackToFrontPacket(
          operation_id, rpc_state.common.sequence_number,
          tickets.Kind.CONTINUATION, rpc_state.common.deserializer(event.bytes))
      rpc_state.common.sequence_number += 1
      self._fore_link.accept_back_to_front_ticket(ticket)

  def _on_complete_event(self, operation_id, event, rpc_state):
    if not event.complete_accepted:
      logging.error('RPC complete not accepted! Event: %s', (event,))
      rpc_state.active = False
      ticket = tickets.BackToFrontPacket(
          operation_id, rpc_state.common.sequence_number,
          tickets.Kind.TRANSMISSION_FAILURE, None)
      rpc_state.common.sequence_number += 1
      self._fore_link.accept_back_to_front_ticket(ticket)

  # TODO(nathaniel): Metadata support.
  def _on_metadata_event(self, operation_id, event, rpc_state):  # pylint: disable=unused-argument
    rpc_state.call.read(operation_id)
    rpc_state.outstanding.add(_low.Event.Kind.READ_ACCEPTED)

  def _on_finish_event(self, operation_id, event, rpc_state):
    """Handle termination of an RPC."""
    # TODO(nathaniel): Cover all statuses.
    if event.status.code is _low.Code.OK:
      category = tickets.Kind.COMPLETION
    elif event.status.code is _low.Code.CANCELLED:
      category = tickets.Kind.CANCELLATION
    elif event.status.code is _low.Code.EXPIRED:
      category = tickets.Kind.EXPIRATION
    else:
      category = tickets.Kind.TRANSMISSION_FAILURE
    ticket = tickets.BackToFrontPacket(
        operation_id, rpc_state.common.sequence_number, category,
        None)
    rpc_state.common.sequence_number += 1
    self._fore_link.accept_back_to_front_ticket(ticket)

  def _spin(self, completion_queue):
    while True:
      event = completion_queue.get(None)
      operation_id = event.tag

      with self._condition:
        rpc_state = self._rpc_states[operation_id]
        rpc_state.outstanding.remove(event.kind)
        if rpc_state.active and self._completion_queue is not None:
          if event.kind is _low.Event.Kind.WRITE_ACCEPTED:
            self._on_write_event(operation_id, event, rpc_state)
          elif event.kind is _low.Event.Kind.METADATA_ACCEPTED:
            self._on_metadata_event(operation_id, event, rpc_state)
          elif event.kind is _low.Event.Kind.READ_ACCEPTED:
            self._on_read_event(operation_id, event, rpc_state)
          elif event.kind is _low.Event.Kind.COMPLETE_ACCEPTED:
            self._on_complete_event(operation_id, event, rpc_state)
          elif event.kind is _low.Event.Kind.FINISH:
            self._on_finish_event(operation_id, event, rpc_state)
          else:
            logging.error('Illegal RPC event! %s', (event,))

        if not rpc_state.outstanding:
          self._rpc_states.pop(operation_id)
        if not self._rpc_states:
          self._spinning = False
          self._condition.notify_all()
          return

  def _invoke(self, operation_id, name, high_state, payload, timeout):
    """Invoke an RPC.

    Args:
      operation_id: Any object to be used as an operation ID for the RPC.
      name: The RPC method name.
      high_state: A _common.HighWrite value representing the "high write state"
        of the RPC.
      payload: A payload object for the RPC or None if no payload was given at
        invocation-time.
      timeout: A duration of time in seconds to allow for the RPC.
    """
    request_serializer = self._request_serializers[name]
    call = _low.Call(self._channel, name, self._host, time.time() + timeout)
    call.invoke(self._completion_queue, operation_id, operation_id)
    outstanding = set(_INVOCATION_EVENT_KINDS)

    if payload is None:
      if high_state is _common.HighWrite.CLOSED:
        call.complete(operation_id)
        low_state = _LowWrite.CLOSED
        outstanding.add(_low.Event.Kind.COMPLETE_ACCEPTED)
      else:
        low_state = _LowWrite.OPEN
    else:
      serialized_payload = request_serializer(payload)
      call.write(serialized_payload, operation_id)
      outstanding.add(_low.Event.Kind.WRITE_ACCEPTED)
      low_state = _LowWrite.ACTIVE

    write_state = _common.WriteState(low_state, high_state, [])
    common_state = _common.CommonRPCState(
        write_state, 0, self._response_deserializers[name], request_serializer)
    self._rpc_states[operation_id] = _RPCState(
        call, outstanding, True, common_state)

    if not self._spinning:
      self._pool.submit(self._spin, self._completion_queue)
      self._spinning = True

  def _commence(self, operation_id, name, payload, timeout):
    self._invoke(operation_id, name, _common.HighWrite.OPEN, payload, timeout)

  def _continue(self, operation_id, payload):
    rpc_state = self._rpc_states.get(operation_id, None)
    if rpc_state is None or not rpc_state.active:
      return

    _write(
        operation_id, rpc_state.call, rpc_state.outstanding,
        rpc_state.common.write, rpc_state.common.serializer(payload))

  def _complete(self, operation_id, payload):
    """Close writes associated with an ongoing RPC.

    Args:
      operation_id: Any object being use as an operation ID for the RPC.
      payload: A payload object for the RPC (and thus the last payload object
        for the RPC) or None if no payload was given along with the instruction
        to indicate the end of writes for the RPC.
    """
    rpc_state = self._rpc_states.get(operation_id, None)
    if rpc_state is None or not rpc_state.active:
      return

    write_state = rpc_state.common.write
    if payload is None:
      if write_state.low is _LowWrite.OPEN:
        rpc_state.call.complete(operation_id)
        rpc_state.outstanding.add(_low.Event.Kind.COMPLETE_ACCEPTED)
        write_state.low = _LowWrite.CLOSED
    else:
      _write(
          operation_id, rpc_state.call, rpc_state.outstanding, write_state,
          rpc_state.common.serializer(payload))
    write_state.high = _common.HighWrite.CLOSED

  def _entire(self, operation_id, name, payload, timeout):
    self._invoke(operation_id, name, _common.HighWrite.CLOSED, payload, timeout)

  def _cancel(self, operation_id):
    rpc_state = self._rpc_states.get(operation_id, None)
    if rpc_state is not None and rpc_state.active:
      rpc_state.call.cancel()
      rpc_state.active = False

  def join_fore_link(self, fore_link):
    """See ticket_interfaces.RearLink.join_fore_link for specification."""
    with self._condition:
      self._fore_link = null.NULL_FORE_LINK if fore_link is None else fore_link

  def start(self):
    """Starts this RearLink.

    This method must be called before attempting to exchange tickets with this
    object.
    """
    with self._condition:
      self._completion_queue = _low.CompletionQueue()
      self._channel = _low.Channel('%s:%d' % (self._host, self._port))

  def stop(self):
    """Stops this RearLink.

    This method must be called for proper termination of this object, and no
    attempts to exchange tickets with this object may be made after this method
    has been called.
    """
    with self._condition:
      self._completion_queue.stop()
      self._completion_queue = None

      while self._spinning:
        self._condition.wait()

  def accept_front_to_back_ticket(self, ticket):
    """See ticket_interfaces.RearLink.accept_front_to_back_ticket for spec."""
    with self._condition:
      if self._completion_queue is None:
        return

      if ticket.kind is tickets.Kind.COMMENCEMENT:
        self._commence(
            ticket.operation_id, ticket.name, ticket.payload, ticket.timeout)
      elif ticket.kind is tickets.Kind.CONTINUATION:
        self._continue(ticket.operation_id, ticket.payload)
      elif ticket.kind is tickets.Kind.COMPLETION:
        self._complete(ticket.operation_id, ticket.payload)
      elif ticket.kind is tickets.Kind.ENTIRE:
        self._entire(
            ticket.operation_id, ticket.name, ticket.payload, ticket.timeout)
      elif ticket.kind is tickets.Kind.CANCELLATION:
        self._cancel(ticket.operation_id)
      else:
        # NOTE(nathaniel): All other categories are treated as cancellation.
        self._cancel(ticket.operation_id)
