/*
 *
 * Copyright 2019 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "src/cpp/server/external_connection_acceptor_impl.h"

#include <memory>

#include <grpcpp/server_builder_impl.h>
#include <grpcpp/support/channel_arguments.h>

namespace grpc_impl {
namespace {
class InternalAcceptor : public grpc::ExternalConnectionAcceptor {
 public:
  explicit InternalAcceptor(
      std::shared_ptr<ExternalConnectionAcceptorImpl> impl)
      : impl_(std::move(impl)) {}
  void HandleNewConnection(NewConnectionParameters* p) override {
    impl_->HandleNewConnection(p);
  }

 private:
  std::shared_ptr<ExternalConnectionAcceptorImpl> impl_;
};
}  // namespace

ExternalConnectionAcceptorImpl::ExternalConnectionAcceptorImpl(
    const grpc::string& name,
    ServerBuilder::experimental_type::ExternalConnectionType type,
    std::shared_ptr<ServerCredentials> creds)
    : name_(name), creds_(std::move(creds)) {
  GPR_ASSERT(type == ServerBuilder::experimental_type::ExternalConnectionType::
                         CONNECTION_FROM_FD);
}

std::unique_ptr<grpc::ExternalConnectionAcceptor>
ExternalConnectionAcceptorImpl::GetAcceptor() {
  std::lock_guard<std::mutex> lock(mu_);
  GPR_ASSERT(!has_acceptor_);
  has_acceptor_ = true;
  return std::unique_ptr<grpc::ExternalConnectionAcceptor>(
      new InternalAcceptor(shared_from_this()));
}

void ExternalConnectionAcceptorImpl::HandleNewConnection(
    grpc::ExternalConnectionAcceptor::NewConnectionParameters* p) {
  std::lock_guard<std::mutex> lock(mu_);
  if (shutdown_ || !started_) {
    // TODO(yangg) clean up.
    return;
  }
  if (handler_) {
    handler_->Handle(p->fd, p->read_buffer.c_buffer());
  }
}

void ExternalConnectionAcceptorImpl::Shutdown() {
  std::lock_guard<std::mutex> lock(mu_);
  shutdown_ = true;
}

void ExternalConnectionAcceptorImpl::Start() {
  std::lock_guard<std::mutex> lock(mu_);
  GPR_ASSERT(!started_);
  GPR_ASSERT(has_acceptor_);
  GPR_ASSERT(!shutdown_);
  started_ = true;
}

void ExternalConnectionAcceptorImpl::SetToChannelArgs(
    ::grpc::ChannelArguments* args) {
  args->SetPointer(name_.c_str(), &handler_);
}

}  // namespace grpc_impl
