# Copyright 2019 gRPC authors.
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
"""Invocation-side implementation of gRPC Asyncio Python."""

import asyncio
from functools import partial
import logging
from typing import AsyncIterable, Awaitable, Dict, Optional

import grpc
from grpc import _common
from grpc._cython import cygrpc

from . import _base_call
from ._typing import (DeserializingFunction, DoneCallbackType, MetadataType,
                      RequestType, ResponseType, SerializingFunction)

__all__ = 'AioRpcError', 'Call', 'UnaryUnaryCall', 'UnaryStreamCall'

_LOCAL_CANCELLATION_DETAILS = 'Locally cancelled by application!'
_GC_CANCELLATION_DETAILS = 'Cancelled upon garbage collection!'
_RPC_ALREADY_FINISHED_DETAILS = 'RPC already finished.'
_RPC_HALF_CLOSED_DETAILS = 'RPC is half closed after calling "done_writing".'

_OK_CALL_REPRESENTATION = ('<{} of RPC that terminated with:\n'
                           '\tstatus = {}\n'
                           '\tdetails = "{}"\n'
                           '>')

_NON_OK_CALL_REPRESENTATION = ('<{} of RPC that terminated with:\n'
                               '\tstatus = {}\n'
                               '\tdetails = "{}"\n'
                               '\tdebug_error_string = "{}"\n'
                               '>')

_LOGGER = logging.getLogger(__name__)


class AioRpcError(grpc.RpcError):
    """An implementation of RpcError to be used by the asynchronous API.

    Raised RpcError is a snapshot of the final status of the RPC, values are
    determined. Hence, its methods no longer needs to be coroutines.
    """

    # TODO(https://github.com/grpc/grpc/issues/20144) Metadata
    # type returned by `initial_metadata` and `trailing_metadata`
    # and also taken in the constructor needs to be revisit and make
    # it more specific.

    _code: grpc.StatusCode
    _details: Optional[str]
    _initial_metadata: Optional[MetadataType]
    _trailing_metadata: Optional[MetadataType]
    _debug_error_string: Optional[str]

    def __init__(self,
                 code: grpc.StatusCode,
                 details: Optional[str] = None,
                 initial_metadata: Optional[MetadataType] = None,
                 trailing_metadata: Optional[MetadataType] = None,
                 debug_error_string: Optional[str] = None) -> None:
        """Constructor.

        Args:
          code: The status code with which the RPC has been finalized.
          details: Optional details explaining the reason of the error.
          initial_metadata: Optional initial metadata that could be sent by the
            Server.
          trailing_metadata: Optional metadata that could be sent by the Server.
        """

        super().__init__(self)
        self._code = code
        self._details = details
        self._initial_metadata = initial_metadata
        self._trailing_metadata = trailing_metadata
        self._debug_error_string = debug_error_string

    def code(self) -> grpc.StatusCode:
        """Accesses the status code sent by the server.

        Returns:
          The `grpc.StatusCode` status code.
        """
        return self._code

    def details(self) -> Optional[str]:
        """Accesses the details sent by the server.

        Returns:
          The description of the error.
        """
        return self._details

    def initial_metadata(self) -> Optional[Dict]:
        """Accesses the initial metadata sent by the server.

        Returns:
          The initial metadata received.
        """
        return self._initial_metadata

    def trailing_metadata(self) -> Optional[Dict]:
        """Accesses the trailing metadata sent by the server.

        Returns:
          The trailing metadata received.
        """
        return self._trailing_metadata

    def debug_error_string(self) -> str:
        """Accesses the debug error string sent by the server.

        Returns:
          The debug error string received.
        """
        return self._debug_error_string

    def _repr(self) -> str:
        """Assembles the error string for the RPC error."""
        return _NON_OK_CALL_REPRESENTATION.format(self.__class__.__name__,
                                                  self._code, self._details,
                                                  self._debug_error_string)

    def __repr__(self) -> str:
        return self._repr()

    def __str__(self) -> str:
        return self._repr()


def _create_rpc_error(initial_metadata: Optional[MetadataType],
                      status: cygrpc.AioRpcStatus,
                      debug_error_string: Optional[str] = None) -> AioRpcError:
    return AioRpcError(
        _common.CYGRPC_STATUS_CODE_TO_STATUS_CODE[status.code()],
        status.details(),
        initial_metadata,
        status.trailing_metadata(),
        debug_error_string,
    )


