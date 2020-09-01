//
//
// Copyright 2020 gRPC authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//

#ifndef GRPC_CORE_LIB_SECURITY_AUTHORIZATION_EVALUATE_ARGS_H
#define GRPC_CORE_LIB_SECURITY_AUTHORIZATION_EVALUATE_ARGS_H

#include <grpc/support/port_platform.h>

#include <map>

#include "src/core/lib/iomgr/endpoint.h"
#include "src/core/lib/security/context/security_context.h"
#include "src/core/lib/transport/metadata_batch.h"

namespace grpc_core {

class EvaluateArgs {
 public:
  EvaluateArgs(grpc_metadata_batch* metadata, grpc_auth_context* auth_context,
               grpc_endpoint* endpoint)
      : metadata_(metadata), auth_context_(auth_context), endpoint_(endpoint) {}

  absl::string_view GetPath() const;
  absl::string_view GetHost() const;
  absl::string_view GetMethod() const;
  std::multimap<absl::string_view, absl::string_view> GetHeaders() const;
  absl::string_view GetLocalAddress() const;
  int GetLocalPort() const;
  absl::string_view GetPeerAddress() const;
  int GetPeerPort() const;
  absl::string_view GetSpiffeId() const;
  absl::string_view GetCertServerName() const;

  // TODO: Add a getter function for source.principal

 private:
  grpc_metadata_batch* metadata_;
  grpc_auth_context* auth_context_;
  grpc_endpoint* endpoint_;
};

}  // namespace grpc_core

#endif  // GRPC_CORE_LIB_SECURITY_AUTHORIZATION_EVALUATE_ARGS_H
