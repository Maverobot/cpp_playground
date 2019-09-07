#include <array>   // std::array
#include <utility> // std::index_sequence, std::make_index_sequence

/**
 * Indices trick (a detailed solution)
 */
namespace indices_trick_std {
template <typename Op, typename Tuple, typename std::size_t... Is>
std::array<int, sizeof...(Is)> f_all_dispatch(Op &&op, Tuple &&t,
                                              std::index_sequence<Is...>) {
  return std::array<int, sizeof...(Is)>{
      {std::forward<Op>(op)(std::get<Is>(std::forward<Tuple>(t)))...}};
};
template <typename Op, typename Tuple>
std::array<int, std::tuple_size<Tuple>::value> execute_all(Op &&op,
                                                           const Tuple &t) {
  return f_all_dispatch(
      op, t, std::make_index_sequence<std::tuple_size<Tuple>::value>{});
}
} // namespace indices_trick_std
