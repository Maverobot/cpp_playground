#include <iostream>

template <typename Func> class Defer {
public:
  Defer(Func &&func) : f_(std::forward<Func>(func)) {}

  ~Defer() { f_(); }

private:
  Func f_;
};

int main(int argc, char *argv[]) {

  auto say_world = [] { std::cout << " world\n"; };

  Defer d(std::move(say_world));

  std::cout << "hello";
  return 0;
}
