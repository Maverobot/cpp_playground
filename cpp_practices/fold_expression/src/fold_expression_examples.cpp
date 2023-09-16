#include <iostream>
template <typename... Args>
auto sum(const Args&... values) {
  static_assert(sizeof...(Args) > 0, "function sum must take at least 1 arguments.");
  return (values + ...);
}

int main(int argc, char* argv[]) {
  // std::cout << "sum(): " << sum() << "\n"; // This would fail the static assert
  std::cout << "sum(1): " << sum(1) << "\n";
  std::cout << "sum(1, 3, 9.0): " << sum(1, 3, 9.0) << "\n";

  return 0;
}
