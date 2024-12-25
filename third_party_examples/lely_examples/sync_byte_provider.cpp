#include <lely/ev/loop.hpp>

#if _WIN32
#include <lely/io2/win32/ixxat.hpp>
#include <lely/io2/win32/poll.hpp>
#elif defined(__linux__)
#include <lely/io2/linux/can.hpp>
#include <lely/io2/posix/poll.hpp>
#else
#error This file requires Windows or Linux.
#endif
#include <iostream>
#include <lely/io2/sys/io.hpp>
#include <thread>

using namespace std::chrono_literals;
using namespace lely;

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <can-interface> <frequency(hz)>\n";
    return 1;
  }

  // Create an I/O context to synchronize I/O services during shutdown.
  io::Context ctx;
  // Create an platform-specific I/O polling instance to monitor the CAN bus, as
  // well as timers and signals.
  io::Poll poll(ctx);
  // Create a polling event loop and pass it the platform-independent polling
  // interface. If no tasks are pending, the event loop will poll for I/O
  // events.
  ev::Loop loop(poll.get_poll());
  // I/O devices only need access to the executor interface of the event loop.
  auto exec = loop.get_executor();

  // Create a virtual SocketCAN CAN controller and channel, and do not modify
  // the current CAN bus state or bitrate.
  io::CanController ctrl(argv[1]);
  io::CanChannel chan(poll, exec);
  chan.open(ctrl);

  const can_msg msg{.id = 0x080, .len = 0};
  std::chrono::nanoseconds duration{static_cast<long>(1.0 / std::stod(argv[2]) * 1e9)};
  auto next = std::chrono::steady_clock::now();
  while (true) {
    chan.write(msg);
    std::this_thread::sleep_until(next);
    next += duration;
  }

  return 0;
}
