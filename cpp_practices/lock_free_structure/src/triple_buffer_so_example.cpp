#include <chrono>
#include <iostream>
#include <thread>

#include <triple_buffer_so.h>

struct buffer {
  int id;
  int value;
};

void produce(buffer* buf) {
  static int value = 0;
  buf->value = value++;
}

void consume(buffer* buf) {
  std::cout << buf->id << ": " << buf->value << '\n';
}
int main(int argc, char* argv[]) {
  buffer bufs[3]{{0}, {1}, {2}};
  triple_buffer<buffer, produce, consume> tb(bufs);
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
