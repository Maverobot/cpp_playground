#include <iostream>
#include <memory>

template <typename Func> class Defer {
public:
  Defer(Func &&func) : f_(std::forward<Func>(func)) {}

  ~Defer() { f_(); }

private:
  Func f_;
};

using defer_s = std::shared_ptr<void>;

int main(int /*argc*/, char ** /*argv[]*/) {

  defer_s _s(nullptr, [](auto) { std::cout << "Do defer with shared_ptr\n"; });

  auto say_world = [] { std::cout << " world\n"; };

  Defer d(std::move(say_world));

  std::cout << "hello";
  return 0;
}
