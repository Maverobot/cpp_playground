#pragma once
/*
  Reference: https://codereview.stackexchange.com/q/163810/216881

  In a producer-consumer scenario sometimes we have to deal with the producer being much faster than
  consumer. The data loss is unavoidable, and we are OK with it, as long as the consumer always has
  the most recent data.

  The answer is triple buffering. Endow the system with three buffers, which assume the roles of
  being presented, ready, and inprogress, and let them change their roles according to the rules:

  1. Once the presented buffer is completely presented, swap roles with ready.
  2. Once the inprogress buffer is completely produced, swap roles with ready.

  It is possible (unlikely but still possible) for producer to fall behind. Consumer must detect
  such situation, and not present a stale buffer. In other words, the presented/ready swap shall
  happen only if ready has been updated since the last swap.
 */

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
