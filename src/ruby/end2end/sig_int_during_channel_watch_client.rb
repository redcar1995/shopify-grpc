#!/usr/bin/env ruby

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

require_relative './end2end_common'

# Start polling the channel state in both the main thread
# and a child thread. Try to get the driver to send process-ending
# interrupt while both a child thread and the main thread are in the
# middle of a blocking connectivity_state call.
def main
  server_port = ''
  OptionParser.new do |opts|
    opts.on('--client_control_port=P', String) do
      STDERR.puts 'client_control_port not used'
    end
    opts.on('--server_port=P', String) do |p|
      server_port = p
    end
  end.parse!

  trap('SIGINT') { exit 0 }

  thd = Thread.new do
    child_thread_channel = GRPC::Core::Channel.new("localhost:#{server_port}",
                                                   {},
                                                   :this_channel_is_insecure)
    loop do
      state = child_thread_channel.connectivity_state(false)
      child_thread_channel.watch_connectivity_state(state, Time.now + 360)
    end
  end

  main_channel = GRPC::Core::Channel.new("localhost:#{server_port}",
                                         {},
                                         :this_channel_is_insecure)
  loop do
    state = main_channel.connectivity_state(false)
    main_channel.watch_connectivity_state(state, Time.now + 360)
  end

  thd.join
end

main
