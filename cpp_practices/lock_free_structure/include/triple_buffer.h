#pragma once

#include <atomic>
#include <thread>

template <typename T>
class TripleBuffer {
 public:
  template <typename U = T,
            typename = std::enable_if_t<std::is_trivially_default_constructible<U>::value>>
  TripleBuffer() {
    bufs_[0] = T{};
    bufs_[1] = T{};
    bufs_[2] = T{};

    present_ = &bufs_[0];
    ready_ = &bufs_[1];
    inprogress_ = &bufs_[2];

    stale_.clear();
  }

  TripleBuffer(const T& val) {
    bufs_[0] = val;
    bufs_[1] = val;
    bufs_[2] = val;

    present_ = &bufs_[0];
    ready_ = &bufs_[1];
    inprogress_ = &bufs_[2];

    stale_.clear();
  }

  ~TripleBuffer() = default;

  TripleBuffer(TripleBuffer& other) = delete;
  TripleBuffer(TripleBuffer&& other) = delete;
  TripleBuffer& operator=(TripleBuffer& other) = delete;

  operator T() const noexcept { return instant_load(); }
  operator T() const volatile noexcept { return instant_load(); }

  T operator=(T i) noexcept {
    store(i);
    return i;
  }
  T operator=(T i) volatile noexcept {
    store(i);
    return i;
  }

  void store(T val) {
    // Store new value into "inprogress"
    *inprogress_.load() = std::move(val);

    // Swap "inprogress" and "ready"
    inprogress_ = ready_.exchange(inprogress_);

    // Notify that a new value has been stored in "ready"
    stale_.clear();
  }

  T wait_load() const {
    // Wait until a new value was stored in "ready"
    while (stale_.test_and_set())
      ;

    // Swap "ready" and "present"
    present_ = ready_.exchange(present_);

    // Load value from "present"
    T val = *present_.load();

    return val;
  }

  T instant_load() const {
    // Load value from "present"
    T val = *present_.load();

    // If there is a new "ready" value, swap "ready" and "present"
    if (!stale_.test_and_set()) {
      present_ = ready_.exchange(present_);
    }
    return val;
  }

 private:
  // store new data -> inprogress -(swap)-> ready -(swap)-> present -> load data
  mutable std::atomic<T*> present_;
  mutable std::atomic<T*> ready_;
  mutable std::atomic<T*> inprogress_;

  mutable std::atomic_flag stale_{ATOMIC_FLAG_INIT};
  T bufs_[3];
};
