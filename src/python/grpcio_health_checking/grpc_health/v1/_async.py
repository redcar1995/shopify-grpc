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
"""Reference implementation for health checking in gRPC Python."""

import logging
import asyncio
import collections

import grpc
from grpc.experimental import aio

from grpc_health.v1 import health_pb2 as _health_pb2
from grpc_health.v1 import health_pb2_grpc as _health_pb2_grpc


class AsyncHealthServicer(_health_pb2_grpc.HealthServicer):
    """An AsyncIO implementation of health checking servicer."""

    def __init__(self):
        self._lock = asyncio.Lock()
        self._server_status = dict()
        self._server_watchers = collections.defaultdict(asyncio.Condition)
        self._gracefully_shutting_down = False

    async def Check(self, request: _health_pb2.HealthCheckRequest, context):
        status = self._server_status.get(request.service)
        logging.debug('Status %s, %s', request.service, status)

        if status is None:
            await context.abort(grpc.StatusCode.NOT_FOUND)
        else:
            return _health_pb2.HealthCheckResponse(status=status)

    async def Watch(self, request: _health_pb2.HealthCheckRequest, context):
        status = self._server_status.get(request.service)

        if status is None:
            status = _health_pb2.HealthCheckResponse.SERVICE_UNKNOWN

        try:
            condition = self._server_watchers[request.service]
            async with condition:
                # Responds with current health state
                await context.write(
                    _health_pb2.HealthCheckResponse(status=status))

                # Polling on health state changes
                while True:
                    await condition.wait()

                    status = self._server_status.get(request.service)
                    await context.write(
                        _health_pb2.HealthCheckResponse(status=status))
        finally:
            del self._server_watchers[request.service]

    async def _set(self, service: str,
                   status: _health_pb2.HealthCheckResponse.ServingStatus):
        if service in self._server_watchers:
            condition = self._server_watchers.get(service)
            async with condition:
                self._server_status[service] = status
                condition.notify_all()
        else:
            self._server_status[service] = status

    async def set(self, service: str,
                  status: _health_pb2.HealthCheckResponse.ServingStatus):
        """Sets the status of a service.

        Args:
          service: string, the name of the service. NOTE, '' must be set.
          status: HealthCheckResponse.status enum value indicating the status of
            the service
        """
        if self._gracefully_shutting_down:
            return
        else:
            await self._set(service, status)

    async def enter_graceful_shutdown(self):
        """Permanently sets the status of all services to NOT_SERVING.

        This should be invoked when the server is entering a graceful shutdown
        period. After this method is invoked, future attempts to set the status
        of a service will be ignored.

        This is an EXPERIMENTAL API.
        """
        if self._gracefully_shutting_down:
            return
        else:
            self._gracefully_shutting_down = True
            for service in self._server_status:
                await self._set(service,
                                _health_pb2.HealthCheckResponse.NOT_SERVING)
