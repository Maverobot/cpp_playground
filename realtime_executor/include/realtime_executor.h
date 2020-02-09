#pragma once
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

template <typename Data>
class RealtimeExecutor {
 public:
  RealtimeExecutor() = delete;

  template <typename F, typename = std::enable_if_t<is_invocable<F, Data>::value>>
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

  void execute(Data data) {
    {
      std::lock_guard<std::mutex> lock(data_mutex_);
      data_ = data;
      turn_ = Status::DATA_ARRIVED;
    }
    updated_cond_.notify_one();
  }

  void stop() { keep_running_ = false; }

 private:
  // non-copyable
  RealtimeExecutor(const RealtimeExecutor&) = delete;
  RealtimeExecutor& operator=(const RealtimeExecutor&) = delete;

  template <typename F>
  void execution_loop(F f) {
    is_running_ = true;
    Data data_copy;
    while (keep_running_) {
      {
        std::unique_lock<std::mutex> l(data_mutex_);
        std::cout << (l.owns_lock() ? "owns" : "not own") << std::endl;
        updated_cond_.wait(l, [this] { return turn_ == Status::DATA_ARRIVED; });
        data_copy = data_;
        turn_ = Status::WAITING;
      }

      f(data_copy);
    }
    is_running_ = false;
  }

  std::thread thread_;

  Data data_;
  std::mutex data_mutex_;  // Protects msg_

  std::atomic<bool> keep_running_;
  std::atomic<bool> is_running_;
  std::condition_variable updated_cond_;

  enum class Status { WAITING, DATA_ARRIVED, LOOP_NOT_STARTED };
  Status turn_{Status::LOOP_NOT_STARTED};  // Who's turn is it to use msg_?
};

template <class Data>
using RealtimeExecutorSharedPtr = std::shared_ptr<RealtimeExecutor<Data>>;

}  // namespace realtime_executor
