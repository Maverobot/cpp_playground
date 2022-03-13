// Listing 5.6 in "C++ Concurrency in Action, Second Edition"
#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int> x;
std::atomic<int> y;
std::atomic<int> z;

std::atomic<bool> go;

unsigned const loop_count = 10;

struct ReadValues {
  int x;
  int y;
  int z;
};

ReadValues values1[loop_count];
ReadValues values2[loop_count];
ReadValues values3[loop_count];
ReadValues values4[loop_count];
ReadValues values5[loop_count];

void increment(std::atomic<int>* var_to_inc, ReadValues* values) {
  while (!go) {
    std::this_thread::yield();
  }

  for (unsigned i = 0; i < loop_count; i++) {
    values[i].x = x.load(std::memory_order_relaxed);
    values[i].y = y.load(std::memory_order_relaxed);
    values[i].z = z.load(std::memory_order_relaxed);
    var_to_inc->store(i + 1, std::memory_order_relaxed);
    std::this_thread::yield();
  }
}

void readValues(ReadValues* values) {
  while (!go) {
    std::this_thread::yield();
  }

  for (unsigned i = 0; i < loop_count; i++) {
    values[i].x = x.load(std::memory_order_relaxed);
    values[i].y = y.load(std::memory_order_relaxed);
    values[i].z = z.load(std::memory_order_relaxed);
    std::this_thread::yield();
  }
}

void print(ReadValues* v) {
  for (unsigned i = 0; i < loop_count; i++) {
    if (i) {
      std::cout << ", ";
    }
    std::cout << "(" << v[i].x << ", " << v[i].y << ", " << v[i].z << ")";
  }
  std::cout << "\n";
}

int main(int argc, char* argv[]) {
  std::thread t1(increment, &x, values1);
  std::thread t2(increment, &y, values2);
  std::thread t3(increment, &z, values3);
  std::thread t4(readValues, values4);
  std::thread t5(readValues, values5);

  go = true;
  t5.join();
  t4.join();
  t3.join();
  t2.join();
  t1.join();

  print(values1);
  print(values2);
  print(values3);
  print(values4);
  print(values5);
  return 0;
}
