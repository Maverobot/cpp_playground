#include <iostream>
#include <type_traits>

template <typename T = void, typename U = void>
struct Foo {
  Foo() { std::cout << "primary\n"; }
};

template <typename T>
struct Foo<T,
           typename std::enable_if_t<std::is_same_v<T, int> || std::is_same_v<T, double>, void>> {
  Foo() { std::cout << "specialized\n"; }
};

int main(int argc, char* argv[]) {
  Foo f{};
  Foo<float> f_f{};
  Foo<int> f_i{};
  Foo<double> f_d{};
  return 0;
}
