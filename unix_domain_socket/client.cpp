#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // const char *socket_path = "./socket";
  char const *socket_path = "\0hidden";

  if (argc > 1)
    socket_path = argv[1];

  int file_descriptor; // file descriptor
  if ((file_descriptor = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(-1);
  }

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  if (*socket_path == '\0') {
    *addr.sun_path = '\0';
    strncpy(addr.sun_path + 1, socket_path + 1, sizeof(addr.sun_path) - 2);
  } else {
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);
  }

  if (connect(file_descriptor, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("connect error");
    exit(-1);
  }

  char buf[100];
  int read_count;
  while ((read_count = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
    if (write(file_descriptor, buf, read_count) != read_count) {
      if (read_count > 0)
        fprintf(stderr, "partial write");
      else {
        perror("write error");
        exit(-1);
      }
    }
  }

  return 0;
}
