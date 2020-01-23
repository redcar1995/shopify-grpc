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

import grpc
from grpc.experimental import aio
from grpc.experimental.aio._typing import MetadataType, MetadatumType


def seen_metadata(expected: MetadataType, actual: MetadataType):
    return not bool(set(expected) - set(actual))


def seen_metadatum(expected: MetadatumType, actual: MetadataType):
    metadata_dict = dict(actual)
    return metadata_dict.get(expected[0]) == expected[1]


async def block_until_certain_state(channel: aio.Channel, expected_state: grpc.ChannelConnectivity):
    state = channel.get_state()
    while state != expected_state:
        import logging;logging.debug('Get %s want %s', state, expected_state)
        await channel.wait_for_state_change(state)
        state = channel.get_state()
