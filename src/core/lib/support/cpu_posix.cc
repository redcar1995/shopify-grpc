/*
 *
 * Copyright 2015 gRPC authors.
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

#include <grpc/support/port_platform.h>

#if defined(GPR_CPU_POSIX)

#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include <atomic>

#include <grpc/support/alloc.h>
#include <grpc/support/cpu.h>
#include <grpc/support/log.h>
#include <grpc/support/sync.h>
#include <grpc/support/useful.h>

static long ncpus = 0;

static void init_ncpus() {
  ncpus = sysconf(_SC_NPROCESSORS_ONLN);
  if (ncpus < 1 || ncpus > INT32_MAX) {
    gpr_log(GPR_ERROR, "Cannot determine number of CPUs: assuming 1");
    ncpus = 1;
  }
}

unsigned gpr_cpu_num_cores(void) {
  static gpr_once once = GPR_ONCE_INIT;
  gpr_once_init(&once, init_ncpus);
  return (unsigned)ncpus;
}

unsigned gpr_cpu_current_cpu(void) {
  /* NOTE: there's no way I know to return the actual cpu index portably...
     most code that's using this is using it to shard across work queues though,
     so here we use thread identity instead to achieve a similar though not
     identical effect */
  static auto DeleteValue = [](void* value_ptr) {
    unsigned int* value = static_cast<unsigned int*>(value_ptr);
    if (value) {
      gpr_free(value);
    }
  };
  static pthread_key_t thread_id_key;
  static int thread_id_key_create_result __attribute__((unused)) =
      pthread_key_create(&thread_id_key, DeleteValue);
  // pthread_t isn't portably defined to map to an integral type. So keep track
  // of thread identity explicitly so hashing works reliably.
  static std::atomic<unsigned int> thread_counter(0);

  unsigned int* thread_id =
      static_cast<unsigned int*>(pthread_getspecific(thread_id_key));
  if (thread_id == nullptr) {
    thread_id = static_cast<unsigned int*>(gpr_malloc(sizeof(unsigned int)));
    *thread_id = thread_counter++;
    pthread_setspecific(thread_id_key, thread_id);
  }

  return (unsigned)GPR_HASH_POINTER(*thread_id, gpr_cpu_num_cores());
}

#endif /* GPR_CPU_POSIX */
