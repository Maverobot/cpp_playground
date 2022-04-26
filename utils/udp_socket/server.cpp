// Server side implementation of UDP client-server model
#include "data.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>

#define PORT 8080

// Driver code
int main() {
  int sockfd;
  char buf[0x0ffff];
  const char* hello = "Hello from server";
  struct sockaddr_in servaddr, cliaddr;

  // Creating socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  // Filling server information
  servaddr.sin_family = AF_INET;  // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(PORT);

  // Bind the socket with the server address
  if (bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  int len, n;
  len = sizeof(cliaddr);  // len is value/resuslt
  struct sockaddr sender;
  memset(&sender, 0, sizeof(struct sockaddr));
  socklen_t sender_len = sizeof(struct sockaddr);

  while (true) {
    n = recvfrom(sockfd, buf, sizeof(buf), MSG_WAITALL, &sender, &sender_len);

    const Data* data = reinterpret_cast<Data*>(buf);

    if (n != -1) {
      printf("Received %lu bytes: %.*s\n", sizeof(buf), static_cast<int>(sizeof(buf)), buf);
      printf("data.identifier = %s \n", std::string(data->connect.identifier).c_str());
      printf("data.version = %d \n", data->connect.version);
    }
  }
  return 0;
}
