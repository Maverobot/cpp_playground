#include <cmath>
#include <cstddef>
#include <utility>

auto get_polynomial_functor() {
  return [=](double x) -> double { return 0; };
}

template <typename T,
          typename... Args,
          typename = std::enable_if<(std::is_convertible<Args, T>::value && ...)>>
auto get_polynomial_functor(T first, Args... args) {
  constexpr size_t num_args = sizeof...(args);
  return [=](double x) -> T {
    return first * std::pow(x, num_args) + get_polynomial_functor(args...)(x);
  };
}

template <typename T,
          typename... Args,
          typename = std::enable_if<(std::is_convertible<Args, T>::value && ...)>>
auto get_polynomial_functor_derivative(T first, Args... args) {
  constexpr size_t num_args = sizeof...(args);
  if constexpr (num_args == 0) {
    return [=](double x) -> T { return T(); };
  } else {
    return [=](double x) -> T {
      return first * std::pow(x, num_args - 1) + get_polynomial_functor_derivative(args...)(x);
    };
  }
}
