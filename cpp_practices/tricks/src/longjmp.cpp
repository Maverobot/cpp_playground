#include <setjmp.h>
#include <stdio.h>

jmp_buf buffer;

void second_function() {
  printf("In second_function, preparing to jump back to first_function.\n");
  longjmp(buffer, 1);  // Jump back to where setjmp was called
}

void first_function() {
  if (setjmp(buffer) == 0) {
    // This code is executed after the initial setjmp call
    printf("In first_function, calling second_function.\n");
    second_function();  // This function will cause a jump back
  } else {
    // This code executes after longjmp is called
    printf("Back in first_function after the long jump.\n");
  }
}

int main() {
  printf("Starting main.\n");
  first_function();
  printf("Ending main.\n");
  return 0;
}
