/*

 * Copyright 2015, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
  A command line tool to talk to a grpc server.
  Run `grpc_cli help` command to see its usage information.

  Example of talking to grpc interop server:
  grpc_cli call localhost:50051 UnaryCall "response_size:10" \
      --protofiles=src/proto/grpc/testing/test.proto --enable_ssl=false

  Options:
    1. --protofiles, use this flag to provide proto files if the server does
       does not have the reflection service.
    2. --proto_path, if your proto file is not under current working directory,
       use this flag to provide a search root. It should work similar to the
       counterpart in protoc. This option is valid only when protofiles is
       provided.
    3. --metadata specifies metadata to be sent to the server, such as:
       --metadata="MyHeaderKey1:Value1:MyHeaderKey2:Value2"
    4. --enable_ssl, whether to use tls.
    5. --use_auth, if set to true, attach a GoogleDefaultCredentials to the call
    6. --infile, input filename (defaults to stdin)
    7. --outfile, output filename (defaults to stdout)
    8. --binary_input, use the serialized request as input. The serialized
       request can be generated by calling something like:
       protoc --proto_path=src/proto/grpc/testing/ \
         --encode=grpc.testing.SimpleRequest \
         src/proto/grpc/testing/messages.proto \
         < input.txt > input.bin
       If this is used and no proto file is provided in the argument list, the
       method string has to be exact in the form of /package.service/method.
    9. --binary_output, use binary format response as output, it can
       be later decoded using protoc:
       protoc --proto_path=src/proto/grpc/testing/ \
       --decode=grpc.testing.SimpleResponse \
       src/proto/grpc/testing/messages.proto \
       < output.bin > output.txt
*/

#include <fstream>
#include <functional>
#include <iostream>

#include <gflags/gflags.h>
#include <grpc++/support/config.h>
#include "test/cpp/util/cli_credentials.h"
#include "test/cpp/util/grpc_tool.h"
#include "test/cpp/util/test_config.h"

DEFINE_string(outfile, "", "Output file (default is stdout)");

static bool SimplePrint(const grpc::string& outfile,
                        const grpc::string& output) {
  if (outfile.empty()) {
    std::cout << output << std::endl;
  } else {
    std::ofstream output_file(outfile, std::ios::app | std::ios::binary);
    output_file << output << std::endl;
    output_file.close();
  }
  return true;
}

int main(int argc, char** argv) {
  grpc::testing::InitTest(&argc, &argv, true);

  return grpc::testing::GrpcToolMainLib(
      argc, (const char**)argv, grpc::testing::CliCredentials(),
      std::bind(SimplePrint, FLAGS_outfile, std::placeholders::_1));
}
