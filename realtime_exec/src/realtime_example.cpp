#include <realtime_exec.h>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;
int main(int argc, char* argv[]) {
  realtime_exec::RealtimeExecutor<double> executor;

  executor.init([](double data) {
    std::cout << "start waiting with data: " << data << std::endl;
    std::this_thread::sleep_for(3s);
    std::cout << "end" << std::endl;
  });

  double i = 0;
  for (;;) {
    executor.execute(i);
    std::this_thread::sleep_for(1ms);
    i += 0.001;
  }
  return 0;
}
