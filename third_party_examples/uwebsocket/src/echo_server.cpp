/* We simply call the root header file "App.h", giving you uWS::App and uWS::SSLApp */
#include "App.h"

/* This is a simple WebSocket echo server example.
 * You may compile it with "WITH_OPENSSL=1 make" or with "make" */

int main() {
  /* ws->getUserData returns one of these */
  struct PerSocketData {
    /* Fill with user data */
  };

  /* Keep in mind that uWS::SSLApp({options}) is the same as uWS::App() when compiled without SSL
   * support. You may swap to using uWS:App() if you don't need SSL */
  uWS::App()
      .ws<PerSocketData>(
          "/*",
          {/* Settings */
           .compression = uWS::SHARED_COMPRESSOR,
           .maxPayloadLength = 16 * 1024 * 1024,
           .idleTimeout = 16,
           .maxBackpressure = 1 * 1024 * 1024,
           .closeOnBackpressureLimit = false,
           .resetIdleTimeoutOnSend = false,
           .sendPingsAutomatically = true,
           /* Handlers */
           .upgrade = nullptr,
           .open =
               [](auto* ws) {
                 /* Open event here, you may access ws->getUserData() which
                  * points to a PerSocketData struct */
               },
           .message =
               [](auto* ws, std::string_view message, uWS::OpCode opCode) {
                 std::cout << "received: " << message << "\n";
                 ws->subscribe("echo");
                 ws->publish("echo", message, uWS::OpCode::BINARY, true);
               },
           .drain =
               [](auto* ws) { std::cout << "buffered amount:" << ws->getBufferedAmount() << "\n"; },
           .ping =
               [](auto* /*ws*/, std::string_view) {
                 /* Not implemented yet */
               },
           .pong =
               [](auto* /*ws*/, std::string_view) {
                 /* Not implemented yet */
               },
           .close =
               [](auto* /*ws*/, int /*code*/, std::string_view /*message*/) {
                 /* You may access ws->getUserData() here */
               }})
      .listen(9001,
              [](auto* listen_socket) {
                if (listen_socket) {
                  std::cout << "Listening on port " << 9001 << std::endl;
                }
              })
      .run();
}
