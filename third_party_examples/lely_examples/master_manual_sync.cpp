#include <iostream>
#include <lely/coapp/fiber_driver.hpp>
#include <lely/coapp/master.hpp>
#include <lely/ev/loop.hpp>
#include <lely/io2/linux/can.hpp>
#include <lely/io2/posix/poll.hpp>
#include <lely/io2/sys/timer.hpp>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <can-interface>" << std::endl;
    return 1;
  }

  // Create a I/O context and executor.
  lely::io::Context ctx;
  lely::io::Poll poll(ctx);
  lely::ev::Loop loop(poll.get_poll());
  auto exec = loop.get_executor();
  lely::io::Timer timer(poll, exec, CLOCK_MONOTONIC);

  // Create and open a CAN channel.
  lely::io::CanController ctrl(argv[1]);
  lely::io::CanChannel chan(poll, exec);
  chan.open(ctrl);

  // Create a CANopen Master.
  lely::canopen::AsyncMaster master(timer, chan, "master.dcf", "", 1);

  // Create a CAN frame for the SYNC message.
  can_msg msg{.id = 0x80, .len = 1, .data = {0x00}};
  // Send the SYNC message through the CAN channel.
  chan.write(msg);

  // Run the I/O context to process events.
  loop.run();

  return 0;
}
