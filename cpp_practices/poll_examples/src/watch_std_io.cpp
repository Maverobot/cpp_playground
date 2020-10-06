#include <stdio.h>
#include <sys/poll.h>
#include <unistd.h>

#define TIMEOUT 5

int main(void) {
  struct pollfd fds[] = {// watch stdin for input
                         {.fd = STDIN_FILENO, .events = POLLIN, .revents = 0},
                         // watch stdout for ability to write
                         {.fd = STDIN_FILENO, .events = POLLOUT, .revents = 0}};
  int ret;

  ret = poll(fds, 2, TIMEOUT * 1000);

  if (ret == -1) {
    perror("poll");
    return 1;
  }

  if (!ret) {
    printf("%d seconds elapsed.\n", TIMEOUT);
    return 0;
  }

  if (fds[1].revents & POLLOUT) {
    printf("stdout is writable\n");
  }

  if (fds[0].revents & POLLIN) {
    constexpr int kBufferSize = 10;
    char buffer[kBufferSize];
    read(STDIN_FILENO, buffer, kBufferSize);
    printf("stdin is readable. The first %d chars are: %.10s\n", kBufferSize, buffer);
  }

  return 0;
}
