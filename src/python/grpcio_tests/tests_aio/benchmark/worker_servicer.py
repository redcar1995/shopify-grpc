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

import asyncio
import logging
import os
import multiprocessing
import sys
import time
from typing import Tuple
import collections

import grpc
from grpc.experimental import aio

from src.proto.grpc.testing import (benchmark_service_pb2_grpc, control_pb2,
                                    stats_pb2, worker_service_pb2_grpc)
from tests.qps import histogram
from tests.unit import resources
from tests_aio.benchmark import benchmark_client, benchmark_servicer

_NUM_CORES = multiprocessing.cpu_count()
_NUM_CORE_PYTHON_CAN_USE = 1

_LOGGER = logging.getLogger(__name__)


def _get_server_status(start_time: float, end_time: float,
                       port: int) -> control_pb2.ServerStatus:
    end_time = time.time()
    elapsed_time = end_time - start_time
    stats = stats_pb2.ServerStats(time_elapsed=elapsed_time,
                                  time_user=elapsed_time,
                                  time_system=elapsed_time)
    return control_pb2.ServerStatus(stats=stats,
                                    port=port,
                                    cores=_NUM_CORE_PYTHON_CAN_USE)


def _create_server(config: control_pb2.ServerConfig) -> Tuple[aio.Server, int]:
    if config.async_server_threads != 1:
        _LOGGER.warning('config.async_server_threads [%d] != 1',
                        config.async_server_threads)

    server = aio.server()
    if config.server_type == control_pb2.ASYNC_SERVER:
        servicer = benchmark_servicer.BenchmarkServicer()
        benchmark_service_pb2_grpc.add_BenchmarkServiceServicer_to_server(
            servicer, server)
    elif config.server_type == control_pb2.ASYNC_GENERIC_SERVER:
        resp_size = config.payload_config.bytebuf_params.resp_size
        servicer = benchmark_servicer.GenericBenchmarkServicer(resp_size)
        method_implementations = {
            'StreamingCall':
                grpc.stream_stream_rpc_method_handler(servicer.StreamingCall),
            'UnaryCall':
                grpc.unary_unary_rpc_method_handler(servicer.UnaryCall),
        }
        handler = grpc.method_handlers_generic_handler(
            'grpc.testing.BenchmarkService', method_implementations)
        server.add_generic_rpc_handlers((handler,))
    else:
        raise NotImplementedError('Unsupported server type {}'.format(
            config.server_type))

    if config.HasField('security_params'):  # Use SSL
        server_creds = grpc.ssl_server_credentials(
            ((resources.private_key(), resources.certificate_chain()),))
        port = server.add_secure_port('[::]:{}'.format(config.port),
                                      server_creds)
    else:
        port = server.add_insecure_port('[::]:{}'.format(config.port))

    return server, port


def _get_client_status(start_time: float, end_time: float,
                       qps_data: histogram.Histogram
                      ) -> control_pb2.ClientStatus:
    latencies = qps_data.get_data()
    end_time = time.time()
    elapsed_time = end_time - start_time
    stats = stats_pb2.ClientStats(latencies=latencies,
                                  time_elapsed=elapsed_time,
                                  time_user=elapsed_time,
                                  time_system=elapsed_time)
    return control_pb2.ClientStatus(stats=stats)


def _create_client(server: str, config: control_pb2.ClientConfig,
                   qps_data: histogram.Histogram
                  ) -> benchmark_client.BenchmarkClient:
    if config.load_params.WhichOneof('load') != 'closed_loop':
        raise NotImplementedError(
            f'Unsupported load parameter {config.load_params}')

    if config.client_type == control_pb2.ASYNC_CLIENT:
        if config.rpc_type == control_pb2.UNARY:
            client_type = benchmark_client.UnaryAsyncBenchmarkClient
        elif config.rpc_type == control_pb2.STREAMING:
            client_type = benchmark_client.StreamingAsyncBenchmarkClient
        else:
            raise NotImplementedError(
                f'Unsupported rpc_type [{config.rpc_type}]')
    else:
        raise NotImplementedError(
            f'Unsupported client type {config.client_type}')

    return client_type(server, config, qps_data)


