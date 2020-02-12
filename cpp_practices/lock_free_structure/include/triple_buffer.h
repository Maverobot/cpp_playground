#pragma once
// https://codereview.stackexchange.com/q/163810/216881

#include <atomic>
#include <thread>

template <typename buffer, void produce(buffer*), void consume(buffer*)>
class triple_buffer {
 public:
  triple_buffer() = delete;
  triple_buffer(triple_buffer& other) = delete;
  triple_buffer(triple_buffer&& other) = delete;
  triple_buffer& operator=(triple_buffer& other) = delete;

  triple_buffer(buffer bufs[3]) : present(&bufs[0]), ready(&bufs[1]), inprogress(&bufs[2]) {
    stop.store(false);
    stale.test_and_set();

    produce(present);

    consumer_thread = std::move(std::thread([this] { consumer(); }));
    producer_thread = std::move(std::thread([this] { producer(); }));
  }

  ~triple_buffer() {
    stop.store(true);
    producer_thread.join();

    stale.clear();
    consumer_thread.join();
  }

 private:
  std::atomic<buffer*> present;
  std::atomic<buffer*> ready;
  std::atomic<buffer*> inprogress;

  std::atomic<bool> stop;
  std::atomic_flag stale;

  std::thread consumer_thread;
  std::thread producer_thread;

  void producer() {
    while (!stop) {
      produce(inprogress.load());
      inprogress = ready.exchange(inprogress);
      stale.clear();
    }
  }

  void consumer() {
    while (!stop) {
      consume(present.load());
      while (stale.test_and_set())
        ;
      present = ready.exchange(present);
    }
  }
};
