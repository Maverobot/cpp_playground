#include <chrono>
#include <iostream>
#include <thread>

#include <smart_rate/simple_control.h>
#include <smart_rate/smart_rate.h>

int main(int argc, char *argv[]) {
  auto rates = std::make_tuple(SmartRate<NoControl>(1000),
                               SmartRate<SimplePControl>(1000));
  using namespace std::chrono_literals;
  using std::chrono::high_resolution_clock;
  auto prev = high_resolution_clock::now();
  size_t i = 0;
  while (true) {

    // do something...
    std::this_thread::sleep_for(100us);

    if (i < 1000) {
      std::get<0>(rates).sleep();
    } else {
      std::get<1>(rates).sleep();
    }
    i++;
  }

  return 0;
}
