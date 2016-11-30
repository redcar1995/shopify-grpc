/*
 *
 * Copyright 2015-2016, Google Inc.
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

#include <signal.h>
#include <string.h>

#include <memory>
#include <mutex>
#include <sstream>
#include <string>

#include <grpc/support/log.h>

#include "src/core/lib/support/env.h"
#include "test/core/util/port.h"
#include "test/cpp/util/subprocess.h"

using grpc::SubProcess;
typedef std::unique_ptr<SubProcess> SubProcessPtr;
SubProcessPtr g_driver;
constexpr auto kNumWorkers = 2;
std::vector<SubProcessPtr> g_workers(2);

template <class T>
std::string as_string(const T& val) {
  std::ostringstream out;
  out << val;
  return out.str();
}

static void sighandler(int sig) {
  const int errno_saved = errno;
  g_driver->Interrupt();
  for (const auto& worker : g_workers)
    if (worker) worker->Interrupt();
  errno = errno_saved;
}

static void register_sighandler() {
  struct sigaction act;
  memset(&act, 0, sizeof(act));
  act.sa_handler = sighandler;

  sigaction(SIGINT, &act, NULL);
  sigaction(SIGTERM, &act, NULL);
}

int main(int argc, char** argv) {
  register_sighandler();

  std::string my_bin = argv[0];
  std::string bin_dir = my_bin.substr(0, my_bin.rfind('/'));

  std::ostringstream env;
  bool first = true;

  for (int i = 0; i < kNumWorkers; i++) {
    const auto port = grpc_pick_unused_port_or_die();
    std::vector<std::string> args = {bin_dir + "/qps_worker", "-driver_port",
                                     as_string(port)};
    g_workers[i].reset(new SubProcess(args));
    if (!first) env << ",";
    env << "localhost:" << port;
    first = false;
  }

  gpr_setenv("QPS_WORKERS", env.str().c_str());
  std::vector<std::string> args = {bin_dir + "/qps_json_driver"};
  for (int i = 1; i < argc; i++) {
    args.push_back(argv[i]);
  }

  g_driver.reset(new SubProcess(args));
  const int driver_join_status = g_driver->Join();
  for (const auto& worker : g_workers)
    if (worker) worker->Interrupt();
  for (const auto& worker : g_workers)
    if (worker) worker->Join();
  GPR_ASSERT(driver_join_status == 0);
}
