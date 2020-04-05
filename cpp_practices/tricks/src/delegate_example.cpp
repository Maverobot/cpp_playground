#include <delegate.h>

#include <iostream>

struct A {
  void foo(int a) { std::cout << "method got: " << a << std::endl; }
};

void foo(int a) {
  std::cout << "function got: " << a << std::endl;
}

int main(int argc, char* argv[]) {
  auto d1(delegate<void(int)>::from<foo>());

  A a;
  auto d2(delegate<void(int)>::from<A, &A::foo>(&a));
  auto d3(delegate<void(int)>{foo});
  auto d4(delegate<void(int)>(&a, &A::foo));

  d1(1);
  d2(2);
  d3(3);
  d4(4);

  int b(2);

  auto dx(delegate<void()>([b]() { std::cout << "hello world: " << b << std::endl; }));

  dx();

  return 0;
}
