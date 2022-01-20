#include "App.h"

int main() {
  /* Overly simple hello world app */
  uWS::App()
      .get("/*", [](auto* res, auto* /*req*/) { res->end("Hello world!"); })
      .listen(3000,
              [](auto* listen_socket) {
                if (listen_socket) {
                  std::cout << "Listening on port " << 3000 << std::endl;
                }
              })
      .run();

  std::cout << "Failed to listen on port 3000" << std::endl;
}
