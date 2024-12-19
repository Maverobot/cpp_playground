#include <iostream>

// A simple function that can take both lvalues and rvalues
void process(int& x) {
  std::cout << "Lvalue reference called with: " << x << '\n';
}

void process(int&& x) {
  std::cout << "Rvalue reference called with: " << x << '\n';
}

// A template function test that performs perfect forwarding
template <typename T>
void forwarder(T&& arg) {
  // Forward the argument to another function
  process(std::forward<T>(arg));
}

int main() {
  int a = 10;
  forwarder(a);   // Calls `process(int&)` with lvalue
  forwarder(20);  // Calls `process(int&&)` with rvalue
}