class Call:
    """Base implementation of client RPC Call object.

    Implements logic around final status, metadata and cancellation.
    """
    _loop: asyncio.AbstractEventLoop
    _code: grpc.StatusCode
    _cython_call: cygrpc._AioCall
    _metadata: MetadataType
    _request_serializer: SerializingFunction
    _response_deserializer: DeserializingFunction

    def __init__(self, cython_call: cygrpc._AioCall, metadata: MetadataType,
                 request_serializer: SerializingFunction,
                 response_deserializer: DeserializingFunction,
                 loop: asyncio.AbstractEventLoop) -> None:
        self._loop = loop
        self._cython_call = cython_call
        self._metadata = metadata
        self._request_serializer = request_serializer
        self._response_deserializer = response_deserializer

    def __del__(self) -> None:
        # The '_cython_call' object might be destructed before Call object
        if hasattr(self, '_cython_call'):
            if not self._cython_call.done():
                self._cancel(_GC_CANCELLATION_DETAILS)

    def cancelled(self) -> bool:
        return self._cython_call.cancelled()

    def _cancel(self, details: str) -> bool:
        """Forwards the application cancellation reasoning."""
        if not self._cython_call.done():
            self._cython_call.cancel(details)
            return True
        else:
            return False

    def cancel(self) -> bool:
        return self._cancel(_LOCAL_CANCELLATION_DETAILS)

    def done(self) -> bool:
        return self._cython_call.done()

    def add_done_callback(self, callback: DoneCallbackType) -> None:
        cb = partial(callback, self)
        self._cython_call.add_done_callback(cb)

    def time_remaining(self) -> Optional[float]:
        return self._cython_call.time_remaining()

    async def initial_metadata(self) -> MetadataType:
        return await self._cython_call.initial_metadata()

    async def trailing_metadata(self) -> MetadataType:
        return (await self._cython_call.status()).trailing_metadata()

    async def code(self) -> grpc.StatusCode:
        cygrpc_code = (await self._cython_call.status()).code()
        return _common.CYGRPC_STATUS_CODE_TO_STATUS_CODE[cygrpc_code]

    async def details(self) -> str:
        return (await self._cython_call.status()).details()

    async def debug_error_string(self) -> str:
        return (await self._cython_call.status()).debug_error_string()

    async def _raise_for_status(self) -> None:
        if self._cython_call.is_locally_cancelled():
            raise asyncio.CancelledError()
        code = await self.code()
        if code != grpc.StatusCode.OK:
            raise _create_rpc_error(await self.initial_metadata(), await
                                    self._cython_call.status())

    def _repr(self) -> str:
        return repr(self._cython_call)

    def __repr__(self) -> str:
        return self._repr()

    def __str__(self) -> str:
        return self._repr()


class _UnaryResponseMixin(Call):
    _call_response: asyncio.Task

    def _init_unary_response_mixin(self,
                                   response_coro: Awaitable[ResponseType]):
        self._call_response = self._loop.create_task(response_coro)

    def cancel(self) -> bool:
        if super().cancel():
            self._call_response.cancel()
            return True
        else:
            return False

    def __await__(self) -> ResponseType:
        """Wait till the ongoing RPC request finishes."""
        try:
            response = yield from self._call_response
        except asyncio.CancelledError:
            # Even if we caught all other CancelledError, there is still
            # this corner case. If the application cancels immediately after
            # the Call object is created, we will observe this
            # `CancelledError`.
            if not self.cancelled():
                self.cancel()
            raise

        # NOTE(lidiz) If we raise RpcError in the task, and users doesn't
        # 'await' on it. AsyncIO will log 'Task exception was never retrieved'.
        # Instead, if we move the exception raising here, the spam stops.
        # Unfortunately, there can only be one 'yield from' in '__await__'. So,
        # we need to access the private instance variable.
        if response is cygrpc.EOF:
            if self._cython_call.is_locally_cancelled():
                raise asyncio.CancelledError()
            else:
                call_status = self._cython_call._status
                debug_error_string = None
                if call_status is not None:
                    debug_error_string = call_status._debug_error_string
                raise _create_rpc_error(self._cython_call._initial_metadata,
                                        call_status, debug_error_string)
        else:
            return response


