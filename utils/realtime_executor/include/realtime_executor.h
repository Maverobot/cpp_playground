#pragma once

#include <queue.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

namespace {
template <typename F, typename... Args>
struct is_invocable
    : std::is_constructible<std::function<void(Args...)>,
                            std::reference_wrapper<typename std::remove_reference<F>::type>> {};
}  // namespace

namespace realtime_executor {

struct with_queue {
  // TODO: watch out for queue overflow
  static constexpr bool value = 0;
};
struct without_queue {
  static constexpr bool value = 1;
};

template <typename DataT, typename QueuePolicy = without_queue>
class RealtimeExecutor {
 public:
  RealtimeExecutor() = delete;

  template <typename F, typename = std::enable_if_t<is_invocable<F, DataT>::value>>
  RealtimeExecutor(F f) {
    keep_running_ = true;
    thread_ = std::thread(&RealtimeExecutor::execution_loop<F>, this, f);
  }

  /// Destructor
  ~RealtimeExecutor() {
    keep_running_ = false;
    while (is_running_) {
      std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
    if (thread_.joinable()) {
      thread_.join();
    }
  }

  void execute(DataT data) { data_queue_.push(data); }

  void stop() { keep_running_ = false; }

 private:
  // non-copyable
  RealtimeExecutor(const RealtimeExecutor&) = delete;
  RealtimeExecutor& operator=(const RealtimeExecutor&) = delete;

  template <typename F>
  void execution_loop(F f) {
    is_running_ = true;
    DataT data_copy;
    while (keep_running_) {
      f(data_queue_.waitAndPop());
    }
    is_running_ = false;
  }

  std::thread thread_;

  Queue<DataT, QueuePolicy::value> data_queue_;

  std::atomic<bool> keep_running_;
  std::atomic<bool> is_running_;
};

template <typename DataT>
using RealtimeExecutorSharedPtr = std::shared_ptr<RealtimeExecutor<DataT>>;

}  // namespace realtime_executor
