/*
 *
 * Copyright 2014, Google Inc.
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

#include <grpc/support/port_platform.h>

#ifdef GPR_CPU_LINUX

#include "src/core/support/cpu.h"

#define _GNU_SOURCE
#define __USE_GNU
#define __USE_MISC
#include <sched.h>
#undef _GNU_SOURCE
#undef __USE_GNU
#undef __USE_MISC

#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <grpc/support/log.h>

int gpr_cpu_num_cores() {
  static int ncpus = 0;
  if (ncpus == 0) {
    ncpus = sysconf(_SC_NPROCESSORS_ONLN);
    if (ncpus < 1) {
      gpr_log(GPR_ERROR, "Cannot determine number of CPUs: assuming 1");
      ncpus = 1;
    }
  }
  return ncpus;
}

int gpr_cpu_current_cpu() {
  int cpu = sched_getcpu();
  if (cpu < 0) {
    gpr_log(GPR_ERROR, "Error determining current CPU: %s\n", strerror(errno));
    return 0;
  }
  return cpu;
}

#endif /* GPR_CPU_LINUX */