class _StreamResponseMixin(Call):
    _message_aiter: AsyncIterable[ResponseType]
    _preparation: asyncio.Task

    def _init_stream_response_mixin(self, preparation: asyncio.Task):
        self._message_aiter = None
        self._preparation = preparation

    def cancel(self) -> bool:
        if super().cancel():
            self._preparation.cancel()
            return True
        else:
            return False

    async def _fetch_stream_responses(self) -> ResponseType:
        message = await self._read()
        while message is not cygrpc.EOF:
            yield message
            message = await self._read()

    def __aiter__(self) -> AsyncIterable[ResponseType]:
        if self._message_aiter is None:
            self._message_aiter = self._fetch_stream_responses()
        return self._message_aiter

    async def _read(self) -> ResponseType:
        # Wait for the request being sent
        await self._preparation

        # Reads response message from Core
        try:
            raw_response = await self._cython_call.receive_serialized_message()
        except asyncio.CancelledError:
            if not self.cancelled():
                self.cancel()
            await self._raise_for_status()

        if raw_response is cygrpc.EOF:
            return cygrpc.EOF
        else:
            return _common.deserialize(raw_response,
                                       self._response_deserializer)

    async def read(self) -> ResponseType:
        if self.done():
            await self._raise_for_status()
            return cygrpc.EOF

        response_message = await self._read()

        if response_message is cygrpc.EOF:
            # If the read operation failed, Core should explain why.
            await self._raise_for_status()
        return response_message


class _StreamRequestMixin(Call):
    _metadata_sent: asyncio.Event
    _done_writing: bool
    _async_request_poller: Optional[asyncio.Task]

    def _init_stream_request_mixin(
            self, request_async_iterator: Optional[AsyncIterable[RequestType]]):
        self._metadata_sent = asyncio.Event(loop=self._loop)
        self._done_writing = False

        # If user passes in an async iterator, create a consumer Task.
        if request_async_iterator is not None:
            self._async_request_poller = self._loop.create_task(
                self._consume_request_iterator(request_async_iterator))
        else:
            self._async_request_poller = None

    def cancel(self) -> bool:
        if super().cancel():
            if self._async_request_poller is not None:
                self._async_request_poller.cancel()
            return True
        else:
            return False

    def _metadata_sent_observer(self):
        self._metadata_sent.set()

    async def _consume_request_iterator(
            self, request_async_iterator: AsyncIterable[RequestType]) -> None:
        try:
            async for request in request_async_iterator:
                await self.write(request)
            await self.done_writing()
        except AioRpcError as rpc_error:
            # Rpc status should be exposed through other API. Exceptions raised
            # within this Task won't be retrieved by another coroutine. It's
            # better to suppress the error than spamming users' screen.
            _LOGGER.debug('Exception while consuming the request_iterator: %s',
                          rpc_error)

    async def write(self, request: RequestType) -> None:
        if self.done():
            raise asyncio.InvalidStateError(_RPC_ALREADY_FINISHED_DETAILS)
        if self._done_writing:
            raise asyncio.InvalidStateError(_RPC_HALF_CLOSED_DETAILS)
        if not self._metadata_sent.is_set():
            await self._metadata_sent.wait()
            if self.done():
                await self._raise_for_status()

        serialized_request = _common.serialize(request,
                                               self._request_serializer)

        try:
            await self._cython_call.send_serialized_message(serialized_request)
        except asyncio.CancelledError:
            if not self.cancelled():
                self.cancel()
            await self._raise_for_status()

    async def done_writing(self) -> None:
        """Implementation of done_writing is idempotent."""
        if self.done():
            # If the RPC is finished, do nothing.
            return
        if not self._done_writing:
            # If the done writing is not sent before, try to send it.
            self._done_writing = True
            try:
                await self._cython_call.send_receive_close()
            except asyncio.CancelledError:
                if not self.cancelled():
                    self.cancel()
                await self._raise_for_status()


class UnaryUnaryCall(_UnaryResponseMixin, Call, _base_call.UnaryUnaryCall):
    """Object for managing unary-unary RPC calls.

    Returned when an instance of `UnaryUnaryMultiCallable` object is called.
    """
    _request: RequestType

    # pylint: disable=too-many-arguments
    def __init__(self, request: RequestType, deadline: Optional[float],
                 metadata: MetadataType,
                 credentials: Optional[grpc.CallCredentials],
                 wait_for_ready: Optional[bool], channel: cygrpc.AioChannel,
                 method: bytes, request_serializer: SerializingFunction,
                 response_deserializer: DeserializingFunction,
                 loop: asyncio.AbstractEventLoop) -> None:
        super().__init__(
            channel.call(method, deadline, credentials, wait_for_ready),
            metadata, request_serializer, response_deserializer, loop)
        self._request = request
        self._init_unary_response_mixin(self._invoke())

    async def _invoke(self) -> ResponseType:
        serialized_request = _common.serialize(self._request,
                                               self._request_serializer)

        # NOTE(lidiz) asyncio.CancelledError is not a good transport for status,
        # because the asyncio.Task class do not cache the exception object.
        # https://github.com/python/cpython/blob/edad4d89e357c92f70c0324b937845d652b20afd/Lib/asyncio/tasks.py#L785
        try:
            serialized_response = await self._cython_call.unary_unary(
                serialized_request, self._metadata)
        except asyncio.CancelledError:
            if not self.cancelled():
                self.cancel()

        if self._cython_call.is_ok():
            return _common.deserialize(serialized_response,
                                       self._response_deserializer)
        else:
            return cygrpc.EOF


