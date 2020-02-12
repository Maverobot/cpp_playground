#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace realtime_executor {
template <typename T, size_t queue_size = 0>
class Queue {
 public:
  Queue() = default;
  void push(T msg) {
    std::lock_guard<std::mutex> lk(m_);
    q_.push(msg);
    c_.notify_all();
    pop_if_needed();
  }

  T waitAndPop() {
    std::unique_lock<std::mutex> lk(m_);
    c_.wait(lk, [this] { return !q_.empty(); });
    auto res = q_.front();
    q_.pop();
    return res;
  }

 private:
  template <size_t q_size = queue_size, std::enable_if_t<q_size == 0>* = nullptr>
  void pop_if_needed() {}

  template <size_t q_size = queue_size, std::enable_if_t<q_size != 0>* = nullptr>
  void pop_if_needed() {
    if (q_.size() > queue_size) {
      q_.pop();
    }
  }

  std::condition_variable c_;
  std::queue<T> q_;
  std::mutex m_;
};
}  // namespace realtime_executor
