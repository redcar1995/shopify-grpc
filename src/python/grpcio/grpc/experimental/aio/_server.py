# Copyright 2019 The gRPC Authors
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
"""Server-side implementation of gRPC Asyncio Python."""

from typing import Text, Optional
import asyncio
import grpc
from grpc._cython import cygrpc

class Server:
    """Serves RPCs."""

    def __init__(self, thread_pool, generic_handlers, interceptors, options,
                 maximum_concurrent_rpcs, compression):
        self._server = cygrpc.AioServer(thread_pool, generic_handlers,
                                        interceptors, options,
                                        maximum_concurrent_rpcs, compression)

    def add_generic_rpc_handlers(
            self,
            generic_rpc_handlers,
            # generic_rpc_handlers: Iterable[grpc.GenericRpcHandlers]
    ) -> None:
        """Registers GenericRpcHandlers with this Server.

        This method is only safe to call before the server is started.

        Args:
          generic_rpc_handlers: An iterable of GenericRpcHandlers that will be
          used to service RPCs.
        """
        self._server.add_generic_rpc_handlers(generic_rpc_handlers)

    def add_insecure_port(self, address: Text) -> int:
        """Opens an insecure port for accepting RPCs.

        This method may only be called before starting the server.

        Args:
          address: The address for which to open a port. If the port is 0,
            or not specified in the address, then gRPC runtime will choose a port.

        Returns:
          An integer port on which server will accept RPC requests.
        """
        return self._server.add_insecure_port(address)

    def add_secure_port(self, address: Text,
                        server_credentials: grpc.ServerCredentials) -> int:
        """Opens a secure port for accepting RPCs.

        This method may only be called before starting the server.

        Args:
          address: The address for which to open a port.
            if the port is 0, or not specified in the address, then gRPC
            runtime will choose a port.
          server_credentials: A ServerCredentials object.

        Returns:
          An integer port on which server will accept RPC requests.
        """
        return self._server.add_secure_port(address, server_credentials)

    async def start(self) -> None:
        """Starts this Server.

        This method may only be called once. (i.e. it is not idempotent).
        """
        await self._server.start()

    def stop(self, grace: Optional[float]) -> asyncio.Event:
        """Stops this Server.

        This method immediately stop service of new RPCs in all cases.

        If a grace period is specified, this method returns immediately
        and all RPCs active at the end of the grace period are aborted.
        If a grace period is not specified (by passing None for `grace`),
        all existing RPCs are aborted immediately and this method
        blocks until the last RPC handler terminates.

        This method is idempotent and may be called at any time.
        Passing a smaller grace value in a subsequent call will have
        the effect of stopping the Server sooner (passing None will
        have the effect of stopping the server immediately). Passing
        a larger grace value in a subsequent call *will not* have the
        effect of stopping the server later (i.e. the most restrictive
        grace value is used).

        Args:
          grace: A duration of time in seconds or None.

        Returns:
          A threading.Event that will be set when this Server has completely
          stopped, i.e. when running RPCs either complete or are aborted and
          all handlers have terminated.
        """
        raise NotImplementedError()

    async def wait_for_termination(self,
                                   timeout: Optional[float] = None) -> bool:
        """Block current thread until the server stops.

        This is an EXPERIMENTAL API.

        The wait will not consume computational resources during blocking, and
        it will block until one of the two following conditions are met:

        1) The server is stopped or terminated;
        2) A timeout occurs if timeout is not `None`.

        The timeout argument works in the same way as `threading.Event.wait()`.
        https://docs.python.org/3/library/threading.html#threading.Event.wait

        Args:
          timeout: A floating point number specifying a timeout for the
            operation in seconds.

        Returns:
          A bool indicates if the operation times out.
        """
        if timeout:
            raise NotImplementedError()
        # TODO(lidiz) replace this wait forever logic
        future = asyncio.get_event_loop().create_future()
        await future


def server(thread_pool=None,
           handlers=None,
           interceptors=None,
           options=None,
           maximum_concurrent_rpcs=None,
           compression=None):
    """Creates a Server with which RPCs can be serviced.

    Args:
      thread_pool: A futures.ThreadPoolExecutor to be used by the Server
        to execute RPC handlers.
      handlers: An optional list of GenericRpcHandlers used for executing RPCs.
        More handlers may be added by calling add_generic_rpc_handlers any time
        before the server is started.
      interceptors: An optional list of ServerInterceptor objects that observe
        and optionally manipulate the incoming RPCs before handing them over to
        handlers. The interceptors are given control in the order they are
        specified. This is an EXPERIMENTAL API.
      options: An optional list of key-value pairs (channel args in gRPC runtime)
        to configure the channel.
      maximum_concurrent_rpcs: The maximum number of concurrent RPCs this server
        will service before returning RESOURCE_EXHAUSTED status, or None to
        indicate no limit.
      compression: An element of grpc.compression, e.g.
        grpc.compression.Gzip. This compression algorithm will be used for the
        lifetime of the server unless overridden. This is an EXPERIMENTAL option.

    Returns:
      A Server object.
    """
    return Server(thread_pool, () if handlers is None else handlers, ()
                  if interceptors is None else interceptors, ()
                  if options is None else options, maximum_concurrent_rpcs,
                  compression)
