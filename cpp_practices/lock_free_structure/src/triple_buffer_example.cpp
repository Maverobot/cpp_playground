#include <triple_buffer.h>
#include <chrono>
#include <iostream>

int main(int argc, char* argv[]) {
  TripleBuffer<double> buf = 0;
  std::atomic<bool> stop = false;

  std::thread print_thread([&buf, &stop] {
    std::cout << "in print thread" << std::endl;
    while (!stop) {
      std::cout << "print" << std::endl;
      std::cout << buf << std::endl;
    }
    std::cout << "out print thread" << std::endl;
  });

  for (size_t i = 0; i < 100; i++) {
    buf = i;
  }

  std::cout << "waiting" << std::endl;
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(1s);
  std::cout << "waiting done" << std::endl;

  stop = true;
  print_thread.join();
  return 0;
}
