/*
 *
 * Copyright 2016 gRPC authors.
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

#ifndef GRPC_CORE_LIB_BACKOFF_BACKOFF_H
#define GRPC_CORE_LIB_BACKOFF_BACKOFF_H

#include "src/core/lib/iomgr/exec_ctx.h"

namespace grpc_core {

class Backoff {
 public:
  class Options;
  struct Result;

  /// Initialize backoff machinery - does not need to be destroyed
  explicit Backoff(const Options& options);

  /// Begin retry loop: returns the deadlines to be used for the current attempt
  /// and the subsequent retry, if any.
  Result Begin(grpc_exec_ctx* exec_ctx);
  /// Step a retry loop: returns the deadlines to be used for the current
  /// attempt and the subsequent retry, if any.
  Result Step(grpc_exec_ctx* exec_ctx);
  /// Reset the backoff, so the next grpc_backoff_step will be a
  /// grpc_backoff_begin.
  void Reset();

  void SetRandomSeed(unsigned int seed);

  class Options {
   public:
    Options& set_initial_backoff(grpc_millis initial_backoff) {
      initial_backoff_ = initial_backoff;
      return *this;
    }
    Options& set_multiplier(double multiplier) {
      multiplier_ = multiplier;
      return *this;
    }
    Options& set_jitter(double jitter) {
      jitter_ = jitter;
      return *this;
    }
    Options& set_min_connect_timeout(grpc_millis min_connect_timeout) {
      min_connect_timeout_ = min_connect_timeout;
      return *this;
    }
    Options& set_max_backoff(grpc_millis max_backoff) {
      max_backoff_ = max_backoff;
      return *this;
    }
    /// how long to wait after the first failure before retrying
    grpc_millis initial_backoff() const { return initial_backoff_; }
    /// factor with which to multiply backoff after a failed retry
    double multiplier() const { return multiplier_; }
    /// amount to randomize backoffs
    double jitter() const { return jitter_; }
    /// minimum time between retries
    grpc_millis min_connect_timeout() const { return min_connect_timeout_; }
    /// maximum time between retries
    grpc_millis max_backoff() const { return max_backoff_; }

   private:
    grpc_millis initial_backoff_;
    double multiplier_;
    double jitter_;
    grpc_millis min_connect_timeout_;
    grpc_millis max_backoff_;
  };  // class Options

  struct Result {
    /// Deadline to be used for the current attempt.
    grpc_millis current_deadline;
    /// Deadline to be used for the next attempt, following the backoff
    /// strategy.
    grpc_millis next_attempt_start_time;
  };

 private:
  const Options options_;
  /// current delay before retries
  grpc_millis current_backoff_;

  unsigned int seed;
};

}  // namespace grpc_core

#endif /* GRPC_CORE_LIB_BACKOFF_BACKOFF_H */
