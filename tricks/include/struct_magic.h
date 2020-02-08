#pragma once

#include <array>   // std::array
#include <utility> // std::index_sequence, std::make_index_sequence

#include <boost/pfr/precise.hpp> // magic_get

namespace struct_magic {

namespace detail {

template <size_t idx, typename... Ts>
using get_nth_type = std::tuple_element_t<idx, std::tuple<Ts...>>;

template <typename... Conds> struct all_true : std::true_type {};
template <typename Cond, typename... Conds>
struct all_true<Cond, Conds...>
    : std::conditional<Cond::value, all_true<Conds...>, std::false_type>::type {
};

template <typename... Ts>
struct all_same : all_true<std::is_same<get_nth_type<0, Ts...>, Ts>...> {};

template <size_t index, typename Func, typename... Ts>
auto transform(Func f, Ts &&... input) {
  return f(boost::pfr::get<index>(std::forward<Ts>(input))...);
}

template <typename Func, size_t... indices, typename... Ts>
auto transform(Func f, std::index_sequence<indices...>, Ts &&... input) {
  return std::decay_t<get_nth_type<0, Ts...>>{
      transform<indices>(f, std::forward<Ts>(input)...)...};
}
} // namespace detail

// Execute function which transforms a "list" of tuples into one tuple
template <typename Func, typename... Ts,
          typename = std::enable_if_t<detail::all_same<Ts...>::value, int>>
auto transform(Func f, Ts &&... input) {
  return detail::transform(
      f,
      std::make_index_sequence<boost::pfr::tuple_size<
          std::decay_t<detail::get_nth_type<0, Ts...>>>::value>(),
      std::forward<Ts>(input)...);
}

} // namespace struct_magic
