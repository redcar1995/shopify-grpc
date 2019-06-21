/*
 *
 * Copyright 2019 gRPC authors.
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

#include "src/core/lib/iomgr/executor/mpmcqueue.h"

#include <grpc/grpc.h>
#include <grpc/support/alloc.h>
#include <grpc/support/log.h>

#include "src/core/lib/gpr/useful.h"
#include "src/core/lib/gprpp/thd.h"
#include "test/core/util/test_config.h"

#define THREAD_SMALL_ITERATION 100
#define THREAD_LARGE_ITERATION 10000

// Testing items for queue
struct WorkItem {
  int index;
  bool done;

  WorkItem(int i) : index(i) { done = false; }
};

// Thread for put items into queue
class ProducerThread {
 public:
  ProducerThread(grpc_core::InfLenFIFOQueue* queue, int start_index,
                 int num_items)
      : start_index_(start_index), num_items_(num_items), queue_(queue) {
    items_ = nullptr;
    thd_ = grpc_core::Thread(
        "mpmcq_test_mt_put_thd",
        [](void* th) { static_cast<ProducerThread*>(th)->Run(); }, this);
  }
  ~ProducerThread() {
    for (int i = 0; i < num_items_; ++i) {
      GPR_ASSERT(items_[i]->done);
      delete items_[i];
    }
    delete[] items_;
  }

  void Start() { thd_.Start(); }
  void Join() { thd_.Join(); }

 private:
  void Run() {
    items_ = new WorkItem*[num_items_];
    for (int i = 0; i < num_items_; ++i) {
      items_[i] = new WorkItem(start_index_ + i);
      queue_->Put(items_[i]);
    }
  }

  int start_index_;
  int num_items_;
  grpc_core::InfLenFIFOQueue* queue_;
  grpc_core::Thread thd_;
  WorkItem** items_;
};

static void ConsumerThread(void* args) {
  grpc_core::InfLenFIFOQueue* queue =
      static_cast<grpc_core::InfLenFIFOQueue*>(args);

  // count number of Get() called in this thread
  int count = 0;

  WorkItem* item;
  while ((item = static_cast<WorkItem*>(queue->Get())) != nullptr) {
    count++;
    GPR_ASSERT(!item->done);
    item->done = true;
  }

  gpr_log(GPR_DEBUG, "ConsumerThread: %d times of Get() called.", count);
}

static void test_get_empty(void) {
  gpr_log(GPR_INFO, "test_get_empty");
  grpc_core::InfLenFIFOQueue queue;
  GPR_ASSERT(queue.count() == 0);
  const int num_threads = 10;
  grpc_core::Thread thds[num_threads];

  // Fork threads. Threads should block at the beginning since queue is empty.
  for (int i = 0; i < num_threads; ++i) {
    thds[i] = grpc_core::Thread("mpmcq_test_ge_thd", ConsumerThread, &queue);
    thds[i].Start();
  }

  WorkItem** items = new WorkItem*[THREAD_LARGE_ITERATION];
  for (int i = 0; i < THREAD_LARGE_ITERATION; ++i) {
    items[i] = new WorkItem(i);
    queue.Put(static_cast<void*>(items[i]));
  }

  gpr_log(GPR_DEBUG, "Terminating threads...");
  for (int i = 0; i < num_threads; ++i) {
    queue.Put(nullptr);
  }
  for (int i = 0; i < num_threads; ++i) {
    thds[i].Join();
  }
  gpr_log(GPR_DEBUG, "Checking and Cleaning Up...");
  for (int i = 0; i < THREAD_LARGE_ITERATION; ++i) {
    GPR_ASSERT(items[i]->done);
    delete items[i];
  }
  delete[] items;
  gpr_log(GPR_DEBUG, "Done.");
}

static void test_FIFO(void) {
  gpr_log(GPR_INFO, "test_FIFO");
  grpc_core::InfLenFIFOQueue large_queue;
  for (int i = 0; i < THREAD_LARGE_ITERATION; ++i) {
    large_queue.Put(static_cast<void*>(new WorkItem(i)));
  }
  GPR_ASSERT(large_queue.count() == THREAD_LARGE_ITERATION);
  for (int i = 0; i < THREAD_LARGE_ITERATION; ++i) {
    WorkItem* item = static_cast<WorkItem*>(large_queue.Get());
    GPR_ASSERT(i == item->index);
    delete item;
  }
}

static void test_many_thread(void) {
  gpr_log(GPR_INFO, "test_many_thread");
  const int num_work_thd = 10;
  const int num_get_thd = 20;
  grpc_core::InfLenFIFOQueue queue;
  ProducerThread** work_thds = new ProducerThread*[num_work_thd];
  grpc_core::Thread get_thds[num_get_thd];

  gpr_log(GPR_DEBUG, "Fork ProducerThread...");
  for (int i = 0; i < num_work_thd; ++i) {
    work_thds[i] = new ProducerThread(&queue, i * THREAD_LARGE_ITERATION,
                                      THREAD_LARGE_ITERATION);
    work_thds[i]->Start();
  }
  gpr_log(GPR_DEBUG, "ProducerThread Started.");
  gpr_log(GPR_DEBUG, "Fork Getter Thread...");
  for (int i = 0; i < num_get_thd; ++i) {
    get_thds[i] =
        grpc_core::Thread("mpmcq_test_mt_get_thd", ConsumerThread, &queue);
    get_thds[i].Start();
  }
  gpr_log(GPR_DEBUG, "Getter Thread Started.");
  gpr_log(GPR_DEBUG, "Waiting ProducerThread to finish...");
  for (int i = 0; i < num_work_thd; ++i) {
    work_thds[i]->Join();
  }
  gpr_log(GPR_DEBUG, "All ProducerThread Terminated.");
  gpr_log(GPR_DEBUG, "Terminating Getter Thread...");
  for (int i = 0; i < num_get_thd; ++i) {
    queue.Put(nullptr);
  }
  for (int i = 0; i < num_get_thd; ++i) {
    get_thds[i].Join();
  }
  gpr_log(GPR_DEBUG, "All Getter Thread Terminated.");
  gpr_log(GPR_DEBUG, "Checking WorkItems and Cleaning Up...");
  for (int i = 0; i < num_work_thd; ++i) {
    delete work_thds[i];
  }
  delete[] work_thds;
  gpr_log(GPR_DEBUG, "Done.");
}

int main(int argc, char** argv) {
  grpc::testing::TestEnvironment env(argc, argv);
  grpc_init();
  gpr_set_log_verbosity(GPR_LOG_SEVERITY_DEBUG);
  test_get_empty();
  test_FIFO();
  test_many_thread();
  grpc_shutdown();
  return 0;
}
