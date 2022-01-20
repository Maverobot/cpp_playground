#include <array>

/**
 * Indices trick (a detailed solution)
 */
namespace indices_trick_own {
template <std::size_t... Is>
struct Indices {};
template <typename Op, typename Tuple, typename std::size_t... Is>
std::array<int, sizeof...(Is)> f_all_dispatch(Op&& op, Tuple&& t, Indices<Is...>) {
  return std::array<int, sizeof...(Is)>{
      {std::forward<Op>(op)(std::get<Is>(std::forward<Tuple>(t)))...}};
};
// Indices builder
template <std::size_t N, std::size_t... Is>
struct build_indices {
  using type = typename build_indices<N - 1, N - 1, Is...>::type;
};
template <std::size_t... Is>
struct build_indices<0, Is...> {
  using type = Indices<Is...>;
};
template <typename Tuple>
using IndicesFor = typename build_indices<std::tuple_size<Tuple>::value>::type;
template <typename Op, typename Tuple>
std::array<int, std::tuple_size<Tuple>::value> execute_all(Op&& op, const Tuple& t) {
  return f_all_dispatch(op, t, IndicesFor<Tuple>());
}
}  // namespace indices_trick_own
