// For more information:
// https://stackoverflow.com/questions/77062940/setting-condition-variable-monitor-flag-without-a-lock-is-it-valid
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

int main() {
  std::mutex m;
  std::condition_variable cv;
  std::atomic<bool> ready = false;
  std::atomic<bool> thread2_finished = false;
  std::thread thread2 = std::thread([&]() {
    std::unique_lock<std::mutex> l(m);
    cv.wait(l, [&ready] {
      auto r = ready.load();
      std::cout << "in wait loop going for a short sleep, ready = " << std::boolalpha << r << "\n";
      std::this_thread::sleep_for(20ms);
      std::cout << "in wait loop, after short sleep, ready = " << std::boolalpha << r << "\n";
      return r;
    });
    std::cout << "Hello from thread2\n";
    thread2_finished = true;
  });
  std::this_thread::sleep_for(20ms);
  std::cout << "Hello from main thread\n";
  ready = true;
  // note: it is valid to call notify_one / notify_all without a lock, see:
  // https://stackoverflow.com/questions/17101922/do-i-have-to-acquire-lock-before-calling-condition-variable-notify-one
  // the idea behind condition variable is that it can be notified by more than one thread (thus no
  // lock)
  cv.notify_one();

  thread2.detach();
  using Clock = std::chrono::steady_clock;
  auto end_time = Clock::now() + 2s;
  while (Clock::now() < end_time) {
    if (thread2_finished)
      break;
    std::this_thread::sleep_for(10ms);
  }
  if (!thread2_finished) {
    std::cout << ">>>> thread2 is hanging, ending main <<<<\n";
  }
  std::cout << "Goodbye from main thread\n";
}
