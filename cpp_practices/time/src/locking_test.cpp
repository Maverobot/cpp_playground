#include <catch.hpp>

#include <atomic>
#include <future>
#include <mutex>
#include <thread>

TEST_CASE("locking") {
  std::mutex mutex;

  std::atomic<bool> finished = false;

  auto lock_unlock_loop = std::async(std::launch::async, [&mutex, &finished]() {
    while (!finished) {
      mutex.lock();
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      mutex.unlock();
      std::this_thread::yield();
    }
  });
  std::this_thread::sleep_for(std::chrono::seconds(1));

  BENCHMARK("lock_guard") { return std::lock_guard<std::mutex>(mutex); };
  BENCHMARK("unique_lock_lock") {
    std::unique_lock<std::mutex> lock(mutex, std::defer_lock);
    return lock.lock();
  };
  BENCHMARK("unique_lock_try_lock_without_sleep") {
    std::unique_lock<std::mutex> lock(mutex, std::defer_lock);
    while (!lock.try_lock()) {
    }
    return;
  };
  BENCHMARK("unique_lock_try_lock_1ms_sleep") {
    std::unique_lock<std::mutex> lock(mutex, std::defer_lock);
    while (!lock.try_lock()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return;
  };
  BENCHMARK("unique_lock_try_lock_10ms_sleep") {
    std::unique_lock<std::mutex> lock(mutex, std::defer_lock);
    while (!lock.try_lock()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return;
  };
  finished = true;
}
