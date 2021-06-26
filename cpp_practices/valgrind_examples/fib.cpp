#include <stdio.h>
#include <stdlib.h>

int fib(int x) {
  if (x == 0)
    return 0;
  else if (x == 1)
    return 1;
  return fib(x - 1) + fib(x - 2);
}

int main(int argc, char* argv[]) {
  for (size_t i = 0; i < 45; ++i) {
    printf("%d\n", fib(i));
  }
  return 0;
}
