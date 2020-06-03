#include <iostream>
#include <type_traits>

template <typename Lambda>
struct is_valid_helper {
  template <typename... LambdaArgs>
  constexpr auto test(int) const
      -> decltype(std::declval<Lambda>()(std::declval<LambdaArgs>()...), std::true_type()) {
    return std::true_type();
  }

  template <typename... LambdaArgs>
  constexpr auto test(...) const -> std::false_type {
    return std::false_type();
  }

  template <typename... LambdaArgs>
  constexpr auto operator()(const LambdaArgs&...) const {
    return this->test<LambdaArgs...>(0);
  }
};

template <typename Lambda>
constexpr auto is_valid(const Lambda&) {
  return is_valid_helper<Lambda>();
}

struct A {};

int main(int argc, char* argv[]) {
  constexpr auto is_addable = is_valid([](auto&& x) -> decltype(x + x) {});
  constexpr auto is_pointer = is_valid([](auto&& x) -> decltype(*x) {});

  const int x = 5;
  int* y;

  std::cout << "Is int addable? " << is_addable(x) << "\n";
  std::cout << "Is int* pointer? " << is_pointer(y) << "\n";
  std::cout << "Is class A addable? " << is_addable(A()) << "\n";
  std::cout << "Is class A pointer? " << is_pointer(A()) << "\n";

  return 0;
}
