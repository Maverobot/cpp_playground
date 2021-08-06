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

/* uWebSocket worker runs in a separate thread */
struct worker_t {
  void work();

  /* uWebSocket worker listens on separate port, or share the same port (works on Linux). */
  struct us_listen_socket_t* listen_socket_;

  /* Every thread has its own Loop, and uWS::Loop::get() returns the Loop for current thread.*/
  struct uWS::Loop* loop_;

  /* Need to capture the uWS::App object (instance). */
  std::shared_ptr<uWS::App> app_;

  /* Thread object for uWebSocket worker */
  std::shared_ptr<std::thread> thread_;

  bool running_ = false;
};

/* uWebSocket worker thread function. */
void worker_t::work() {
  running_ = true;

  /* Every thread has its own Loop, and uWS::Loop::get() returns the Loop for current thread.*/
  loop_ = uWS::Loop::get();

  /* uWS::App object / instance is used in uWS::Loop::defer(lambda_function) */
  app_ = std::make_shared<uWS::App>();

  /* Very simple WebSocket broadcasting echo server */
  app_->ws<PerSocketData>(
          "/*",
          {/* Settings */
           .compression = uWS::SHARED_COMPRESSOR,
           .maxPayloadLength = 16 * 1024 * 1024,
           .idleTimeout = 0,
           .maxBackpressure = 1 * 1024 * 1204,
           /* Handlers */
           // Refer to  6735bad  commit. Don't pass req in open handler
           // https://github.com/uNetworking/uWebSockets/commit/625efbc499460a21d27c5811ce7c948a69a04eea
           .open =
               [](auto* ws) {
                 /* Let's make every connection subscribe to the "broadcast" topic.
                    Since our example server runs multiple worker threads, the client connection
                    may be served by any of the worker thread.
                    However, this example demonstrates publishing message(s) to all subscribers
                    served by all workers (threads).
                 */
                 ws->subscribe("broadcast");
               },
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
                  std::cout << "Thread " << std::this_thread::get_id() << " listening on port "
                            << 9001 << std::endl;
                } else {
                  std::cout << "Thread " << std::this_thread::get_id()
                            << " failed to listen on port " << 9001 << std::endl;
                }
              })
      .run();

  std::cout << "Thread " << std::this_thread::get_id() << " exiting" << std::endl;
  running_ = false;
}

/* Main */
int main() {
  /* uWebSocket workers. */
  std::vector<worker_t> workers;
  workers.resize(std::thread::hardware_concurrency());

  std::transform(workers.begin(), workers.end(), workers.begin(), [](worker_t& w) {
    w.thread_ = std::make_shared<std::thread>([&w]() {
      /* create uWebSocket worker and capture uWS::Loop, uWS::App objects. */
      w.work();
    });
    return w;
  });

  /**Loop::defer(function) is the only thread-safe function here; you use it to defer
   * the calling of a function to the thread that runs the Loop. Use this to signal
   * every Loop that it should publish some message.
   */
  while (
      std::any_of(workers.cbegin(), workers.cend(), [](const worker_t& w) { return w.running_; })) {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
    std::for_each(workers.begin(), workers.end(), [](worker_t& w) {
      /* uWs::Loop of each worker. */
      if (w.loop_) {
        w.loop_->defer([&w]() {
          /* uWs::App of each worker. uWs::App object has knowledge of
           * all socket contexts. */
          std::ostringstream oss;
          using namespace date;
          oss << std::chrono::system_clock::now();
          w.app_->publish("broadcast", oss.str(), uWS::OpCode::BINARY);
        });
      }
    });
  }

  std::for_each(workers.begin(), workers.end(), [](worker_t& w) { w.thread_->join(); });

  return 0;
}
