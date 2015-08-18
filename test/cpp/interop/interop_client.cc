/*
 *
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

#include "test/cpp/interop/interop_client.h"

#include <fstream>
#include <memory>

#include <unistd.h>

#include <grpc/grpc.h>
#include <grpc/support/log.h>
#include <grpc/support/string_util.h>
#include <grpc++/channel_interface.h>
#include <grpc++/client_context.h>
#include <grpc++/credentials.h>
#include <grpc++/status.h>
#include <grpc++/stream.h>

#include "test/cpp/interop/client_helper.h"
#include "test/proto/test.grpc.pb.h"
#include "test/proto/empty.grpc.pb.h"
#include "test/proto/messages.grpc.pb.h"
#include "src/core/transport/stream_op.h"

namespace grpc {
namespace testing {

static const char* kRandomFile = "test/cpp/interop/rnd.dat";

namespace {
// The same value is defined by the Java client.
const std::vector<int> request_stream_sizes = {27182, 8, 1828, 45904};
const std::vector<int> response_stream_sizes = {31415, 9, 2653, 58979};
const int kNumResponseMessages = 2000;
const int kResponseMessageSize = 1030;
const int kReceiveDelayMilliSeconds = 20;
const int kLargeRequestSize = 271828;
const int kLargeResponseSize = 314159;
}  // namespace

InteropClient::InteropClient(std::shared_ptr<ChannelInterface> channel)
    : channel_(channel) {}

void InteropClient::AssertOkOrPrintErrorStatus(const Status& s) {
  if (s.ok()) {
    return;
  }
  gpr_log(GPR_INFO, "Error status code: %d, message: %s", s.error_code(),
          s.error_message().c_str());
  GPR_ASSERT(0);
}

void InteropClient::DoEmpty() {
  gpr_log(GPR_INFO, "Sending an empty rpc...");
  std::unique_ptr<TestService::Stub> stub(TestService::NewStub(channel_));

  Empty request = Empty::default_instance();
  Empty response = Empty::default_instance();
  ClientContext context;

  Status s = stub->EmptyCall(&context, request, &response);
  AssertOkOrPrintErrorStatus(s);

  gpr_log(GPR_INFO, "Empty rpc done.");
}

// Shared code to set large payload, make rpc and check response payload.
void InteropClient::PerformLargeUnary(SimpleRequest* request,
                                      SimpleResponse* response) {
  std::unique_ptr<TestService::Stub> stub(TestService::NewStub(channel_));

  ClientContext context;
  InteropClientContextInspector inspector(context);
  // If the request doesn't already specify the response type, default to
  // COMPRESSABLE.
  request->set_response_size(kLargeResponseSize);
  grpc::string payload(kLargeRequestSize, '\0');
  request->mutable_payload()->set_body(payload.c_str(), kLargeRequestSize);

  Status s = stub->UnaryCall(&context, *request, response);

  // Compression related checks.
  GPR_ASSERT(request->response_compression() ==
             GetInteropCompressionTypeFromCompressionAlgorithm(
                 inspector.GetCallCompressionAlgorithm()));
  if (request->response_compression() == NONE) {
    GPR_ASSERT(!(inspector.GetMessageFlags() & GRPC_WRITE_INTERNAL_COMPRESS));
  } else if (request->response_type() == PayloadType::COMPRESSABLE) {
    // requested compression and compressable response => results should always
    // be compressed.
    GPR_ASSERT(inspector.GetMessageFlags() & GRPC_WRITE_INTERNAL_COMPRESS);
  }

  AssertOkOrPrintErrorStatus(s);

  // Payload related checks.
  if (request->response_type() != PayloadType::RANDOM) {
    GPR_ASSERT(response->payload().type() == request->response_type());
  }
  switch (response->payload().type()) {
    case PayloadType::COMPRESSABLE:
      GPR_ASSERT(response->payload().body() ==
                 grpc::string(kLargeResponseSize, '\0'));
      break;
    case PayloadType::UNCOMPRESSABLE: {
      std::ifstream rnd_file(kRandomFile);
      GPR_ASSERT(rnd_file.good());
      for (int i = 0; i < kLargeResponseSize; i++) {
        GPR_ASSERT(response->payload().body()[i] == (char)rnd_file.get());
      }
    } break;
    default:
      GPR_ASSERT(false);
  }
}

void InteropClient::DoComputeEngineCreds(
    const grpc::string& default_service_account,
    const grpc::string& oauth_scope) {
  gpr_log(GPR_INFO,
          "Sending a large unary rpc with compute engine credentials ...");
  SimpleRequest request;
  SimpleResponse response;
  request.set_fill_username(true);
  request.set_fill_oauth_scope(true);
  request.set_response_type(PayloadType::COMPRESSABLE);
  PerformLargeUnary(&request, &response);
  gpr_log(GPR_INFO, "Got username %s", response.username().c_str());
  gpr_log(GPR_INFO, "Got oauth_scope %s", response.oauth_scope().c_str());
  GPR_ASSERT(!response.username().empty());
  GPR_ASSERT(response.username().c_str() == default_service_account);
  GPR_ASSERT(!response.oauth_scope().empty());
  const char* oauth_scope_str = response.oauth_scope().c_str();
  GPR_ASSERT(oauth_scope.find(oauth_scope_str) != grpc::string::npos);
  gpr_log(GPR_INFO, "Large unary with compute engine creds done.");
}

void InteropClient::DoServiceAccountCreds(const grpc::string& username,
                                          const grpc::string& oauth_scope) {
  gpr_log(GPR_INFO,
          "Sending a large unary rpc with service account credentials ...");
  SimpleRequest request;
  SimpleResponse response;
  request.set_fill_username(true);
  request.set_fill_oauth_scope(true);
  request.set_response_type(PayloadType::COMPRESSABLE);
  PerformLargeUnary(&request, &response);
  GPR_ASSERT(!response.username().empty());
  GPR_ASSERT(!response.oauth_scope().empty());
  GPR_ASSERT(username.find(response.username()) != grpc::string::npos);
  const char* oauth_scope_str = response.oauth_scope().c_str();
  GPR_ASSERT(oauth_scope.find(oauth_scope_str) != grpc::string::npos);
  gpr_log(GPR_INFO, "Large unary with service account creds done.");
}

void InteropClient::DoOauth2AuthToken(const grpc::string& username,
                                      const grpc::string& oauth_scope) {
  gpr_log(GPR_INFO,
          "Sending a unary rpc with raw oauth2 access token credentials ...");
  SimpleRequest request;
  SimpleResponse response;
  request.set_fill_username(true);
  request.set_fill_oauth_scope(true);
  std::unique_ptr<TestService::Stub> stub(TestService::NewStub(channel_));

  ClientContext context;

  Status s = stub->UnaryCall(&context, request, &response);

  AssertOkOrPrintErrorStatus(s);
  GPR_ASSERT(!response.username().empty());
  GPR_ASSERT(!response.oauth_scope().empty());
  GPR_ASSERT(username.find(response.username()) != grpc::string::npos);
  const char* oauth_scope_str = response.oauth_scope().c_str();
  GPR_ASSERT(oauth_scope.find(oauth_scope_str) != grpc::string::npos);
  gpr_log(GPR_INFO, "Unary with oauth2 access token credentials done.");
}

void InteropClient::DoPerRpcCreds(const grpc::string& username,
                                  const grpc::string& oauth_scope) {
  gpr_log(GPR_INFO,
          "Sending a unary rpc with per-rpc raw oauth2 access token ...");
  SimpleRequest request;
  SimpleResponse response;
  request.set_fill_username(true);
  request.set_fill_oauth_scope(true);
  std::unique_ptr<TestService::Stub> stub(TestService::NewStub(channel_));

  ClientContext context;
  grpc::string access_token = GetOauth2AccessToken();
  std::shared_ptr<Credentials> creds = AccessTokenCredentials(access_token);
  context.set_credentials(creds);

  Status s = stub->UnaryCall(&context, request, &response);

  AssertOkOrPrintErrorStatus(s);
  GPR_ASSERT(!response.username().empty());
  GPR_ASSERT(!response.oauth_scope().empty());
  GPR_ASSERT(username.find(response.username()) != grpc::string::npos);
  const char* oauth_scope_str = response.oauth_scope().c_str();
  GPR_ASSERT(oauth_scope.find(oauth_scope_str) != grpc::string::npos);
  gpr_log(GPR_INFO, "Unary with per-rpc oauth2 access token done.");
}

void InteropClient::DoJwtTokenCreds(const grpc::string& username) {
  gpr_log(GPR_INFO, "Sending a large unary rpc with JWT token credentials ...");
  SimpleRequest request;
  SimpleResponse response;
  request.set_fill_username(true);
  request.set_response_type(PayloadType::COMPRESSABLE);
  PerformLargeUnary(&request, &response);
  GPR_ASSERT(!response.username().empty());
  GPR_ASSERT(username.find(response.username()) != grpc::string::npos);
  gpr_log(GPR_INFO, "Large unary with JWT token creds done.");
}

void InteropClient::DoLargeUnary() {
  gpr_log(GPR_INFO, "Sending a large unary rpc...");
  SimpleRequest request;
  SimpleResponse response;
  request.set_response_type(PayloadType::COMPRESSABLE);
  PerformLargeUnary(&request, &response);
  gpr_log(GPR_INFO, "Large unary done.");
}

void InteropClient::DoLargeCompressedUnary() {
  const CompressionType compression_types[] = {NONE, GZIP, DEFLATE};
  const PayloadType payload_types[] = {COMPRESSABLE, UNCOMPRESSABLE, RANDOM};
  for (const auto payload_type : payload_types) {
    for (const auto compression_type : compression_types) {
      char* log_suffix;
      gpr_asprintf(&log_suffix, "(compression=%s; payload=%s)",
                   CompressionType_Name(compression_type).c_str(),
                   PayloadType_Name(payload_type).c_str());

      gpr_log(GPR_INFO, "Sending a large compressed unary rpc %s.", log_suffix);
      SimpleRequest request;
      SimpleResponse response;
      request.set_response_type(payload_type);
      request.set_response_compression(compression_type);
      PerformLargeUnary(&request, &response);
      gpr_log(GPR_INFO, "Large compressed unary done %s.", log_suffix);
      gpr_free(log_suffix);
    }
  }
}

void InteropClient::DoRequestStreaming() {
  gpr_log(GPR_INFO, "Sending request steaming rpc ...");
  std::unique_ptr<TestService::Stub> stub(TestService::NewStub(channel_));

  ClientContext context;
  StreamingInputCallRequest request;
  StreamingInputCallResponse response;

  std::unique_ptr<ClientWriter<StreamingInputCallRequest>> stream(
      stub->StreamingInputCall(&context, &response));

  int aggregated_payload_size = 0;
  for (unsigned int i = 0; i < request_stream_sizes.size(); ++i) {
    Payload* payload = request.mutable_payload();
    payload->set_body(grpc::string(request_stream_sizes[i], '\0'));
    GPR_ASSERT(stream->Write(request));
    aggregated_payload_size += request_stream_sizes[i];
  }
  stream->WritesDone();
  Status s = stream->Finish();

  GPR_ASSERT(response.aggregated_payload_size() == aggregated_payload_size);
  AssertOkOrPrintErrorStatus(s);
  gpr_log(GPR_INFO, "Request streaming done.");
}

void InteropClient::DoResponseStreaming() {
  gpr_log(GPR_INFO, "Receiving response steaming rpc ...");
  std::unique_ptr<TestService::Stub> stub(TestService::NewStub(channel_));

  ClientContext context;
  StreamingOutputCallRequest request;
  for (unsigned int i = 0; i < response_stream_sizes.size(); ++i) {
    ResponseParameters* response_parameter = request.add_response_parameters();
    response_parameter->set_size(response_stream_sizes[i]);
  }
  StreamingOutputCallResponse response;
  std::unique_ptr<ClientReader<StreamingOutputCallResponse>> stream(
      stub->StreamingOutputCall(&context, request));

  unsigned int i = 0;
  while (stream->Read(&response)) {
    GPR_ASSERT(response.payload().body() ==
               grpc::string(response_stream_sizes[i], '\0'));
    ++i;
  }
  GPR_ASSERT(response_stream_sizes.size() == i);
  Status s = stream->Finish();
  AssertOkOrPrintErrorStatus(s);
  gpr_log(GPR_INFO, "Response streaming done.");
}

void InteropClient::DoResponseCompressedStreaming() {
  std::unique_ptr<TestService::Stub> stub(TestService::NewStub(channel_));

  const CompressionType compression_types[] = {NONE, GZIP, DEFLATE};
  const PayloadType payload_types[] = {COMPRESSABLE, UNCOMPRESSABLE, RANDOM};
  for (const auto payload_type : payload_types) {
    for (const auto compression_type : compression_types) {
      ClientContext context;
      InteropClientContextInspector inspector(context);
      StreamingOutputCallRequest request;

      char* log_suffix;
      gpr_asprintf(&log_suffix, "(compression=%s; payload=%s)",
                   CompressionType_Name(compression_type).c_str(),
                   PayloadType_Name(payload_type).c_str());

      gpr_log(GPR_INFO, "Receiving response steaming rpc %s.", log_suffix);

      request.set_response_type(payload_type);
      request.set_response_compression(compression_type);

      for (unsigned int i = 0; i < response_stream_sizes.size(); ++i) {
        ResponseParameters* response_parameter =
            request.add_response_parameters();
        response_parameter->set_size(response_stream_sizes[i]);
      }
      StreamingOutputCallResponse response;

      std::unique_ptr<ClientReader<StreamingOutputCallResponse>> stream(
          stub->StreamingOutputCall(&context, request));

      unsigned int i = 0;
      while (stream->Read(&response)) {
        GPR_ASSERT(response.payload().body() ==
                   grpc::string(response_stream_sizes[i], '\0'));

        // Compression related checks.
        GPR_ASSERT(request.response_compression() ==
                   GetInteropCompressionTypeFromCompressionAlgorithm(
                       inspector.GetCallCompressionAlgorithm()));
        if (request.response_compression() == NONE) {
          GPR_ASSERT(
              !(inspector.GetMessageFlags() & GRPC_WRITE_INTERNAL_COMPRESS));
        } else if (request.response_type() == PayloadType::COMPRESSABLE) {
          // requested compression and compressable response => results should
          // always be compressed.
          GPR_ASSERT(inspector.GetMessageFlags() &
                     GRPC_WRITE_INTERNAL_COMPRESS);
        }

        ++i;
      }

      GPR_ASSERT(response_stream_sizes.size() == i);
      Status s = stream->Finish();

      AssertOkOrPrintErrorStatus(s);
      gpr_log(GPR_INFO, "Response streaming done %s.", log_suffix);
      gpr_free(log_suffix);
    }
  }
}

void InteropClient::DoResponseStreamingWithSlowConsumer() {
  gpr_log(GPR_INFO, "Receiving response steaming rpc with slow consumer ...");
  std::unique_ptr<TestService::Stub> stub(TestService::NewStub(channel_));

  ClientContext context;
  StreamingOutputCallRequest request;

  for (int i = 0; i < kNumResponseMessages; ++i) {
    ResponseParameters* response_parameter = request.add_response_parameters();
    response_parameter->set_size(kResponseMessageSize);
  }
  StreamingOutputCallResponse response;
  std::unique_ptr<ClientReader<StreamingOutputCallResponse>> stream(
      stub->StreamingOutputCall(&context, request));

  int i = 0;
  while (stream->Read(&response)) {
    GPR_ASSERT(response.payload().body() ==
               grpc::string(kResponseMessageSize, '\0'));
    gpr_log(GPR_INFO, "received message %d", i);
    usleep(kReceiveDelayMilliSeconds * 1000);
    ++i;
  }
  GPR_ASSERT(kNumResponseMessages == i);
  Status s = stream->Finish();

  AssertOkOrPrintErrorStatus(s);
  gpr_log(GPR_INFO, "Response streaming done.");
}

void InteropClient::DoHalfDuplex() {
  gpr_log(GPR_INFO, "Sending half-duplex streaming rpc ...");
  std::unique_ptr<TestService::Stub> stub(TestService::NewStub(channel_));

  ClientContext context;
  std::unique_ptr<ClientReaderWriter<StreamingOutputCallRequest,
                                     StreamingOutputCallResponse>>
      stream(stub->HalfDuplexCall(&context));

  StreamingOutputCallRequest request;
  ResponseParameters* response_parameter = request.add_response_parameters();
  for (unsigned int i = 0; i < response_stream_sizes.size(); ++i) {
    response_parameter->set_size(response_stream_sizes[i]);
    GPR_ASSERT(stream->Write(request));
  }
  stream->WritesDone();

  unsigned int i = 0;
  StreamingOutputCallResponse response;
  while (stream->Read(&response)) {
    GPR_ASSERT(response.payload().body() ==
               grpc::string(response_stream_sizes[i], '\0'));
    ++i;
  }
  GPR_ASSERT(response_stream_sizes.size() == i);
  Status s = stream->Finish();
  AssertOkOrPrintErrorStatus(s);
  gpr_log(GPR_INFO, "Half-duplex streaming rpc done.");
}

void InteropClient::DoPingPong() {
  gpr_log(GPR_INFO, "Sending Ping Pong streaming rpc ...");
  std::unique_ptr<TestService::Stub> stub(TestService::NewStub(channel_));

  ClientContext context;
  std::unique_ptr<ClientReaderWriter<StreamingOutputCallRequest,
                                     StreamingOutputCallResponse>>
      stream(stub->FullDuplexCall(&context));

  StreamingOutputCallRequest request;
  request.set_response_type(PayloadType::COMPRESSABLE);
  ResponseParameters* response_parameter = request.add_response_parameters();
  Payload* payload = request.mutable_payload();
  StreamingOutputCallResponse response;
  for (unsigned int i = 0; i < request_stream_sizes.size(); ++i) {
    response_parameter->set_size(response_stream_sizes[i]);
    payload->set_body(grpc::string(request_stream_sizes[i], '\0'));
    GPR_ASSERT(stream->Write(request));
    GPR_ASSERT(stream->Read(&response));
    GPR_ASSERT(response.payload().body() ==
               grpc::string(response_stream_sizes[i], '\0'));
  }

  stream->WritesDone();
  GPR_ASSERT(!stream->Read(&response));
  Status s = stream->Finish();
  AssertOkOrPrintErrorStatus(s);
  gpr_log(GPR_INFO, "Ping pong streaming done.");
}

void InteropClient::DoCancelAfterBegin() {
  gpr_log(GPR_INFO, "Sending request steaming rpc ...");
  std::unique_ptr<TestService::Stub> stub(TestService::NewStub(channel_));

  ClientContext context;
  StreamingInputCallRequest request;
  StreamingInputCallResponse response;

  std::unique_ptr<ClientWriter<StreamingInputCallRequest>> stream(
      stub->StreamingInputCall(&context, &response));

  gpr_log(GPR_INFO, "Trying to cancel...");
  context.TryCancel();
  Status s = stream->Finish();
  GPR_ASSERT(s.error_code() == StatusCode::CANCELLED);
  gpr_log(GPR_INFO, "Canceling streaming done.");
}

void InteropClient::DoCancelAfterFirstResponse() {
  gpr_log(GPR_INFO, "Sending Ping Pong streaming rpc ...");
  std::unique_ptr<TestService::Stub> stub(TestService::NewStub(channel_));

  ClientContext context;
  std::unique_ptr<ClientReaderWriter<StreamingOutputCallRequest,
                                     StreamingOutputCallResponse>>
      stream(stub->FullDuplexCall(&context));

  StreamingOutputCallRequest request;
  request.set_response_type(PayloadType::COMPRESSABLE);
  ResponseParameters* response_parameter = request.add_response_parameters();
  response_parameter->set_size(31415);
  request.mutable_payload()->set_body(grpc::string(27182, '\0'));
  StreamingOutputCallResponse response;
  GPR_ASSERT(stream->Write(request));
  GPR_ASSERT(stream->Read(&response));
  GPR_ASSERT(response.payload().body() == grpc::string(31415, '\0'));
  gpr_log(GPR_INFO, "Trying to cancel...");
  context.TryCancel();

  Status s = stream->Finish();
  gpr_log(GPR_INFO, "Canceling pingpong streaming done.");
}

void InteropClient::DoTimeoutOnSleepingServer() {
  gpr_log(GPR_INFO, "Sending Ping Pong streaming rpc with a short deadline...");
  std::unique_ptr<TestService::Stub> stub(TestService::NewStub(channel_));

  ClientContext context;
  std::chrono::system_clock::time_point deadline =
      std::chrono::system_clock::now() + std::chrono::milliseconds(1);
  context.set_deadline(deadline);
  std::unique_ptr<ClientReaderWriter<StreamingOutputCallRequest,
                                     StreamingOutputCallResponse>>
      stream(stub->FullDuplexCall(&context));

  StreamingOutputCallRequest request;
  request.mutable_payload()->set_body(grpc::string(27182, '\0'));
  stream->Write(request);

  Status s = stream->Finish();
  GPR_ASSERT(s.error_code() == StatusCode::DEADLINE_EXCEEDED);
  gpr_log(GPR_INFO, "Pingpong streaming timeout done.");
}

void InteropClient::DoStatusWithMessage() {
  gpr_log(GPR_INFO, "Sending RPC with a request for status code 2 and message");
  std::unique_ptr<TestService::Stub> stub(TestService::NewStub(channel_));

  ClientContext context;
  SimpleRequest request;
  SimpleResponse response;
  EchoStatus* requested_status = request.mutable_response_status();
  requested_status->set_code(grpc::StatusCode::UNKNOWN);
  grpc::string test_msg = "This is a test message";
  requested_status->set_message(test_msg);

  Status s = stub->UnaryCall(&context, request, &response);

  GPR_ASSERT(s.error_code() == grpc::StatusCode::UNKNOWN);
  GPR_ASSERT(s.error_message() == test_msg);
  gpr_log(GPR_INFO, "Done testing Status and Message");
}

}  // namespace testing
}  // namespace grpc
