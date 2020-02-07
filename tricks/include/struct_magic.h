#pragma once

#include <array>   // std::array
#include <utility> // std::index_sequence, std::make_index_sequence

#include <boost/pfr/precise.hpp> // magic_get

namespace struct_magic {
template <typename Op, typename Struct, typename std::size_t... Is>
auto f_all_dispatch(Op &&op, Struct &&st, std::index_sequence<Is...>) {
  return Struct{
      std::forward<Op>(op)(boost::pfr::get<Is>(std::forward<Struct>(st)))...};
}

template <typename... Types>
constexpr bool all_same_v =
    sizeof...(Types)
        ? (std::is_same_v<std::tuple_element_t<0, std::tuple<Types...>>, Types>
               &&...)
        : false;

// Execute function on all element types of a tuple and return an array of int
template <typename Op, typename Struct>
auto transform(Op &&op, const Struct &st) {
  return f_all_dispatch(
      op, st,
      std::make_index_sequence<boost::pfr::tuple_size<Struct>::value>{});
}
} // namespace struct_magic
