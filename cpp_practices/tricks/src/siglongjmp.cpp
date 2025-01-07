#include <setjmp.h>
#include <signal.h>
#include <stdio.h>

sigjmp_buf jump_buffer;

void error_handler(int sig) {
  printf("An error occurred! Signal: %d\n", sig);
  siglongjmp(jump_buffer, 1);  // Jump back to the point saved by sigsetjmp.
}

int main() {
  // Set up a signal handler for demonstration (e.g., division by zero).
  signal(SIGFPE, error_handler);  // Handle floating-point exception (e.g., division by zero).

  if (sigsetjmp(jump_buffer, 1) == 0) {
    // Normal flow of execution.
    printf("Starting main calculation...\n");

    int result = 5 / 0;              // This will cause a division by zero.
    printf("Result: %d\n", result);  // This line will not be executed.

  } else {
    // This block is executed after siglongjmp is called.
    printf("Recovered from an error, program continues...\n");
  }

  printf("Program continues executing after handling the error...\n");
  return 0;
}
