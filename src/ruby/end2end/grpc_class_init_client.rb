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

# For GRPC::Core classes, which use the grpc c-core, object init
# is interesting because it's related to overall library init.

require_relative './end2end_common'

def main
  grpc_class = ''
  OptionParser.new do |opts|
    opts.on('--grpc_class=P', String) do |p|
      grpc_class = p
    end
  end.parse!

  case grpc_class
  when 'channel'
    thd = Thread.new do
      GRPC::Core::Channel.new('dummy_host', nil, :this_channel_is_insecure)
    end
    GRPC::Core::Channel.new('dummy_host', nil, :this_channel_is_insecure)
    thd.join
  when 'server'
    thd = Thread.new do
      GRPC::Core::Server.new({})
    end
    GRPC::Core::Server.new({})
    thd.join
  when 'channel_credentials'
    thd = Thread.new do
      GRPC::Core::ChannelCredentials.new
    end
    GRPC::Core::ChannelCredentials.new
    thd.join
  when 'call_credentials'
    thd = Thread.new do
      GRPC::Core::CallCredentials.new(proc { |noop| noop })
    end
    GRPC::Core::CallCredentials.new(proc { |noop| noop })
    thd.join
  when 'compression_options'
    thd = Thread.new do
      GRPC::Core::CompressionOptions.new
    end
    GRPC::Core::CompressionOptions.new
    thd.join
  else
    fail "bad --grpc_class=#{grpc_class} param"
  end
end

main
