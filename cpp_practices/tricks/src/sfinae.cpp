#include <iostream>
#include <type_traits>

template <typename T, typename... Ts>
constexpr auto is_in = (std::is_same_v<T, Ts> || ...);

template <typename T>
typename std::enable_if_t<is_in<T, int, double>> fun(T t) {
  std::cout << "fun: int/double" << std::endl;
}

template <typename T>
typename std::enable_if_t<is_in<T, float>> fun(T t) {
  std::cout << "fun: float" << std::endl;
}

int main(const int /* argc */, const char** /* argv */) {
  fun(1);
  fun(1.0f);
  fun(double());

  return 0;
}
