#include <iostream>

[[nodiscard]] bool funcWithNoDiscard() {
  return true;
}

__attribute__((warn_unused_result)) bool funcWithUnusedResultWarning() {
  return true;
}

[[noreturn]] void infiniteLoop() {
  while (true) {
  }
}

[[deprecated("This function is too old...")]] void oldFunc() {}

int main(int /*argc*/, char* /*argv*/[]) {
  funcWithNoDiscard();
  funcWithUnusedResultWarning();

  // gcc/clang does not give warning about unreachable code here
  infiniteLoop();
  std::cout << "No! That's impossible" << std::endl;

  oldFunc();

  return 0;
}
