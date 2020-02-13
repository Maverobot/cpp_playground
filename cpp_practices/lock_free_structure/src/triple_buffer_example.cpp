#include <triple_buffer.h>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

int main(int argc, char* argv[]) {
  TripleBuffer<double> buf = 0;
  std::atomic<bool> stop = false;

  std::thread print_thread([&buf, &stop] {
    while (!stop) {
      std::cout << buf << std::endl;
    }
  });

  for (size_t i = 0; i < 100; i++) {
    buf = i;
    std::this_thread::sleep_for(200ms);
  }

  std::this_thread::sleep_for(1s);
  stop = true;
  print_thread.join();
  return 0;
}
