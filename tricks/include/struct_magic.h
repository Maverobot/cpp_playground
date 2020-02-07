#pragma once

#include <array>   // std::array
#include <utility> // std::index_sequence, std::make_index_sequence

#include <boost/pfr/precise.hpp> // magic_get

namespace struct_magic {

namespace detail {
template <bool...> struct bool_pack;
template <bool... bs>
// if any are false, they'll be shifted in the second version, so types won't
// match
using all_true_impl =
    std::is_same<bool_pack<bs..., true>, bool_pack<true, bs...>>;
template <typename... Ts> using all_true = detail::all_true_impl<Ts::value...>;

template <size_t idx, typename... Ts>
using get_nth_type = std::tuple_element_t<idx, std::tuple<Ts...>>;

template <size_t index, typename Func, typename... Ts>
auto transform(Func f, Ts... input) {
  return f(boost::pfr::get<index>(std::forward<Ts>(input))...);
}

template <typename Func, size_t... indices, typename... Ts>
auto transform(Func f, std::index_sequence<indices...>, Ts... input) {
  return get_nth_type<0, Ts...>{transform<indices>(f, input...)...};
}

} // namespace detail

template <typename... Ts>
using all_same =
    detail::all_true<std::is_same<detail::get_nth_type<0, Ts...>, Ts...>>;

// Execute function which transforms a "list" of tuples into one tuple
template <typename Func, typename... Ts> auto transform(Func f, Ts... input) {
  return detail::transform(
      f,
      std::make_index_sequence<
          boost::pfr::tuple_size<detail::get_nth_type<0, Ts...>>::value>(),
      std::forward<Ts>(input)...);
}

} // namespace struct_magic
