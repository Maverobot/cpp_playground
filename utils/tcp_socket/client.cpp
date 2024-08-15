#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <functional>
#include <string>
#include <thread>

class TCPSocketClient {
 public:
  TCPSocketClient(std::string hostname,
                  uint16_t port,
                  std::function<void(void* message, size_t message_length)> on_message_received)
      : addr_info_{nullptr, freeaddrinfo}, on_message_received_{on_message_received} {
    printf("Configuring remote address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo* peer_address;
    getaddrinfo(hostname.c_str(), std::to_string(port).c_str(), &hints,
                &peer_address);  // For easy switching between IPv4 and IPv6
    this->addr_info_.reset(peer_address);
    printf("Rmote address is: ");
    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen, address_buffer,
                sizeof(address_buffer), service_buffer, sizeof(service_buffer),
                NI_NUMERICHOST | NI_NUMERICSERV);
    printf("%s %s\n", address_buffer, service_buffer);

    connect();
  }

  ~TCPSocketClient() {
    printf("Closing receive thread...\n");
    running_ = false;
    receive_thread_.join();
    printf("Closing socket...\n");
    close(socket_peer_);
    printf("Finished.\n");
  }

  bool connect() {
    printf("Creating socket...\n");
    socket_peer_ = socket(this->addr_info_->ai_family, this->addr_info_->ai_socktype,
                          this->addr_info_->ai_protocol);
    if (socket_peer_ < 0) {
      fprintf(stderr, "socket() failed. (%d)\n", socket_peer_);
      return false;
    }

    printf("Connecting...\n");
    if (::connect(socket_peer_, this->addr_info_->ai_addr, this->addr_info_->ai_addrlen)) {
      fprintf(stderr, "Failed. (%s)\n", strerror(errno));
      return false;
    }
    if (receive_thread_.joinable()) {
      receive_thread_.join();
    }
    receive_thread_ = std::thread(&TCPSocketClient::receive_message, this);
    connected_ = true;
    return true;
  }

  void send_message(const void* message, ssize_t message_length) {
    if (!this->connected_) {
      if (!this->connect()) {
        return;
      }
    }
    int bytes_sent = send(socket_peer_, message, message_length, 0);
    printf("Sent %d bytes.\n", bytes_sent);
  }

  void receive_message() {
    while (running_) {
      fd_set reads;
      FD_ZERO(&reads);
      FD_SET(socket_peer_, &reads);
      FD_SET(0, &reads);

      struct timeval timeout;
      timeout.tv_sec = 0;
      timeout.tv_usec = 100000;

      if (select(socket_peer_ + 1, &reads, 0, 0, &timeout) < 0) {
        fprintf(stderr, "select() failed. (%d)\n", socket_peer_);
        return;
      }

      if (FD_ISSET(socket_peer_, &reads)) {
        // Socket is readable
        char read[4096];
        int bytes_received = recv(socket_peer_, read, 4096, 0);
        if (bytes_received < 1) {
          printf("Connection closed by peer.\n");
          connected_ = false;
          break;
        }
        printf("Received: %.*s", bytes_received, read);
        on_message_received_(read, bytes_received);
      }
    }
  }

 private:
  int socket_peer_;
  std::unique_ptr<struct addrinfo, decltype(&freeaddrinfo)> addr_info_;
  std::function<void(void* message, size_t message_length)> on_message_received_;
  std::thread receive_thread_;
  std::atomic<bool> running_{true};
  std::atomic<bool> connected_{false};
};

// This is a simple TCP client that connects to a server and sends and receives data.
// Modified from the book "Hands-On Network Programming with C" by Lewis Van Winkle.
int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr, "usage: tcp_client hostname port\n");
    return 1;
  }

  char* hostname = argv[1];
  uint16_t port = atoi(argv[2]);

  TCPSocketClient client(hostname, port, [](void* message, size_t message_length) {
    printf("Received: %.*s", (int)message_length, (char*)message);
  });

  while (true) {
    printf("Enter a message: ");
    char message[4096];
    fgets(message, 4096, stdin);
    if (strcmp(message, "exit\n") == 0) {
      break;
    }
    client.send_message(message, strlen(message));
  }

  return 0;
}
