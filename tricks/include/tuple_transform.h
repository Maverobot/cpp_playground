#pragma once

#include <tuple>

namespace tuple_utils {
namespace detail {
template <size_t idx, typename... Ts>
using get_nth_type = std::tuple_element_t<idx, std::tuple<Ts...>>;

template <size_t index, typename Func, typename... Ts>
auto transform(Func f, Ts &&... input) {
  return f(std::get<index>(std::forward<Ts>(input))...);
}

template <typename Func, size_t... indices, typename... Ts>
auto transform(Func f, std::index_sequence<indices...>, Ts &&... input) {
  return std::make_tuple(transform<indices>(f, input...)...);
}

} // namespace detail

// Execute function which transforms a "list" of tuples into one tuple
template <typename Func, typename... Ts>
auto transform(Func f, Ts &&... input) {
  return detail::transform(
      f,
      std::make_index_sequence<
          std::tuple_size<detail::get_nth_type<0, Ts...>>::value>(),
      std::forward<Ts>(input)...);
}
} // namespace tuple_utils
