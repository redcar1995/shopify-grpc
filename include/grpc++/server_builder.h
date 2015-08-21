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

#ifndef GRPCXX_SERVER_BUILDER_H
#define GRPCXX_SERVER_BUILDER_H

#include <memory>
#include <vector>

#include <grpc++/config.h>

namespace grpc {

class AsyncGenericService;
class AsynchronousService;
class CompletionQueue;
class RpcService;
class Server;
class ServerCompletionQueue;
class ServerCredentials;
class SynchronousService;
class ThreadPoolInterface;

class ServerBuilder {
 public:
  ServerBuilder();

  // Register a service. This call does not take ownership of the service.
  // The service must exist for the lifetime of the Server instance returned by
  // BuildAndStart().
  // Matches requests with any :authority
  void RegisterService(SynchronousService* service);

  // Register an asynchronous service.
  // This call does not take ownership of the service or completion queue.
  // The service and completion queuemust exist for the lifetime of the Server
  // instance returned by BuildAndStart().
  // Matches requests with any :authority
  void RegisterAsyncService(AsynchronousService* service);

  // Register a generic service.
  // Matches requests with any :authority
  void RegisterAsyncGenericService(AsyncGenericService* service);

  // Register a service. This call does not take ownership of the service.
  // The service must exist for the lifetime of the Server instance returned by
  // BuildAndStart().
  // Only matches requests with :authority \a host
  void RegisterService(const grpc::string& host, SynchronousService* service);

  // Register an asynchronous service.
  // This call does not take ownership of the service or completion queue.
  // The service and completion queuemust exist for the lifetime of the Server
  // instance returned by BuildAndStart().
  // Only matches requests with :authority \a host
  void RegisterAsyncService(const grpc::string& host,
                            AsynchronousService* service);

  // Set max message size in bytes.
  void SetMaxMessageSize(int max_message_size) {
    max_message_size_ = max_message_size;
  }

  // Add a listening port. Can be called multiple times.
  void AddListeningPort(const grpc::string& addr,
                        std::shared_ptr<ServerCredentials> creds,
                        int* selected_port = nullptr);

  // Set the thread pool used for running appliation rpc handlers.
  // Does not take ownership.
  void SetThreadPool(ThreadPoolInterface* thread_pool);

  // Add a completion queue for handling asynchronous services
  // Caller is required to keep this completion queue live until calling
  // BuildAndStart()
  std::unique_ptr<ServerCompletionQueue> AddCompletionQueue();

  // Return a running server which is ready for processing rpcs.
  std::unique_ptr<Server> BuildAndStart();

 private:
  struct Port {
    grpc::string addr;
    std::shared_ptr<ServerCredentials> creds;
    int* selected_port;
  };

  typedef std::unique_ptr<grpc::string> HostString;
  template <class T>
  struct NamedService {
    explicit NamedService(T* s) : service(s) {}
    NamedService(const grpc::string& h, T* s)
        : host(new grpc::string(h)), service(s) {}
    HostString host;
    T* service;
  };

  int max_message_size_;
  std::vector<std::unique_ptr<NamedService<RpcService>>> services_;
  std::vector<std::unique_ptr<NamedService<AsynchronousService>>>
      async_services_;
  std::vector<Port> ports_;
  std::vector<ServerCompletionQueue*> cqs_;
  std::shared_ptr<ServerCredentials> creds_;
  AsyncGenericService* generic_service_;
  ThreadPoolInterface* thread_pool_;
};

}  // namespace grpc

#endif  // GRPCXX_SERVER_BUILDER_H
