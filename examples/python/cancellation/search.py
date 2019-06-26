# Copyright the 2019 gRPC authors.
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
"""A search algorithm over the space of all bytestrings."""

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import base64
import hashlib
import logging
import struct

from examples.python.cancellation import hash_name_pb2

_LOGGER = logging.getLogger(__name__)
_BYTE_MAX = 255


def _get_hamming_distance(a, b):
    """Calculates hamming distance between strings of equal length."""
    distance = 0
    for char_a, char_b in zip(a, b):
        if char_a.lower() != char_b.lower():
            distance += 1
    return distance


def _get_substring_hamming_distance(candidate, target):
    """Calculates the minimum hamming distance between between the target
        and any substring of the candidate.

    Args:
      candidate: The string whose substrings will be tested.
      target: The target string.

    Returns:
      The minimum Hamming distance between candidate and target.
    """
    min_distance = None
    for i in range(len(candidate) - len(target) + 1):
        distance = _get_hamming_distance(candidate[i:i + len(target)], target)
        if min_distance is None or distance < min_distance:
            min_distance = distance
    return min_distance


def _get_hash(secret):
    hasher = hashlib.sha1()
    hasher.update(secret)
    return base64.b64encode(hasher.digest()).decode('ascii')


class ResourceLimitExceededError(Exception):
    """Signifies the request has exceeded configured limits."""


def _bytestrings_of_length(length):
    """Generates a stream containing all bytestrings of a given length.

    Args:
      length: A positive integer length.

    Yields:
      All bytestrings of length `length`.
    """
    digits = [0] * length
    while True:
        yield b''.join(struct.pack('B', i) for i in digits)
        digits[-1] += 1
        i = length - 1
        while digits[i] == _BYTE_MAX + 1:
            digits[i] = 0
            i -= 1
            if i == -1:
                # Terminate the generator since we've run out of strings of
                # `length` bytes.
                raise StopIteration()  # pylint: disable=stop-iteration-return
            else:
                digits[i] += 1


def _all_bytestrings():
    """Generates a stream containing all possible bytestrings.

    This generator does not terminate.

    Yields:
      All bytestrings in ascending order of length.
    """
    length = 1
    while True:
        for bytestring in _bytestrings_of_length(length):
            yield bytestring
        length += 1


def search(target,
           ideal_distance,
           stop_event,
           maximum_hashes,
           interesting_hamming_distance=None):
    """Find candidate strings.

    Search through the space of all bytestrings, in order of increasing length,
    indefinitely, until a hash with a Hamming distance of `maximum_distance` or
    less has been found.

    Args:
      target: The search string.
      ideal_distance: The desired Hamming distance.
      stop_event: An event indicating whether the RPC should terminate.
      maximum_hashes: The maximum number of hashes to check before stopping.
      interesting_hamming_distance: If specified, strings with a Hamming
        distance from the target below this value will be yielded.

    Yields:
      Instances  of HashNameResponse. The final entry in the stream will be of
        `maximum_distance` Hamming distance or less from the target string,
        while all others will be of less than `interesting_hamming_distance`.

    Raises:
      ResourceLimitExceededError: If the computation exceeds `maximum_hashes`
        iterations.
    """
    hashes_computed = 0
    for secret in _all_bytestrings():
        if stop_event.is_set():
            raise StopIteration()  # pylint: disable=stop-iteration-return
        candidate_hash = _get_hash(secret)
        distance = _get_substring_hamming_distance(candidate_hash, target)
        if interesting_hamming_distance is not None and distance <= interesting_hamming_distance:
            # Surface interesting candidates, but don't stop.
            yield hash_name_pb2.HashNameResponse(
                secret=base64.b64encode(secret),
                hashed_name=candidate_hash,
                hamming_distance=distance)
        elif distance <= ideal_distance:
            # Yield ideal candidate and end the stream.
            yield hash_name_pb2.HashNameResponse(
                secret=base64.b64encode(secret),
                hashed_name=candidate_hash,
                hamming_distance=distance)
            raise StopIteration()  # pylint: disable=stop-iteration-return
        hashes_computed += 1
        if hashes_computed == maximum_hashes:
            raise ResourceLimitExceededError()