class UnaryStreamCall(_StreamResponseMixin, Call, _base_call.UnaryStreamCall):
    """Object for managing unary-stream RPC calls.

    Returned when an instance of `UnaryStreamMultiCallable` object is called.
    """
    _request: RequestType
    _send_unary_request_task: asyncio.Task

    # pylint: disable=too-many-arguments
    def __init__(self, request: RequestType, deadline: Optional[float],
                 metadata: MetadataType,
                 credentials: Optional[grpc.CallCredentials],
                 wait_for_ready: Optional[bool], channel: cygrpc.AioChannel,
                 method: bytes, request_serializer: SerializingFunction,
                 response_deserializer: DeserializingFunction,
                 loop: asyncio.AbstractEventLoop) -> None:
        super().__init__(
            channel.call(method, deadline, credentials, wait_for_ready),
            metadata, request_serializer, response_deserializer, loop)
        self._request = request
        self._send_unary_request_task = loop.create_task(
            self._send_unary_request())
        self._init_stream_response_mixin(self._send_unary_request_task)

    async def _send_unary_request(self) -> ResponseType:
        serialized_request = _common.serialize(self._request,
                                               self._request_serializer)
        try:
            await self._cython_call.initiate_unary_stream(
                serialized_request, self._metadata)
        except asyncio.CancelledError:
            if not self.cancelled():
                self.cancel()
            raise


class StreamUnaryCall(_StreamRequestMixin, _UnaryResponseMixin, Call,
                      _base_call.StreamUnaryCall):
    """Object for managing stream-unary RPC calls.

    Returned when an instance of `StreamUnaryMultiCallable` object is called.
    """

    # pylint: disable=too-many-arguments
    def __init__(self,
                 request_async_iterator: Optional[AsyncIterable[RequestType]],
                 deadline: Optional[float], metadata: MetadataType,
                 credentials: Optional[grpc.CallCredentials],
                 wait_for_ready: Optional[bool], channel: cygrpc.AioChannel,
                 method: bytes, request_serializer: SerializingFunction,
                 response_deserializer: DeserializingFunction,
                 loop: asyncio.AbstractEventLoop) -> None:
        super().__init__(
            channel.call(method, deadline, credentials, wait_for_ready),
            metadata, request_serializer, response_deserializer, loop)

        self._init_stream_request_mixin(request_async_iterator)
        self._init_unary_response_mixin(self._conduct_rpc())

    async def _conduct_rpc(self) -> ResponseType:
        try:
            serialized_response = await self._cython_call.stream_unary(
                self._metadata, self._metadata_sent_observer)
        except asyncio.CancelledError:
            if not self.cancelled():
                self.cancel()

        if self._cython_call.is_ok():
            return _common.deserialize(serialized_response,
                                       self._response_deserializer)
        else:
            return cygrpc.EOF


class StreamStreamCall(_StreamRequestMixin, _StreamResponseMixin, Call,
                       _base_call.StreamStreamCall):
    """Object for managing stream-stream RPC calls.

    Returned when an instance of `StreamStreamMultiCallable` object is called.
    """
    _initializer: asyncio.Task

    # pylint: disable=too-many-arguments
    def __init__(self,
                 request_async_iterator: Optional[AsyncIterable[RequestType]],
                 deadline: Optional[float], metadata: MetadataType,
                 credentials: Optional[grpc.CallCredentials],
                 wait_for_ready: Optional[bool], channel: cygrpc.AioChannel,
                 method: bytes, request_serializer: SerializingFunction,
                 response_deserializer: DeserializingFunction,
                 loop: asyncio.AbstractEventLoop) -> None:
        super().__init__(
            channel.call(method, deadline, credentials, wait_for_ready),
            metadata, request_serializer, response_deserializer, loop)
        self._initializer = self._loop.create_task(self._prepare_rpc())
        self._init_stream_request_mixin(request_async_iterator)
        self._init_stream_response_mixin(self._initializer)

    async def _prepare_rpc(self):
        """This method prepares the RPC for receiving/sending messages.

        All other operations around the stream should only happen after the
        completion of this method.
        """
        try:
            await self._cython_call.initiate_stream_stream(
                self._metadata, self._metadata_sent_observer)
        except asyncio.CancelledError:
            if not self.cancelled():
                self.cancel()
            # No need to raise RpcError here, because no one will `await` this task.
