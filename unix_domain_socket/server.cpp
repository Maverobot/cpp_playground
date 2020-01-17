#include "unix_domain_socket.h"

#include <iostream>

int main(int argc, char *argv[]) {
  // const char *socket_path = "./socket";
  char const *socket_path = "\0hidden";

  if (argc > 1)
    socket_path = argv[1];

  uds::UnixDomainSocketServer server(socket_path);

  while (1) {
    server.accept();
  }

  return 0;
}
