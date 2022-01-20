#include <unistd.h>
#include <atomic>
#include <cassert>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

// clang-format off
/* [WIP] This example tries to synchronize two threads in the following way:
 *   Thread 1:  First_inc-------------------------------[barrier2]Second_inc
 *   Thread 2:  ----------[barrier1]First_dec Second_dec----------
 * Note: "-" means idle or doing other unrelated operations.
 +*/
// clang-format on

int state = 0;
std::mutex mutex;
bool barrier1 = false;
bool barrier2 = false;
std::condition_variable workflow_cv;

void worker_a() {
  while (true) {
    // First ++
    {
      std::unique_lock<std::mutex> lock(mutex);
      state++;
      assert(state == 1);
      std::cout << std::string("worker_a - first state++: ") + std::to_string(state) + "\n";
      barrier1 = true;
    }
    workflow_cv.notify_one();

    // Second ++
    {
      std::unique_lock<std::mutex> lock(mutex);
      workflow_cv.wait(lock, [] { return barrier2; });
      barrier2 = false;
      state++;
      assert(state == 0);
      std::cout << std::string("worker_a - second state++: ") + std::to_string(state) + "\n";
    }
  }
}

void worker_b() {
  while (true) {
    // First --
    {
      std::unique_lock<std::mutex> lock(mutex);
      workflow_cv.wait(lock, [] { return barrier1; });
      barrier1 = false;
      state--;
      assert(state == 0);
      std::cout << std::string("worker_b - first state--: ") + std::to_string(state) + "\n";
    }

    // Second --
    {
      std::unique_lock<std::mutex> lock(mutex);
      state--;
      assert(state == -1);
      std::cout << std::string("worker_b - second state--: ") + std::to_string(state) + "\n";
      barrier2 = true;
    }
    workflow_cv.notify_one();
  }
}

int main() {
  std::thread t2(worker_b);
  std::thread t1(worker_a);
  t1.join();
  t2.join();
}
