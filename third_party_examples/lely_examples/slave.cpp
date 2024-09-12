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
#include <lely/coapp/fiber_driver.hpp>
#include <lely/coapp/slave.hpp>
#include <lely/io2/sys/io.hpp>
#include <lely/io2/sys/sigset.hpp>
#include <lely/io2/sys/timer.hpp>

#if _WIN32
#include <thread>
#endif

#include <iostream>

#include "syslog_diag_handler.h"

using namespace lely;

class MySlave : canopen::detail::FiberDriverBase, public canopen::BasicSlave {
 public:
  using BasicSlave::BasicSlave;

  template <typename... Args>
  explicit MySlave(ev_exec_t* exec, Args&&... args)
      : FiberDriverBase(exec), BasicSlave(FiberDriverBase::exec, std::forward<Args>(args)...) {
    repeatedly_execute();
  }

 protected:
  void wait(lely::canopen::SdoFuture<void> f) {
    fiber_await(f);
    try {
      return f.get().value();
    } catch (const ev::future_not_ready& e) {
      util::throw_error_code("Wait", ::std::errc::operation_canceled);
    }
  }

  void repeatedly_execute() {
    strand.defer([this]() {
      // printf("Execute repeatedly\n");

      // Trigger the next execution every 1 second.
      wait(AsyncWait(std::chrono::seconds(1)));

      repeatedly_execute();
    });
  }

  // This function gets called every time a value is written to the local object
  // dictionary by an SDO or RPDO.
  void OnWrite(uint16_t idx, uint8_t subidx) noexcept override {
    if (idx == 0x4000 && subidx == 0) {
      // Read the value just written to object 4000:00, probably by RPDO 1.
      uint32_t val = (*this)[0x4000][0];
      std::cout << "Received value: " << val << std::endl;
      // Copy it to object 4001:00, so that it will be sent by the next TPDO.
      (*this)[0x4001][0] = val;
    }
  }
};

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <can-interface>" << std::endl;
  }

  // Initialize the I/O library. This is required on Windows, but a no-op on
  // Linux (for now).
  io::IoGuard io_guard;
#if _WIN32
  // Load vcinpl2.dll (or vcinpl.dll if CAN FD is disabled).
  io::IxxatGuard ixxat_guard;
#endif
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
  // Create a timer using a monotonic clock, i.e., a clock that is not affected
  // by discontinuous jumps in the system time.
  io::Timer timer(poll, exec, CLOCK_MONOTONIC);

  set_lely_diag_handler_with_syslog();
#if _WIN32
  // Create an IXXAT CAN controller and channel. The VCI requires us to
  // explicitly specify the bitrate and restart the controller.
  io::IxxatController ctrl(0, 0, io::CanBusFlag::NONE, 125000);
  ctrl.restart();
  io::IxxatChannel chan(ctx, exec);
#elif defined(__linux__)
  // Create a virtual SocketCAN CAN controller and channel, and do not modify
  // the current CAN bus state or bitrate.
  io::CanController ctrl(argv[1]);
  io::CanChannel chan(poll, exec);
#endif
  chan.open(ctrl);

  // Create a CANopen slave with node-ID 2.
  MySlave slave(exec, timer, chan, "cpp-slave.eds", "", 1);

  // Create a signal handler.
  io::SignalSet sigset(poll, exec);
  // Watch for Ctrl+C or process termination.
  sigset.insert(SIGHUP);
  sigset.insert(SIGINT);
  sigset.insert(SIGTERM);

  // Submit a task to be executed when a signal is raised. We don't care which.
  sigset.submit_wait([&](int /*signo*/) {
    // If the signal is raised again, terminate immediately.
    sigset.clear();
    // Perform a clean shutdown.
    ctx.shutdown();
  });

  // Start the NMT service of the slave by pretending to receive a 'reset node'
  // command.
  slave.Reset();

#if _WIN32
  // Create two worker threads to ensure the blocking canChannelReadMessage()
  // and canChannelSendMessage() used by the IXXAT CAN channel do not hold up
  // the event loop.
  std::thread workers[] = {std::thread([&]() { loop.run(); }), std::thread([&]() { loop.run(); })};
#endif

  // Run the event loop until no tasks remain (or the I/O context is shut down).
  loop.run();

#if _WIN32
  // Wait for the worker threads to finish.
  for (auto& worker : workers)
    worker.join();
#endif

  return 0;
}
