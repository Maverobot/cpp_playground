#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// This is a simple TCP server that can handle multiple clients at the same time.
// Modified from the book "Hands-On Network Programming with C" by Lewis Van Winkle.
int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr, "usage: tcp_server hostname port\n");
    exit(1);
  }

  printf("Configuring local address...\n");
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  struct addrinfo* bind_address;
  getaddrinfo(argv[1], argv[2], &hints, &bind_address);

  printf("Creating socket...\n");
  int socket_listen =
      socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
  if (socket_listen < 0) {
    fprintf(stderr, "socket() failed. (%d)\n", errno);
    exit(1);
  }

  printf("Binding socket to local address...\n");
  const int yes = 1;
  if (setsockopt(socket_listen, SOL_SOCKET, SO_REUSEADDR, (void*)&yes, sizeof(yes)) < 0) {
    fprintf(stderr, "setsockopt() failed. (%d)\n", errno);
  }
  if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
    fprintf(stderr, "bind() failed. (%d)\n", errno);
    exit(1);
  }

  freeaddrinfo(bind_address);
  printf("Listening...\n");

  if (listen(socket_listen, 10) <
      0) {  // 10 is the maximum length of the queue of pending connections.
    fprintf(stderr, "listen() failed. (%d)\n", errno);
    exit(1);
  }

  fd_set master;
  FD_ZERO(&master);
  FD_SET(socket_listen, &master);
  int max_socket = socket_listen;

  printf("Waiting for connections...\n");
  while (1) {
    fd_set reads;
    reads = master;
    if (select(max_socket + 1, &reads, NULL, NULL, NULL) < 0) {
      fprintf(stderr, "select() failed. (%d)\n", errno);
      exit(1);
    }

    for (int i = 1; i <= max_socket; i++) {
      if (FD_ISSET(i, &reads)) {
        if (i == socket_listen) {
          // Handle new connections.
          struct sockaddr_storage client_address;
          socklen_t client_len = sizeof(client_address);
          int socket_client = accept(socket_listen, (struct sockaddr*)&client_address, &client_len);
          if (socket_client < 0) {
            fprintf(stderr, "accept() failed. (%d)\n", errno);
            exit(1);
          }

          FD_SET(socket_client, &master);
          if (socket_client > max_socket) {
            max_socket = socket_client;
          }

          char address_buffer[100];
          getnameinfo((struct sockaddr*)&client_address, client_len, address_buffer,
                      sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
          printf("New connection from: %s\n", address_buffer);
        } else {
          // Handle data from a client.
          char read[1024];
          int bytes_received = recv(i, read, sizeof(read), 0);
          if (bytes_received < 1) {
            FD_CLR(i, &master);
            close(i);
            continue;
          }

          for (int j = 1; j <= max_socket; j++) {
            if (FD_ISSET(j, &master)) {
              // Send to everyone except the listener and the sender.
              if (j == socket_listen || j == i) {
                continue;
              }
              send(j, read, bytes_received, 0);
            }
          }

          read[bytes_received] = '\0';
          printf("Received: %s\n", read);
        }
      }
    }
  }

  return 0;
}
