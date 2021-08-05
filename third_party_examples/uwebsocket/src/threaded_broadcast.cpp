/*
   This example demonstrates basic idea of handling multi-threaded uWebSockets sever.
   There may be better ways to capture and use uWS::Loop and uWS::App objects.
*/

#include <memory>
#include <sstream>
#include <thread>
#include <vector>
#include "App.h"

#include "date.h"
#include "trim.h"

/* ws->getUserData returns one of these */
struct PerSocketData {
  std::string_view user_secure_token;
};

/* uWebSocket broadcaster runs in a separate thread */
class ThreadedBroadcaster {
 public:
  ThreadedBroadcaster() {
    thread_ = std::make_shared<std::thread>([this]() {
      /* create uWebSocket broadcaster and capture uWS::Loop, uWS::App objects. */
      this->work();
    });
  }

  ~ThreadedBroadcaster() { thread_->join(); }

  bool finished() const { return finished_; }

  [[nodiscard]] bool publish(std::string topic,
                             std::string message,
                             uWS::OpCode opCode,
                             bool compress = false) {
    if (loop_) {
      loop_->defer([=, this]() { app_->publish(topic, message, opCode); });
      return true;
    }
    return false;
  };

 private:
  /* Every thread has its own Loop, and uWS::Loop::get() returns the Loop for current thread.*/
  struct uWS::Loop* loop_;

  /* Need to capture the uWS::App object (instance). */
  std::shared_ptr<uWS::App> app_;

  /* uWebSocket broadcaster listens on separate port, or share the same port (works on Linux). */
  struct us_listen_socket_t* listen_socket_;

  /* Thread object for uWebSocket broadcaster */
  std::shared_ptr<std::thread> thread_;

  std::atomic<bool> finished_{false};

  void work();
};

/* uWebSocket broadcaster thread function. */
void ThreadedBroadcaster::work() {
  /* Every thread has its own Loop, and uWS::Loop::get() returns the Loop for current thread.*/
  loop_ = uWS::Loop::get();

  /* uWS::App object / instance is used in uWS::Loop::defer(lambda_function) */
  app_ = std::make_shared<uWS::App>();

  /* Very simple WebSocket broadcasting echo server */
  app_->ws<PerSocketData>("/*",
                          {/* Settings */
                           .compression = uWS::SHARED_COMPRESSOR,
                           .maxPayloadLength = 16 * 1024 * 1024,
                           .idleTimeout = 0,
                           .maxBackpressure = 1 * 1024 * 1204,
                           /* Handlers */
                           .open = [](auto* ws) { ws->subscribe("broadcast"); },
                           .message =
                               [this](auto* ws, std::string_view message, uWS::OpCode opCode) {
                                 /* Exit gracefully if we get a closedown message (ASAN debug) */
                                 if (trim(message) == "closedown") {
                                   /* Bye bye */
                                   us_listen_socket_close(0, listen_socket_);
                                   ws->close();
                                 }
                                 /* Simple echo the message to the client. */
                                 else {
                                   ws->send(message, opCode);
                                 }
                               },
                           .drain =
                               [](auto* ws) {
                                 /* Check getBufferedAmount here */
                               },
                           .ping =
                               [](auto* /*ws*/, std::string_view) {
                                 /* Not implemented yet */
                               },
                           .pong =
                               [](auto* /*ws*/, std::string_view) {
                                 /* Not implemented yet */
                               },
                           .close =
                               [](auto* ws, int code, std::string_view message) {
                                 /* We automatically unsubscribe from any topic here */
                               }})
      .listen(9001,
              [this](auto* token) {
                listen_socket_ = token;
                if (listen_socket_) {
                  std::cout << "Listening on port " << 9001 << std::endl;
                } else {
                  std::cout << "Failed to listen on port " << 9001 << std::endl;
                }
              })
      .run();
  finished_ = true;
}

/* Main */
int main() {
  /* uWebSocket workers. */
  ThreadedBroadcaster broadcaster;

  /**Loop::defer(function) is the only thread-safe function here; you use it to defer
   * the calling of a function to the thread that runs the Loop. Use this to signal
   * every Loop that it should publish some message.
   */
  while (!broadcaster.finished()) {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1ns);
    /* uWs::App of each broadcaster. uWs::App object has knowledge of
     * all socket contexts. */
    std::ostringstream oss;
    using namespace date;
    oss << std::chrono::system_clock::now();
    if (!broadcaster.publish("broadcast", oss.str(), uWS::OpCode::BINARY)) {
      std::cout << "Broadcaster is not ready... Message discarded.\n";
    }
  }

  std::cout << "Waiting for threads to join...\n";
  return 0;
}
