#include <cxxabi.h>
#include <iostream>
#include <memory>
#include <tuple>

inline std::string demangle(const char* name) {
  int status = -4;  // some arbitrary value to eliminate the compiler warning
  // enable c++11 by passing the flag -std=c++11 to g++
  std::unique_ptr<char, void (*)(void*)> res{abi::__cxa_demangle(name, NULL, NULL, &status),
                                             std::free};
  return (status == 0) ? res.get() : name;
}
template <class T>
std::string name(T&&) {
  return demangle(typeid(std::decay_t<T>).name());
}
template <class T>
std::string name() {
  return demangle(typeid(std::decay_t<T>).name());
}

template <typename T>
struct flatten_tuple_helper {
  using type = std::tuple<T>;
};
template <typename T>
using flatten_tuple_helper_t = typename flatten_tuple_helper<T>::type;
template <typename... Ts>
struct flatten_tuple_helper<std::tuple<Ts...>> {
  using type = decltype(std::tuple_cat(std::declval<flatten_tuple_helper_t<Ts>>()...));
};
template <typename T>
struct flatten_tuple {
  using type = T;
};
template <typename... Ts>
struct flatten_tuple<std::tuple<Ts...>> {
  using type = flatten_tuple_helper_t<std::tuple<Ts...>>;
};
template <typename T>
using flatten_tuple_t = typename flatten_tuple<T>::type;

struct C1 {};
struct C2 {};
struct C3 {};
struct C4 {};
struct C5 {};
struct C6 {};

int main(int argc, char* argv[]) {
  static_assert(
      std::is_same_v<flatten_tuple_t<std::tuple<C6, C5, std::tuple<C4, std::tuple<C3>, C2>, C1>>,
                     std::tuple<C6, C5, C4, C3, C2, C1>>);
  static_assert(std::is_same_v<flatten_tuple_t<std::tuple<std::tuple<std::tuple<C3>, C2>, C1>>,
                               std::tuple<C3, C2, C1>>);
  static_assert(
      std::is_same_v<flatten_tuple_t<std::tuple<std::tuple<std::tuple<C5, C4>, C3>, C2, C1>>,
                     std::tuple<C5, C4, C3, C2, C1>>);
  static_assert(
      std::is_same_v<flatten_tuple_t<std::tuple<
                         std::tuple<std::tuple<std::tuple<std::tuple<C6, C5>, C4>, C3>, C2>, C1>>,
                     std::tuple<C6, C5, C4, C3, C2, C1>>);
  return 0;
}
