#include <catch.hpp>

#include <sys/syscall.h>
#include <unistd.h>
#include <chrono>
#include <iostream>
#include <thread>

void setThreadPriority(int priority) {
  int pid = syscall(SYS_gettid);
  struct sched_param param;
  param.sched_priority = priority;
  if (sched_setscheduler(pid, SCHED_FIFO | SCHED_RESET_ON_FORK, &param) == -1) {
    throw std::runtime_error("failed to set thread priority");
  }
}

TEST_CASE("timing") {
  setThreadPriority(99);

  timespec deadline;
  deadline.tv_sec = 0;
  deadline.tv_nsec = 1000000;  // 1ms

  using namespace std::chrono_literals;
  BENCHMARK("clock_nanosleep") { return clock_nanosleep(CLOCK_REALTIME, 0, &deadline, NULL); };
  BENCHMARK("this_thread::sleep_for") { return std::this_thread::sleep_for(1ms); };
}
