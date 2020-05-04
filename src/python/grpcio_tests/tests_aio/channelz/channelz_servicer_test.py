# Copyright 2020 The gRPC Authors
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
"""Tests of grpc_channelz.v1.channelz."""

import unittest
import logging
import asyncio

import grpc
from grpc.experimental import aio

from grpc_channelz.v1 import channelz
from grpc_channelz.v1 import channelz_pb2
from grpc_channelz.v1 import channelz_pb2_grpc

from tests.unit.framework.common import test_constants
from tests_aio.unit._test_base import AioTestBase

aio.shutdown_grpc_aio()

_SUCCESSFUL_UNARY_UNARY = '/test/SuccessfulUnaryUnary'
_FAILED_UNARY_UNARY = '/test/FailedUnaryUnary'
_SUCCESSFUL_STREAM_STREAM = '/test/SuccessfulStreamStream'

_REQUEST = b'\x00\x00\x00'
_RESPONSE = b'\x01\x01\x01'

_DISABLE_REUSE_PORT = (('grpc.so_reuseport', 0),)
_ENABLE_CHANNELZ = (('grpc.enable_channelz', 1),)
_DISABLE_CHANNELZ = (('grpc.enable_channelz', 0),)


async def _successful_unary_unary(request, servicer_context):
    return _RESPONSE


async def _failed_unary_unary(request, servicer_context):
    servicer_context.set_code(grpc.StatusCode.INTERNAL)
    servicer_context.set_details("Channelz Test Intended Failure")


async def _successful_stream_stream(request_iterator, servicer_context):
    async for _ in request_iterator:
        yield _RESPONSE


class _GenericHandler(grpc.GenericRpcHandler):

    def service(self, handler_call_details):
        if handler_call_details.method == _SUCCESSFUL_UNARY_UNARY:
            return grpc.unary_unary_rpc_method_handler(_successful_unary_unary)
        elif handler_call_details.method == _FAILED_UNARY_UNARY:
            return grpc.unary_unary_rpc_method_handler(_failed_unary_unary)
        elif handler_call_details.method == _SUCCESSFUL_STREAM_STREAM:
            return grpc.stream_stream_rpc_method_handler(
                _successful_stream_stream)
        else:
            return None


class _ChannelServerPair(object):

    async def start(self):
        # Server will enable channelz service
        self.server = aio.server(options=_DISABLE_REUSE_PORT + _ENABLE_CHANNELZ)
        port = self.server.add_insecure_port('[::]:0')
        self.server.add_generic_rpc_handlers((_GenericHandler(),))
        await self.server.start()

        # Channel will enable channelz service...
        self.channel = aio.insecure_channel('localhost:%d' % port,
                                            options=_ENABLE_CHANNELZ)


# Stores channel-server pairs globally, since the memory deallocation is
# non-deterministic in both Core and Python with multiple threads. The
# destroyed Channelz node might still present. So, as a work around, this
# test doesn't close channel-server-pairs between cases.
_pairs = []


async def _generate_channel_server_pairs(n):
    """Creates channel-server pairs globally, returns their indexes."""
    new_pairs = [_ChannelServerPair() for i in range(n)]
    for pair in new_pairs:
        await pair.start()
    _pairs.extend(new_pairs)
    return list(range(len(_pairs) - n, len(_pairs)))


