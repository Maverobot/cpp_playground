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
#include <string>

#define PORT 8080

// Driver code
int main() {
  int sockfd;
  char buffer[sizeof(Data)];
  Data d;
  d.connect.version = 7;
  strcpy(d.connect.identifier, std::string("file_name").c_str());
  memcpy(buffer, &d, sizeof(d));

  struct sockaddr_in servaddr;

  // Creating socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));

  // Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = INADDR_ANY;

  int n, len;

  sendto(sockfd, (const char*)buffer, sizeof(buffer), MSG_CONFIRM,
         (const struct sockaddr*)&servaddr, sizeof(servaddr));
  printf("message sent.\n");
  close(sockfd);
  return 0;
}
