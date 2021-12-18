#include <stdio.h>
#include <string>

#include <hello-tp.h>

void addTrace(const std::string& message) {
  tracepoint(hello_world, my_first_tracepoint, const_cast<char*>(message.c_str()));
}

int main(int argc, char* argv[]) {
  int x;

  puts("Hello, World!\nPress Enter to continue...");

  /*
   * The following getchar() call is only placed here for the purpose
   * of this demonstration, to pause the application in order for
   * you to have time to list its tracepoints. It's not needed
   * otherwise.
   */
  getchar();

  /*
   * A tracepoint() call.
   *
   * Arguments, as defined in hello-tp.h:
   *
   * 1. Tracepoint provider name   (required)
   * 2. Tracepoint name            (required)
   * 3. my_integer_arg             (first user-defined argument)
   * 4. my_string_arg              (second user-defined argument)
   *
   * Notice the tracepoint provider and tracepoint names are
   * NOT strings: they are in fact parts of variables that the
   * macros in hello-tp.h create.
   */
  addTrace("hi there!");

  for (x = 0; x < argc; ++x) {
    addTrace(argv[x]);
  }

  puts("Quitting now!");
  addTrace("x^2");

  return 0;
}
