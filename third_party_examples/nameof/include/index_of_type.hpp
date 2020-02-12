#pragma once

#include <tuple>

// Index of type in tuple
template <class T, class Tuple> struct Index;
template <class T, class... Types> struct Index<T, std::tuple<T, Types...>> {
  static constexpr std::size_t value = 0;
};
template <class T, class U, class... Types>
struct Index<T, std::tuple<U, Types...>> {
  static constexpr std::size_t value =
      1 + Index<T, std::tuple<Types...>>::value;
};
template <typename T, typename Tuple>
constexpr int idx = Index<T, Tuple>::value;
