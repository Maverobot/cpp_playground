
// Client side implementation of UDP client-server model
#include "data.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <memory>
#include <string>

int main(int argc, char* argv[]) {
  const int port = 1234;

  /////////////////////////////////////////////////////////////////////////////
  //                              Receiver init                              //
  /////////////////////////////////////////////////////////////////////////////
  int recv_socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (recv_socket_fd == -1) {
    std::cerr << "Error opening UDP socket: " << strerror(errno) << std::endl;
    return 1;
  }
  struct sockaddr_in source;
  memset(&source, 0, sizeof(sockaddr_in));
  source.sin_family = AF_INET;
  source.sin_port = htons(port);
  inet_aton("localhost", &source.sin_addr);
  if (bind(recv_socket_fd, (struct sockaddr*)&source, sizeof(sockaddr_in)) == -1) {
    std::cerr << "Failed to bind\n";
    return 1;
  }

  /////////////////////////////////////////////////////////////////////////////
  //                                  Sender                                 //
  /////////////////////////////////////////////////////////////////////////////
  const std::string hostname = "127.0.0.1";
  const std::string identifier = "test_file_name";

  using defer_s = std::shared_ptr<void>;

  int send_socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  defer_s defer_closing_socket(nullptr, [send_socket_fd](auto) { close(send_socket_fd); });

  struct sockaddr_in destination;
  memset(&destination, 0, sizeof(sockaddr_in));
  destination.sin_family = AF_INET;
  destination.sin_port = htons(port);
  inet_aton(hostname.c_str(), &destination.sin_addr);

  Data* data = reinterpret_cast<Data*>(calloc(1, sizeof(Data)));
  defer_s defer_free_data(nullptr, [data](auto) { free(data); });

  data->message_type = MSG_START;

  if (identifier.size() > std::size(data->connect.identifier)) {
    std::cerr << "identifier too long\n";
    return 1;
  }

  data->connect.version = 2;
  std::cerr << "Send data with version " << data->connect.version << "\n";
  strcpy(data->connect.identifier, identifier.c_str());
  ssize_t result = sendto(send_socket_fd, data, sizeof(Data), 0, (struct sockaddr*)&destination,
                          sizeof(destination));
  if (result == -1) {
    std::cerr << "Failed to send\n";
  }

  /////////////////////////////////////////////////////////////////////////////
  //                               Receiver                                  //
  /////////////////////////////////////////////////////////////////////////////
  unsigned char receive_buffer[0xffff];  // Maximal UDP Datagram size.
  const Data* received_data = reinterpret_cast<const Data*>(receive_buffer);
  struct sockaddr sender;
  memset(&sender, 0, sizeof(struct sockaddr));
  socklen_t sender_len = sizeof(struct sockaddr);
  ssize_t n =
      recvfrom(recv_socket_fd, receive_buffer, sizeof(receive_buffer), 0, &sender, &sender_len);

  if (n != -1) {
    printf("Received %lu bytes: %.*s\n", sizeof(receive_buffer),
           static_cast<int>(sizeof(receive_buffer)), receive_buffer);
    printf("data.identifier = %s \n", std::string(data->connect.identifier).c_str());
    printf("data.version = %d \n", data->connect.version);
  }

  return 0;
}
