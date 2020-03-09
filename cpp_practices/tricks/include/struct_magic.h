#pragma once

#include <array>    // std::array
#include <utility>  // std::index_sequence, std::make_index_sequence

#include <boost/pfr/precise.hpp>  // magic_get

namespace struct_magic {

namespace detail {

template <size_t idx, typename... Ts>
using get_nth_type = std::tuple_element_t<idx, std::tuple<Ts...>>;

template <typename... Conds>
struct all_true : std::true_type {};
template <typename Cond, typename... Conds>
struct all_true<Cond, Conds...>
    : std::conditional<Cond::value, all_true<Conds...>, std::false_type>::type {};

template <typename... Ts>
struct all_same : all_true<std::is_same<get_nth_type<0, Ts...>, Ts>...> {};

template <size_t index, typename Func, typename... Ts>
auto transform(Func&& f, Ts&&... input) {
  return std::forward<Func>(f)(boost::pfr::get<index>(std::forward<Ts>(input))...);
}

template <typename Func, size_t... indices, typename... Ts>
auto transform(Func&& f, std::index_sequence<indices...>, Ts&&... input) {
  return std::decay_t<get_nth_type<0, Ts...>>{
      transform<indices>(std::forward<Func>(f), std::forward<Ts>(input)...)...};
}

template <typename Op, typename Struct, typename std::size_t... Is>
void apply_dispatch(Op&& op, Struct&& s, std::index_sequence<Is...>) {
  // TODO: in c++17 ((void) foo(std::forward<Args>(args)), ...);
  int dummy[] = {0,
                 ((void)std::forward<Op>(op)(boost::pfr::get<Is>(std::forward<Struct>(s))), 0)...};
}

}  // namespace detail

// Apply a transformation to turn a "list" of structs into one struct
template <typename Func,
          typename... Ts,
          typename = std::enable_if_t<detail::all_same<Ts...>::value, int>>
auto transform(Func&& f, Ts&&... input) {
  return detail::transform(
      std::forward<Func>(f),
      std::make_index_sequence<
          boost::pfr::tuple_size<std::decay_t<detail::get_nth_type<0, Ts...>>>::value>(),
      std::forward<Ts>(input)...);
}

// Apply a transformation function on all elements of a struct and return a tuple of results
template <typename Op, typename Struct>
void apply(Op&& op, Struct&& s) {
  detail::apply_dispatch(
      op, std::forward<Struct>(s),
      std::make_index_sequence<boost::pfr::tuple_size<std::decay_t<Struct>>::value>{});
}

}  // namespace struct_magic
