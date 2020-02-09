#include <realtime_executor.h>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;
int main(int argc, char* argv[]) {
  // Define a function which is too slow to run in a realtime loop
  auto slow_func = [](double data) {
    std::cout << "start waiting with data: " << data << std::endl;
    std::this_thread::sleep_for(3s);
    std::cout << "end" << std::endl;
    return 0;
  };

  // Instantiate the realtime executor
  realtime_executor::RealtimeExecutor<double> executor(slow_func);

  double i = 0;
  for (;;) {
    executor.execute(i);
    std::this_thread::sleep_for(1ms);
    i += 0.001;
  }
  return 0;
}
