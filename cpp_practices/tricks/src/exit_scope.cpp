#include <scope_action.h>

#include <functional>
#include <iostream>

int main(int argc, char* argv[]) {
  try {
    SCOPE_FAIL { std::cerr << "scope_fail: exception thrown in scope\n"; };
    SCOPE_EXIT { std::cout << "scope_exit: 1\n"; };
    SCOPE_EXIT { std::cout << "scope_exit: 2\n"; };
    throw std::runtime_error("exception for testing purpose");
    std::cout << "scope end\n";
  } catch (...) {
    std::cout << "caught exception\n";
  }
  return 0;
}
