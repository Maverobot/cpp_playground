#include <array>   // std::array
#include <tuple>   // std::tuple
#include <utility> // std::index_sequence, std::make_index_sequence

/**
 * Indices trick (a detailed solution)
 */
namespace indices_trick_std {
namespace simple_test {
template <typename Op, typename Tuple, typename std::size_t... Is>
std::array<int, sizeof...(Is)> f_all_dispatch(Op &&op, Tuple &&t,
                                              std::index_sequence<Is...>) {
  return std::array<int, sizeof...(Is)>{
      {std::forward<Op>(op)(std::get<Is>(std::forward<Tuple>(t)))...}};
}

// Execute function on all element types of a tuple and return an array of int
template <typename Op, typename Tuple>
std::array<int, std::tuple_size<Tuple>::value> execute_all(Op &&op,
                                                           const Tuple &t) {
  return f_all_dispatch(
      op, t, std::make_index_sequence<std::tuple_size<Tuple>::value>{});
}
} // namespace simple_test

namespace detail {

template <typename Op, typename Tuple, typename std::size_t... Is>
auto f_all_dispatch(Op &&op, Tuple &&t, std::index_sequence<Is...>) {
  return std::make_tuple(
      std::forward<Op>(op)(std::get<Is>(std::forward<Tuple>(t)))...);
}

} // namespace detail

// Execute function on all element types of a tuple and return a tuple of
// results
template <typename Op, typename Tuple>
auto execute_all(Op &&op, const Tuple &t) {
  return detail::f_all_dispatch(
      op, t, std::make_index_sequence<std::tuple_size<Tuple>::value>{});
}

} // namespace indices_trick_std
