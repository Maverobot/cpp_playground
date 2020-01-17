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
    unlink(socket_path);
  }

  if (bind(file_descriptor, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("bind error");
    exit(-1);
  }

  if (listen(file_descriptor, 5) == -1) {
    perror("listen error");
    exit(-1);
  }

  char buf[100];
  while (1) {
    int socket_descriptor;
    if ((socket_descriptor = accept(file_descriptor, NULL, NULL)) == -1) {
      perror("accept error");
      continue;
    }
    int read_count;
    while ((read_count = read(socket_descriptor, buf, sizeof(buf))) > 0) {
      printf("read %u bytes: %.*s\n", read_count, read_count, buf);
    }
    if (read_count == -1) {
      perror("read");
      exit(-1);
    } else if (read_count == 0) {
      printf("EOF\n");
      close(socket_descriptor);
    }
  }

  return 0;
}