WORKER_ENTRY_FILE = os.path.split(os.path.abspath(__file__))[0] + 'worker.py'
SubWorker = collections.namedtuple('SubWorker', ['process', 'port', 'channel', 'stub'])


async def _create_sub_worker(port: int) -> SubWorker:
    process = asyncio.create_subprocess_exec(
        sys.executable,
        WORKER_ENTRY_FILE,
        '--driver_port', port
    )
    channel = aio.insecure_channel(f'localhost:{port}')
    stub = worker_service_pb2_grpc.WorkerServiceStub(channel)
    return SubWorker(
        process=process,
        port=port,
        channel=channel,
        stub=stub,
    )


class WorkerServicer(worker_service_pb2_grpc.WorkerServiceServicer):
    """Python Worker Server implementation."""

    def __init__(self):
        self._loop = asyncio.get_event_loop()
        self._quit_event = asyncio.Event()

    async def RunServer(self, request_iterator, context):
        config = (await context.read()).setup
        _LOGGER.info('Received ServerConfig: %s', config)

        server, port = _create_server(config)
        await server.start()
        _LOGGER.info('Server started at port [%d]', port)

        start_time = time.time()
        yield _get_server_status(start_time, start_time, port)

        async for request in request_iterator:
            end_time = time.time()
            status = _get_server_status(start_time, end_time, port)
            if request.mark.reset:
                start_time = end_time
            yield status
        await server.stop(None)

    async def _run_single_client(self, config, request_iterator, context):
        running_tasks = []
        qps_data = histogram.Histogram(config.histogram_params.resolution,
                                       config.histogram_params.max_possible)
        start_time = time.time()

        # Create a client for each channel as asyncio.Task
        for i in range(config.client_channels):
            server = config.server_targets[i % len(config.server_targets)]
            client = _create_client(server, config, qps_data)
            _LOGGER.info('Client created against server [%s]', server)
            running_tasks.append(self._loop.create_task(client.run()))

        end_time = time.time()
        await context.write(_get_client_status(start_time, end_time, qps_data))

        # Respond to stat requests
        async for request in request_iterator:
            end_time = time.time()
            status = _get_client_status(start_time, end_time, qps_data)
            if request.mark.reset:
                qps_data.reset()
                start_time = time.time()
            await context.write(status)

        # Cleanup the clients
        for task in running_tasks:
            task.cancel()

    async def RunClient(self, request_iterator, context):
        config_request = await context.read()
        config = config_request.setup
        _LOGGER.info('Received ClientConfig: %s', config)

        if config.async_server_threads <= 0:
            raise ValueError('async_server_threads can\'t be [%d]' % config.async_server_threads)
        elif config.async_server_threads == 1:
            await self._run_single_client(config, request_iterator, context)
        else:
            sub_workers = []
            for i in range(config.async_server_threads):
                port = 40000+i
                _LOGGER.info('Creating sub worker at port [%d]...', port)
                sub_workers.append(await _create_sub_worker(port))

            calls = [worker.stub.RunClient() for worker in sub_workers]

            for call in calls:
                await call.write(config_request)

            start_time = time.time()
            result = histogram.Histogram(config.histogram_params.resolution,
                                         config.histogram_params.max_possible)
            end_time = time.time()
            yield _get_client_status(start_time, end_time, result)

            async for request in request_iterator:
                end_time = time.time()

                for call in calls:
                    await call.write(request)
                    sub_status = await call.read()
                    result.merge(sub_status.latencies)

                status = _get_client_status(start_time, end_time, result)
                if request.mark.reset:
                    result.reset()
                    start_time = time.time()
                yield status

            for call in calls:
                await call.QuitWorker()

            for worker in sub_workers:
                await worker.channel.close()
                _LOGGER.info('Waiting for sub worker [%s] to quit...', worker)
                await worker.process.wait()
                _LOGGER.info('Sub worker [%s] quit', worker)

    async def CoreCount(self, unused_request, unused_context):
        return control_pb2.CoreResponse(cores=_NUM_CORES)

    async def QuitWorker(self, unused_request, unused_context):
        _LOGGER.info('QuitWorker command received.')
        self._quit_event.set()
        return control_pb2.Void()

    async def wait_for_quit(self):
        await self._quit_event.wait()
