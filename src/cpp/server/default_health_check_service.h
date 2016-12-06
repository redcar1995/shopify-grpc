/*
 *
 * Copyright 2016, Google Inc.
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

#ifndef GRPC_INTERNAL_CPP_SERVER_DEFAULT_HEALTH_CHECK_SERVICE_H
#define GRPC_INTERNAL_CPP_SERVER_DEFAULT_HEALTH_CHECK_SERVICE_H

#include <mutex>

#include <grpc++/health_check_service_interface.h>
#include <grpc++/impl/codegen/service_types.h>

namespace grpc {

class SyncHealthCheckServiceImpl : public Service {
 public:
  explicit SyncHealthCheckServiceImpl(DefaultHealthCheckService* service);
  Status Check(ServerContext* context, const ByteBuffer* request,
               ByteBuffer* response);

 private:
  const DefaultHealthCheckService* service_;
};

// Default implementation of HealthCheckServiceInterface. Server will create and
// own it.
class DefaultHealthCheckService : public HealthCheckServiceInterface {
 public:
  DefaultHealthCheckService();
  void SetServingStatus(const grpc::string& service_name, bool serving) final;
  void SetServingStatus(bool serving) final;
  enum ServingStatus { NOT_FOUND, SERVING, NOT_SERVING };
  ServingStatus GetServingStatus(const grpc::string& service_name) const;
  SyncHealthCheckServiceImpl* GetSyncHealthCheckService() const {
    return sync_service_.get();
  }

 private:
  std::mutex mu_;
  std::map<grpc::string, bool> services_map_;
  std::unique_ptr<SyncHealthCheckServiceImpl> sync_service_;
};

}  // namespace grpc

#endif  // GRPC_INTERNAL_CPP_SERVER_DEFAULT_HEALTH_CHECK_SERVICE_H
