/*
 *
 * Copyright 2017 gRPC authors.
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

#ifndef GRPC_CORE_LIB_GPRPP_REF_COUNTED_H
#define GRPC_CORE_LIB_GPRPP_REF_COUNTED_H

#include <grpc/support/port_platform.h>

#include <grpc/support/log.h>
#include <grpc/support/sync.h>

#include <cinttypes>

#include "src/core/lib/debug/trace.h"
#include "src/core/lib/gprpp/abstract.h"
#include "src/core/lib/gprpp/debug_location.h"
#include "src/core/lib/gprpp/memory.h"
#include "src/core/lib/gprpp/ref_counted_ptr.h"

namespace grpc_core {

// PolymorphicRefCount enforces polymorphic destruction of RefCounted.
class PolymorphicRefCount {
 public:
  GRPC_ABSTRACT_BASE_CLASS

 protected:
  GPRC_ALLOW_CLASS_TO_USE_NON_PUBLIC_DELETE

  virtual ~PolymorphicRefCount() {}
};

// NonPolymorphicRefCount does not enforce polymorphic destruction of
// RefCounted. Please refer to grpc_core::RefCounted for more details, and
// when in doubt use PolymorphicRefCount.
class NonPolymorphicRefCount {
 public:
  GRPC_ABSTRACT_BASE_CLASS

 protected:
  GPRC_ALLOW_CLASS_TO_USE_NON_PUBLIC_DELETE

  ~NonPolymorphicRefCount() {}
};

// A base class for reference-counted objects.
// New objects should be created via New() and start with a refcount of 1.
// When the refcount reaches 0, the object will be deleted via Delete().
//
// This will commonly be used by CRTP (curiously-recurring template pattern)
// e.g., class MyClass : public RefCounted<MyClass>
//
// Use PolymorphicRefCount and NonPolymorphicRefCount to select between
// different implementations of RefCounted.
//
// Note that NonPolymorphicRefCount does not support polymorphic destruction.
// So, use NonPolymorphicRefCount only when both of the following conditions
// are guaranteed to hold:
// (a) Child is a concrete leaf class in RefCounted<Child>, and
// (b) you are gauranteed to call Unref only on concrete leaf classes and not
//     their parents.
//
// The following example is illegal, because calling Unref() will not call
// the dtor of Child.
//
//    class Parent : public RefCounted<Parent, NonPolymorphicRefCount> {}
//    class Child : public Parent {}
//
//    Child* ch;
//    ch->Unref();
//
template <typename Child, typename Impl = PolymorphicRefCount>
class RefCounted : public Impl {
 public:
  RefCountedPtr<Child> Ref() GRPC_MUST_USE_RESULT {
    IncrementRefCount();
    return RefCountedPtr<Child>(static_cast<Child*>(this));
  }

  // TODO(roth): Once all of our code is converted to C++ and can use
  // RefCountedPtr<> instead of manual ref-counting, make this method
  // private, since it will only be used by RefCountedPtr<>, which is a
  // friend of this class.
  void Unref() {
    if (gpr_unref(&refs_)) {
      Delete(static_cast<Child*>(this));
    }
  }

  // Not copyable nor movable.
  RefCounted(const RefCounted&) = delete;
  RefCounted& operator=(const RefCounted&) = delete;

  GRPC_ABSTRACT_BASE_CLASS

 protected:
  GPRC_ALLOW_CLASS_TO_USE_NON_PUBLIC_DELETE

  RefCounted() { gpr_ref_init(&refs_, 1); }

  // Note: Depending on the Impl used, this dtor can be implicitly virtual.
  ~RefCounted() {}

 private:
  // Allow RefCountedPtr<> to access IncrementRefCount().
  template <typename T>
  friend class RefCountedPtr;

  void IncrementRefCount() { gpr_ref(&refs_); }

  gpr_refcount refs_;
};

// An alternative version of the RefCounted base class that
// supports tracing.  This is intended to be used in cases where the
// object will be handled both by idiomatic C++ code using smart
// pointers and legacy code that is manually calling Ref() and Unref().
// Once all of our code is converted to idiomatic C++, we may be able to
// eliminate this class.
template <typename Child, typename Impl = PolymorphicRefCount>
class RefCountedWithTracing : public Impl {
 public:
  RefCountedPtr<Child> Ref() GRPC_MUST_USE_RESULT {
    IncrementRefCount();
    return RefCountedPtr<Child>(static_cast<Child*>(this));
  }

  RefCountedPtr<Child> Ref(const DebugLocation& location,
                           const char* reason) GRPC_MUST_USE_RESULT {
    if (location.Log() && trace_flag_ != nullptr && trace_flag_->enabled()) {
      gpr_atm old_refs = gpr_atm_no_barrier_load(&refs_.count);
      gpr_log(GPR_INFO, "%s:%p %s:%d ref %" PRIdPTR " -> %" PRIdPTR " %s",
              trace_flag_->name(), this, location.file(), location.line(),
              old_refs, old_refs + 1, reason);
    }
    return Ref();
  }

  // TODO(roth): Once all of our code is converted to C++ and can use
  // RefCountedPtr<> instead of manual ref-counting, make the Unref() methods
  // private, since they will only be used by RefCountedPtr<>, which is a
  // friend of this class.

  void Unref() {
    if (gpr_unref(&refs_)) {
      Delete(static_cast<Child*>(this));
    }
  }

  void Unref(const DebugLocation& location, const char* reason) {
    if (location.Log() && trace_flag_ != nullptr && trace_flag_->enabled()) {
      gpr_atm old_refs = gpr_atm_no_barrier_load(&refs_.count);
      gpr_log(GPR_INFO, "%s:%p %s:%d unref %" PRIdPTR " -> %" PRIdPTR " %s",
              trace_flag_->name(), this, location.file(), location.line(),
              old_refs, old_refs - 1, reason);
    }
    Unref();
  }

  // Not copyable nor movable.
  RefCountedWithTracing(const RefCountedWithTracing&) = delete;
  RefCountedWithTracing& operator=(const RefCountedWithTracing&) = delete;

  GRPC_ABSTRACT_BASE_CLASS

 protected:
  GPRC_ALLOW_CLASS_TO_USE_NON_PUBLIC_DELETE

  RefCountedWithTracing()
      : RefCountedWithTracing(static_cast<TraceFlag*>(nullptr)) {}

  explicit RefCountedWithTracing(TraceFlag* trace_flag)
      : trace_flag_(trace_flag) {
    gpr_ref_init(&refs_, 1);
  }

#ifdef NDEBUG
  explicit RefCountedWithTracing(DebugOnlyTraceFlag* trace_flag)
      : RefCountedWithTracing() {}
#endif

  // Note: Depending on the Impl used, this dtor can be implicitly virtual.
  ~RefCountedWithTracing() {}

 private:
  // Allow RefCountedPtr<> to access IncrementRefCount().
  template <typename T>
  friend class RefCountedPtr;

  void IncrementRefCount() { gpr_ref(&refs_); }

  TraceFlag* trace_flag_ = nullptr;
  gpr_refcount refs_;
};

}  // namespace grpc_core

#endif /* GRPC_CORE_LIB_GPRPP_REF_COUNTED_H */
