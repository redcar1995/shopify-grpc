//
// Copyright 2016 gRPC authors.
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

// This is similar to the sockaddr resolver, except that it supports a
// bunch of query args that are useful for dependency injection in tests.

#include <grpc/support/port_platform.h>

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <grpc/support/alloc.h>
#include <grpc/support/string_util.h>

#include "src/core/ext/filters/client_channel/parse_address.h"
#include "src/core/ext/filters/client_channel/resolver_registry.h"
#include "src/core/ext/filters/client_channel/server_address.h"
#include "src/core/lib/channel/channel_args.h"
#include "src/core/lib/gpr/string.h"
#include "src/core/lib/gpr/useful.h"
#include "src/core/lib/iomgr/closure.h"
#include "src/core/lib/iomgr/logical_thread.h"
#include "src/core/lib/iomgr/resolve_address.h"
#include "src/core/lib/iomgr/unix_sockets_posix.h"
#include "src/core/lib/slice/slice_internal.h"
#include "src/core/lib/slice/slice_string_helpers.h"

#include "src/core/ext/filters/client_channel/resolver/fake/fake_resolver.h"

namespace grpc_core {

// This cannot be in an anonymous namespace, because it is a friend of
// FakeResolverResponseGenerator.
class FakeResolver : public Resolver {
 public:
  explicit FakeResolver(ResolverArgs args);

  void StartLocked() override;

  void RequestReresolutionLocked() override;

 private:
  friend class FakeResolverResponseGenerator;

  virtual ~FakeResolver();

  void ShutdownLocked() override {
    shutdown_ = true;
    if (response_generator_ != nullptr) {
      response_generator_->SetFakeResolver(nullptr);
      response_generator_.reset();
    }
  }

  void MaybeSendResultLocked();

  static void ReturnReresolutionResult(void* arg);

