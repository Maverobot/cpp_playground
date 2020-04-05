#include <catch2/catch.hpp>

#include <delegate.h>

static constexpr size_t kSamples = 100000000;

void stdF() {
  int x{0};
  std::function<int(int)> t2 = [&x](int i) { return i + x; };
  std::function<void(int)> t1 = [&x, &t2](int i) { x = t2(i); };
  for (size_t i = 0; i < kSamples; ++i)
    t1(i);
}

void delegateF() {
  int x{0};
  delegate<int(int)> t2 = [&x](int i) { return i + x; };
  delegate<void(int)> t1 = [&x, &t2](int i) { x = t2(i); };
  for (size_t i = 0; i < kSamples; ++i)
    t1(i);
}

TEST_CASE("delegate") {
  BENCHMARK("delegate") { return delegateF(); };
  BENCHMARK("std::function") { return stdF(); };
}
