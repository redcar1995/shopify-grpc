# Copyright 2015 gRPC authors.
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

import contextlib
import socket


def get_socket(bind_address='localhost', listen=True, sock_options=(socket.SO_REUSEPORT,)):
    """Opens a listening socket on an arbitrary port.

    Useful for reserving a port for a system-under-test.

    Args:
      bind_address: The host to which to bind.
      sock_options: A sequence of socket options to apply to the socket.

    Returns:
      A tuple containing:
        - the address to which the socket is bound
        - the port to which the socket is bound
        - the socket object itself
    """
    _sock_options = sock_options if sock_options else []
    for address_family in (socket.AF_INET, socket.AF_INET6):
        try:
            sock = socket.socket(address_family, socket.SOCK_STREAM)
            for sock_option in _sock_options:
                sock.setsockopt(socket.SOL_SOCKET, sock_option, 1)
            sock.bind((bind_address, 0))
            if listen:
                sock.listen(1)
            return bind_address, sock.getsockname()[1], sock
        except socket.error:
            continue
    raise RuntimeError("Failed to find to {} with sock_options {}".format(bind_address, sock_options))


@contextlib.contextmanager
def bound_socket(bind_address='localhost', listen=True, sock_options=(socket.SO_REUSEPORT,)):
    # TODO: Docstring.
    # TODO: Just yield address?
    host, port, sock = get_socket(bind_address=bind_address, listen=listen, sock_options=sock_options)
    try:
        yield host, port
    finally:
        sock.close()
