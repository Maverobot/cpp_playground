#include "unix_domain_socket.h"

#include <iostream>

int main(int argc, char *argv[]) {
  // const char *socket_path = "./socket";
  char const *socket_path = "\0hidden";

  if (argc > 1)
    socket_path = argv[1];

  uds::UnixDomainSocketClient client(socket_path);

  std::cout << "Connected to server.\nType here and press enter:" << std::endl;

  while (1) {
    std::string msg;
    std::getline(std::cin, msg);
    client.send(msg.c_str());
  }

  return 0;
}