  // passed-in parameters
  grpc_channel_args* channel_args_ = nullptr;
  RefCountedPtr<FakeResolverResponseGenerator> response_generator_;
  // If has_next_result_ is true, next_result_ is the next resolution result
  // to be returned.
  bool has_next_result_ = false;
  Result next_result_;
  // Result to use for the pretended re-resolution in
  // RequestReresolutionLocked().
  bool has_reresolution_result_ = false;
  Result reresolution_result_;
  // True after the call to StartLocked().
  bool started_ = false;
  // True after the call to ShutdownLocked().
  bool shutdown_ = false;
  // if true, return failure
  bool return_failure_ = false;
  // pending re-resolution
  bool reresolution_closure_pending_ = false;
};

FakeResolver::FakeResolver(ResolverArgs args)
    : Resolver(args.logical_thread, std::move(args.result_handler)),
      response_generator_(
          FakeResolverResponseGenerator::GetFromArgs(args.args)) {
  // Channels sharing the same subchannels may have different resolver response
  // generators. If we don't remove this arg, subchannel pool will create new
  // subchannels for the same address instead of reusing existing ones because
  // of different values of this channel arg.
  const char* args_to_remove[] = {GRPC_ARG_FAKE_RESOLVER_RESPONSE_GENERATOR};
  channel_args_ = grpc_channel_args_copy_and_remove(
      args.args, args_to_remove, GPR_ARRAY_SIZE(args_to_remove));
  if (response_generator_ != nullptr) {
    response_generator_->SetFakeResolver(Ref());
  }
}

FakeResolver::~FakeResolver() { grpc_channel_args_destroy(channel_args_); }

void FakeResolver::StartLocked() {
  started_ = true;
  MaybeSendResultLocked();
}

void FakeResolver::RequestReresolutionLocked() {
  if (has_reresolution_result_ || return_failure_) {
    next_result_ = reresolution_result_;
    has_next_result_ = true;
    // Return the result in a different closure, so that we don't call
    // back into the LB policy while it's still processing the previous
    // update.
    if (!reresolution_closure_pending_) {
      reresolution_closure_pending_ = true;
      Ref().release();  // ref held by closure
      logical_thread()->Run([this]() { ReturnReresolutionResult(this); },
                            DEBUG_LOCATION);
    }
  }
}

void FakeResolver::MaybeSendResultLocked() {
  if (!started_ || shutdown_) return;
  if (return_failure_) {
    // TODO(roth): Change resolver result generator to be able to inject
    // the error to be returned.
    result_handler()->ReturnError(grpc_error_set_int(
        GRPC_ERROR_CREATE_FROM_STATIC_STRING("Resolver transient failure"),
        GRPC_ERROR_INT_GRPC_STATUS, GRPC_STATUS_UNAVAILABLE));
    return_failure_ = false;
  } else if (has_next_result_) {
    Result result;
    result.addresses = std::move(next_result_.addresses);
    result.service_config = std::move(next_result_.service_config);
    // TODO(roth): Use std::move() once grpc_error is converted to C++.
    result.service_config_error = next_result_.service_config_error;
    next_result_.service_config_error = GRPC_ERROR_NONE;
    // When both next_results_ and channel_args_ contain an arg with the same
    // name, only the one in next_results_ will be kept since next_results_ is
    // before channel_args_.
    result.args = grpc_channel_args_union(next_result_.args, channel_args_);
    result_handler()->ReturnResult(std::move(result));
    has_next_result_ = false;
  }
}

void FakeResolver::ReturnReresolutionResult(void* arg) {
  FakeResolver* self = static_cast<FakeResolver*>(arg);
  self->reresolution_closure_pending_ = false;
  self->MaybeSendResultLocked();
  self->Unref();
}

//
// FakeResolverResponseGenerator
//

FakeResolverResponseGenerator::FakeResolverResponseGenerator() {}

FakeResolverResponseGenerator::~FakeResolverResponseGenerator() {}

void FakeResolverResponseGenerator::SetResponseLocked(SetResponseArg* arg) {
  auto& resolver = arg->resolver;
  if (!resolver->shutdown_) {
    resolver->next_result_ = std::move(arg->result);
    resolver->has_next_result_ = true;
    resolver->MaybeSendResultLocked();
  }
  delete arg;
}

void FakeResolverResponseGenerator::SetResponse(Resolver::Result result) {
  RefCountedPtr<FakeResolver> resolver;
  {
    MutexLock lock(&mu_);
    if (resolver_ == nullptr) {
      has_result_ = true;
      result_ = std::move(result);
      return;
    }
    resolver = resolver_->Ref();
  }
  SetResponseArg* arg = new SetResponseArg();
  arg->resolver = std::move(resolver);
  arg->result = std::move(result);
  arg->resolver->logical_thread()->Run([arg]() { SetResponseLocked(arg); },
                                       DEBUG_LOCATION);
}

void FakeResolverResponseGenerator::SetReresolutionResponseLocked(
    SetResponseArg* arg) {
  auto& resolver = arg->resolver;
  if (!resolver->shutdown_) {
    resolver->reresolution_result_ = std::move(arg->result);
    resolver->has_reresolution_result_ = arg->has_result;
  }
  delete arg;
}

void FakeResolverResponseGenerator::SetReresolutionResponse(
    Resolver::Result result) {
  RefCountedPtr<FakeResolver> resolver;
  {
    MutexLock lock(&mu_);
    GPR_ASSERT(resolver_ != nullptr);
    resolver = resolver_->Ref();
  }
  SetResponseArg* arg = new SetResponseArg();
  arg->resolver = std::move(resolver);
  arg->result = std::move(result);
  arg->has_result = true;
  arg->resolver->logical_thread()->Run(
      [arg]() { SetReresolutionResponseLocked(arg); }, DEBUG_LOCATION);
}

void FakeResolverResponseGenerator::UnsetReresolutionResponse() {
  RefCountedPtr<FakeResolver> resolver;
  {
    MutexLock lock(&mu_);
    GPR_ASSERT(resolver_ != nullptr);
    resolver = resolver_->Ref();
  }
  SetResponseArg* arg = new SetResponseArg();
  arg->resolver = std::move(resolver);
  arg->resolver->logical_thread()->Run(
      [arg]() { SetReresolutionResponseLocked(arg); }, DEBUG_LOCATION);
}

void FakeResolverResponseGenerator::SetFailureLocked(SetResponseArg* arg) {
  auto& resolver = arg->resolver;
  if (!resolver->shutdown_) {
    resolver->return_failure_ = true;
    if (arg->immediate) resolver->MaybeSendResultLocked();
  }
  delete arg;
}

void FakeResolverResponseGenerator::SetFailure() {
  RefCountedPtr<FakeResolver> resolver;
  {
    MutexLock lock(&mu_);
    GPR_ASSERT(resolver_ != nullptr);
    resolver = resolver_->Ref();
  }
  SetResponseArg* arg = new SetResponseArg();
  arg->resolver = std::move(resolver);
  arg->resolver->logical_thread()->Run([arg]() { SetFailureLocked(arg); },
                                       DEBUG_LOCATION);
}

void FakeResolverResponseGenerator::SetFailureOnReresolution() {
  RefCountedPtr<FakeResolver> resolver;
  {
    MutexLock lock(&mu_);
    GPR_ASSERT(resolver_ != nullptr);
    resolver = resolver_->Ref();
  }
  SetResponseArg* arg = new SetResponseArg();
  arg->resolver = std::move(resolver);
  arg->immediate = false;
  arg->resolver->logical_thread()->Run([arg]() { SetFailureLocked(arg); },
                                       DEBUG_LOCATION);
}

void FakeResolverResponseGenerator::SetFakeResolver(
    RefCountedPtr<FakeResolver> resolver) {
  MutexLock lock(&mu_);
  resolver_ = std::move(resolver);
  if (resolver_ == nullptr) return;
  if (has_result_) {
    SetResponseArg* arg = new SetResponseArg();
    arg->resolver = resolver_->Ref();
    arg->result = std::move(result_);
    resolver_->logical_thread()->Run([arg]() { SetResponseLocked(arg); },
                                     DEBUG_LOCATION);
    has_result_ = false;
  }
}

namespace {

static void* response_generator_arg_copy(void* p) {
  FakeResolverResponseGenerator* generator =
      static_cast<FakeResolverResponseGenerator*>(p);
  // TODO(roth): We currently deal with this ref manually.  Once the
  // new channel args code is converted to C++, find a way to track this ref
  // in a cleaner way.
  RefCountedPtr<FakeResolverResponseGenerator> copy = generator->Ref();
  copy.release();
  return p;
}

static void response_generator_arg_destroy(void* p) {
  FakeResolverResponseGenerator* generator =
      static_cast<FakeResolverResponseGenerator*>(p);
  generator->Unref();
}

static int response_generator_cmp(void* a, void* b) { return GPR_ICMP(a, b); }

static const grpc_arg_pointer_vtable response_generator_arg_vtable = {
    response_generator_arg_copy, response_generator_arg_destroy,
    response_generator_cmp};

}  // namespace

grpc_arg FakeResolverResponseGenerator::MakeChannelArg(
    FakeResolverResponseGenerator* generator) {
  grpc_arg arg;
  arg.type = GRPC_ARG_POINTER;
  arg.key = (char*)GRPC_ARG_FAKE_RESOLVER_RESPONSE_GENERATOR;
  arg.value.pointer.p = generator;
  arg.value.pointer.vtable = &response_generator_arg_vtable;
  return arg;
}

RefCountedPtr<FakeResolverResponseGenerator>
FakeResolverResponseGenerator::GetFromArgs(const grpc_channel_args* args) {
  const grpc_arg* arg =
      grpc_channel_args_find(args, GRPC_ARG_FAKE_RESOLVER_RESPONSE_GENERATOR);
  if (arg == nullptr || arg->type != GRPC_ARG_POINTER) return nullptr;
  return static_cast<FakeResolverResponseGenerator*>(arg->value.pointer.p)
      ->Ref();
}

//
// Factory
//

namespace {

class FakeResolverFactory : public ResolverFactory {
 public:
  bool IsValidUri(const grpc_uri* /*uri*/) const override { return true; }

  OrphanablePtr<Resolver> CreateResolver(ResolverArgs args) const override {
    return MakeOrphanable<FakeResolver>(std::move(args));
  }

  const char* scheme() const override { return "fake"; }
};

}  // namespace

}  // namespace grpc_core

void grpc_resolver_fake_init() {
  grpc_core::ResolverRegistry::Builder::RegisterResolverFactory(
      grpc_core::MakeUnique<grpc_core::FakeResolverFactory>());
}

void grpc_resolver_fake_shutdown() {}
