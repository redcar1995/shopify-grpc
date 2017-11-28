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

#include "src/core/lib/backoff/backoff.h"

#include <algorithm>

#include <grpc/support/useful.h>

namespace grpc_core {

namespace {

/* Generate a random number between 0 and 1. We roll our own RNG because seeding
 * rand() modifies a global variable we have no control over. */
double generate_uniform_random_number(uint32_t* rng_state) {
  *rng_state = (1103515245 * *rng_state + 12345) % ((uint32_t)1 << 31);
  return *rng_state / (double)((uint32_t)1 << 31);
}

double generate_uniform_random_number_between(uint32_t* rng_state, double a,
                                              double b) {
  if (a == b) return a;
  if (a > b) GPR_SWAP(double, a, b);  // make sure a < b
  const double range = b - a;
  return a + generate_uniform_random_number(rng_state) * range;
}
}  // namespace

Backoff::Backoff(const Options& options) : options_(options) {
  rng_state_ = (unsigned int)gpr_now(GPR_CLOCK_REALTIME).tv_nsec;
}

Backoff::Result Backoff::Begin(grpc_exec_ctx* exec_ctx) {
  current_backoff_ = options_.initial_backoff();
  const grpc_millis initial_timeout =
      std::max(options_.initial_backoff(), options_.min_connect_timeout());
  const grpc_millis now = grpc_exec_ctx_now(exec_ctx);
  return Backoff::Result{now + initial_timeout, now + current_backoff_};
}

Backoff::Result Backoff::Step(grpc_exec_ctx* exec_ctx) {
  current_backoff_ =
      (grpc_millis)(std::min(current_backoff_ * options_.multiplier(),
                             (double)options_.max_backoff()));
  const double jitter = generate_uniform_random_number_between(
      &rng_state_, -options_.jitter() * current_backoff_,
      options_.jitter() * current_backoff_);
  const grpc_millis current_timeout = std::max(
      (grpc_millis)(current_backoff_ + jitter), options_.min_connect_timeout());
  const grpc_millis next_timeout = std::min(
      (grpc_millis)(current_backoff_ + jitter), options_.max_backoff());
  const grpc_millis now = grpc_exec_ctx_now(exec_ctx);
  return Backoff::Result{now + current_timeout, now + next_timeout};
}

void Backoff::Reset() { current_backoff_ = options_.initial_backoff(); }

void Backoff::SetRandomSeed(uint32_t seed) { rng_state_ = seed; }

}  // namespace grpc_core
