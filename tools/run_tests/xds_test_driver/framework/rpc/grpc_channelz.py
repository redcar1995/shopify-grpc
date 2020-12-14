# Copyright 2020 gRPC authors.
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
import ipaddress
import logging
from typing import Optional, Iterator

import grpc
from grpc_channelz.v1 import channelz_pb2
from grpc_channelz.v1 import channelz_pb2_grpc

import framework.rpc

logger = logging.getLogger(__name__)

# Type aliases
# Channel
Channel = channelz_pb2.Channel
ChannelConnectivityState = channelz_pb2.ChannelConnectivityState
GetTopChannelsRequest = channelz_pb2.GetTopChannelsRequest
GetTopChannelsResponse = channelz_pb2.GetTopChannelsResponse
# Subchannel
Subchannel = channelz_pb2.Subchannel
GetSubchannelRequest = channelz_pb2.GetSubchannelRequest
GetSubchannelResponse = channelz_pb2.GetSubchannelResponse
# Server
Server = channelz_pb2.Server
GetServersRequest = channelz_pb2.GetServersRequest
GetServersResponse = channelz_pb2.GetServersResponse
# Sockets
Socket = channelz_pb2.Socket
SocketRef = channelz_pb2.SocketRef
GetSocketRequest = channelz_pb2.GetSocketRequest
GetSocketResponse = channelz_pb2.GetSocketResponse
Address = channelz_pb2.Address
Security = channelz_pb2.Security
# Server Sockets
GetServerSocketsRequest = channelz_pb2.GetServerSocketsRequest
GetServerSocketsResponse = channelz_pb2.GetServerSocketsResponse


class ChannelzServiceClient(framework.rpc.GrpcClientHelper):
    stub: channelz_pb2_grpc.ChannelzStub

    def __init__(self, channel: grpc.Channel):
        super().__init__(channel, channelz_pb2_grpc.ChannelzStub)

    @staticmethod
    def is_sock_tcpip_address(address: Address):
        return address.WhichOneof('address') == 'tcpip_address'

    @staticmethod
    def is_ipv4(tcpip_address: Address.TcpIpAddress):
        # According to proto, tcpip_address.ip_address is either IPv4 or IPv6.
        # Correspondingly, it's either 4 bytes or 16 bytes in length.
        return len(tcpip_address.ip_address) == 4

    @classmethod
    def sock_address_to_str(cls, address: Address):
        if cls.is_sock_tcpip_address(address):
            tcpip_address: Address.TcpIpAddress = address.tcpip_address
            if cls.is_ipv4(tcpip_address):
                ip = ipaddress.IPv4Address(tcpip_address.ip_address)
            else:
                ip = ipaddress.IPv6Address(tcpip_address.ip_address)
            return f'{ip}:{tcpip_address.port}'
        else:
            raise NotImplementedError('Only tcpip_address implemented')

    @classmethod
    def sock_addresses_pretty(cls, socket: Socket):
        return (f'local={cls.sock_address_to_str(socket.local)}, '
                f'remote={cls.sock_address_to_str(socket.remote)}')

    @staticmethod
    def find_server_socket_matching_client(server_sockets: Iterator[Socket],
                                           client_socket: Socket) -> Socket:
        for server_socket in server_sockets:
            if server_socket.remote == client_socket.local:
                return server_socket
        return None

    def find_channels_for_target(self, target: str) -> Iterator[Channel]:
        return (channel for channel in self.list_channels()
                if channel.data.target == target)

    def find_server_listening_on_port(self, port: int) -> Optional[Server]:
        for server in self.list_servers():
            listen_socket_ref: SocketRef
            for listen_socket_ref in server.listen_socket:
                listen_socket = self.get_socket(listen_socket_ref.socket_id)
                listen_address: Address = listen_socket.local
                if (self.is_sock_tcpip_address(listen_address) and
                        listen_address.tcpip_address.port == port):
                    return server
        return None

    def list_channels(self) -> Iterator[Channel]:
        """
        Iterate over all pages of all root channels.

        Root channels are those which application has directly created.
        This does not include subchannels nor non-top level channels.
        """
        start: int = -1
        response: Optional[GetTopChannelsResponse] = None
        while start < 0 or not response.end:
            # From proto: To request subsequent pages, the client generates this
            # value by adding 1 to the highest seen result ID.
            start += 1
            response = self.call_unary_when_channel_ready(
                rpc='GetTopChannels',
                req=GetTopChannelsRequest(start_channel_id=start))
            for channel in response.channel:
                start = max(start, channel.ref.channel_id)
                yield channel

    def list_servers(self) -> Iterator[Server]:
        """Iterate over all pages of all servers that exist in the process."""
        start: int = -1
        response: Optional[GetServersResponse] = None
        while start < 0 or not response.end:
            # From proto: To request subsequent pages, the client generates this
            # value by adding 1 to the highest seen result ID.
            start += 1
            response = self.call_unary_when_channel_ready(
                rpc='GetServers', req=GetServersRequest(start_server_id=start))
            for server in response.server:
                start = max(start, server.ref.server_id)
                yield server

    def list_server_sockets(self, server_id) -> Iterator[Socket]:
        """Iterate over all server sockets that exist in server process."""
        start: int = -1
        response: Optional[GetServerSocketsResponse] = None
        while start < 0 or not response.end:
            # From proto: To request subsequent pages, the client generates this
            # value by adding 1 to the highest seen result ID.
            start += 1
            response = self.call_unary_when_channel_ready(
                rpc='GetServerSockets',
                req=GetServerSocketsRequest(server_id=server_id,
                                            start_socket_id=start))
            socket_ref: SocketRef
            for socket_ref in response.socket_ref:
                start = max(start, socket_ref.socket_id)
                # Yield actual socket
                yield self.get_socket(socket_ref.socket_id)

    def get_subchannel(self, subchannel_id) -> Subchannel:
        """Return a single Subchannel, otherwise raises RpcError."""
        response: GetSubchannelResponse = self.call_unary_when_channel_ready(
            rpc='GetSubchannel',
            req=GetSubchannelRequest(subchannel_id=subchannel_id))
        return response.subchannel

    def get_socket(self, socket_id) -> Socket:
        """Return a single Socket, otherwise raises RpcError."""
        response: GetSocketResponse = self.call_unary_when_channel_ready(
            rpc='GetSocket', req=GetSocketRequest(socket_id=socket_id))
        return response.socket