class ChannelzServicerTest(AioTestBase):

    async def setUp(self):
        self._pairs = []
        # This server is for Channelz info fetching only
        # It self should not enable Channelz
        self._server = aio.server(options=_DISABLE_REUSE_PORT +
                                  _DISABLE_CHANNELZ)
        port = self._server.add_insecure_port('[::]:0')
        channelz.add_channelz_servicer(self._server)
        await self._server.start()

        # This channel is used to fetch Channelz info only
        # Channelz should not be enabled
        self._channel = aio.insecure_channel('localhost:%d' % port,
                                             options=_DISABLE_CHANNELZ)
        self._channelz_stub = channelz_pb2_grpc.ChannelzStub(self._channel)

    async def tearDown(self):
        await self._server.stop(None)
        await self._channel.close()

    async def _send_successful_unary_unary(self, idx):
        call = _pairs[idx].channel.unary_unary(_SUCCESSFUL_UNARY_UNARY)(
            _REQUEST)
        self.assertEqual(grpc.StatusCode.OK, await call.code())

    async def _send_failed_unary_unary(self, idx):
        try:
            await _pairs[idx].channel.unary_unary(_FAILED_UNARY_UNARY)(_REQUEST)
        except grpc.RpcError:
            return
        else:
            self.fail("This call supposed to fail")

    async def _send_successful_stream_stream(self, idx):
        call = _pairs[idx].channel.stream_stream(_SUCCESSFUL_STREAM_STREAM)(
            iter([_REQUEST] * test_constants.STREAM_LENGTH))
        cnt = 0
        async for _ in call:
            cnt += 1
        self.assertEqual(cnt, test_constants.STREAM_LENGTH)

    async def _get_channel_id(self, idx):
        """Channel id may not be consecutive"""
        resp = await self._channelz_stub.GetTopChannels(
            channelz_pb2.GetTopChannelsRequest(start_channel_id=0))
        self.assertGreater(len(resp.channel), idx)
        return resp.channel[idx].ref.channel_id

    async def _get_server_by_id(self, idx):
        """Server id may not be consecutive"""
        resp = await self._channelz_stub.GetServers(
            channelz_pb2.GetServersRequest(start_server_id=0))
        return resp.server[idx]

    async def test_get_top_channels_basic(self):
        before = await self._channelz_stub.GetTopChannels(
            channelz_pb2.GetTopChannelsRequest(start_channel_id=0))
        await _generate_channel_server_pairs(1)
        after = await self._channelz_stub.GetTopChannels(
            channelz_pb2.GetTopChannelsRequest(start_channel_id=0))
        self.assertEqual(len(after.channel) - len(before.channel), 1)
        self.assertEqual(after.end, True)

    async def test_get_top_channels_high_start_id(self):
        await _generate_channel_server_pairs(1)
        resp = await self._channelz_stub.GetTopChannels(
            channelz_pb2.GetTopChannelsRequest(start_channel_id=10000))
        self.assertEqual(len(resp.channel), 0)
        self.assertEqual(resp.end, True)

    async def test_successful_request(self):
        idx = await _generate_channel_server_pairs(1)
        await self._send_successful_unary_unary(idx[0])
        resp = await self._channelz_stub.GetChannel(
            channelz_pb2.GetChannelRequest(
                channel_id=await self._get_channel_id(idx[0])))
        self.assertEqual(resp.channel.data.calls_started, 1)
        self.assertEqual(resp.channel.data.calls_succeeded, 1)
        self.assertEqual(resp.channel.data.calls_failed, 0)

    async def test_failed_request(self):
        idx = await _generate_channel_server_pairs(1)
        await self._send_failed_unary_unary(idx[0])
        resp = await self._channelz_stub.GetChannel(
            channelz_pb2.GetChannelRequest(
                channel_id=await self._get_channel_id(idx[0])))
        self.assertEqual(resp.channel.data.calls_started, 1)
        self.assertEqual(resp.channel.data.calls_succeeded, 0)
        self.assertEqual(resp.channel.data.calls_failed, 1)

    async def test_many_requests(self):
        idx = await _generate_channel_server_pairs(1)
        k_success = 7
        k_failed = 9
        for i in range(k_success):
            await self._send_successful_unary_unary(idx[0])
        for i in range(k_failed):
            await self._send_failed_unary_unary(idx[0])
        resp = await self._channelz_stub.GetChannel(
            channelz_pb2.GetChannelRequest(
                channel_id=await self._get_channel_id(idx[0])))
        self.assertEqual(resp.channel.data.calls_started, k_success + k_failed)
        self.assertEqual(resp.channel.data.calls_succeeded, k_success)
        self.assertEqual(resp.channel.data.calls_failed, k_failed)

    async def test_server_call(self):
        idx = await _generate_channel_server_pairs(1)
        k_success = 23
        k_failed = 29
        for i in range(k_success):
            await self._send_successful_unary_unary(idx[0])
        for i in range(k_failed):
            await self._send_failed_unary_unary(idx[0])

        resp = await self._get_server_by_id(idx[0])
        self.assertEqual(resp.data.calls_started, k_success + k_failed)
        self.assertEqual(resp.data.calls_succeeded, k_success)
        self.assertEqual(resp.data.calls_failed, k_failed)

    async def test_streaming_rpc(self):
        idx = await _generate_channel_server_pairs(1)
        # In C++, the argument for _send_successful_stream_stream is message length.
        # Here the argument is still channel idx, to be consistent with the other two.
        await self._send_successful_stream_stream(idx[0])

        gc_resp = await self._channelz_stub.GetChannel(
            channelz_pb2.GetChannelRequest(
                channel_id=await self._get_channel_id(idx[0])))
        self.assertEqual(gc_resp.channel.data.calls_started, 1)
        self.assertEqual(gc_resp.channel.data.calls_succeeded, 1)
        self.assertEqual(gc_resp.channel.data.calls_failed, 0)
        # Subchannel exists
        self.assertGreater(len(gc_resp.channel.subchannel_ref), 0)

        gsc_resp = await self._channelz_stub.GetSubchannel(
            channelz_pb2.GetSubchannelRequest(
                subchannel_id=gc_resp.channel.subchannel_ref[0].subchannel_id))
        self.assertEqual(gsc_resp.subchannel.data.calls_started, 1)
        self.assertEqual(gsc_resp.subchannel.data.calls_succeeded, 1)
        self.assertEqual(gsc_resp.subchannel.data.calls_failed, 0)
        # Socket exists
        self.assertEqual(len(gsc_resp.subchannel.socket_ref), 1)

        gs_resp = await self._channelz_stub.GetSocket(
            channelz_pb2.GetSocketRequest(
                socket_id=gsc_resp.subchannel.socket_ref[0].socket_id))
        self.assertEqual(gs_resp.socket.data.streams_started, 1)
        self.assertEqual(gs_resp.socket.data.streams_succeeded, 1)
        self.assertEqual(gs_resp.socket.data.streams_failed, 0)
        self.assertEqual(gs_resp.socket.data.messages_sent,
                         test_constants.STREAM_LENGTH)
        self.assertEqual(gs_resp.socket.data.messages_received,
                         test_constants.STREAM_LENGTH)

    async def test_server_sockets(self):
        idx = await _generate_channel_server_pairs(1)
        await self._send_successful_unary_unary(idx[0])
        await self._send_failed_unary_unary(idx[0])

        resp = await self._get_server_by_id(idx[0])
        self.assertEqual(resp.data.calls_started, 2)
        self.assertEqual(resp.data.calls_succeeded, 1)
        self.assertEqual(resp.data.calls_failed, 1)

        gss_resp = await self._channelz_stub.GetServerSockets(
            channelz_pb2.GetServerSocketsRequest(server_id=resp.ref.server_id,
                                                 start_socket_id=0))
        # If the RPC call failed, it will raise a grpc.RpcError
        # So, if there is no exception raised, considered pass

    async def test_server_listen_sockets(self):
        idx = await _generate_channel_server_pairs(1)

        resp = await self._get_server_by_id(idx[0])
        self.assertEqual(len(resp.listen_socket), 1)

        gs_resp = await self._channelz_stub.GetSocket(
            channelz_pb2.GetSocketRequest(
                socket_id=resp.listen_socket[0].socket_id))
        # If the RPC call failed, it will raise a grpc.RpcError
        # So, if there is no exception raised, considered pass

    async def test_invalid_query_get_server(self):
        with self.assertRaises(aio.AioRpcError) as exception_context:
            await self._channelz_stub.GetServer(
                channelz_pb2.GetServerRequest(server_id=10000))
        self.assertEqual(grpc.StatusCode.NOT_FOUND,
                         exception_context.exception.code())

    async def test_invalid_query_get_channel(self):
        with self.assertRaises(aio.AioRpcError) as exception_context:
            await self._channelz_stub.GetChannel(
                channelz_pb2.GetChannelRequest(channel_id=10000))
        self.assertEqual(grpc.StatusCode.NOT_FOUND,
                         exception_context.exception.code())

    async def test_invalid_query_get_subchannel(self):
        with self.assertRaises(aio.AioRpcError) as exception_context:
            await self._channelz_stub.GetSubchannel(
                channelz_pb2.GetSubchannelRequest(subchannel_id=10000))
        self.assertEqual(grpc.StatusCode.NOT_FOUND,
                         exception_context.exception.code())

    async def test_invalid_query_get_socket(self):
        with self.assertRaises(aio.AioRpcError) as exception_context:
            await self._channelz_stub.GetSocket(
                channelz_pb2.GetSocketRequest(socket_id=10000))
        self.assertEqual(grpc.StatusCode.NOT_FOUND,
                         exception_context.exception.code())

    async def test_invalid_query_get_server_sockets(self):
        with self.assertRaises(aio.AioRpcError) as exception_context:
            await self._channelz_stub.GetServerSockets(
                channelz_pb2.GetServerSocketsRequest(
                    server_id=10000,
                    start_socket_id=0,
                ))
        self.assertEqual(grpc.StatusCode.NOT_FOUND,
                         exception_context.exception.code())


if __name__ == '__main__':
    logging.basicConfig(level=logging.DEBUG)
    unittest.main(verbosity=2)
