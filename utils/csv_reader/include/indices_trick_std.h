#include <tuple>    // std::tuple
#include <utility>  // std::index_sequence, std::make_index_sequence

/**
 * Indices trick (a detailed solution)
 */

template <typename T>
using Bare = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

template <typename Op, typename Tuple, typename std::size_t... Is>
Bare<Tuple> f_all_dispatch(Op&& op, Tuple&& t, std::index_sequence<Is...>) {
  return Bare<Tuple>{std::forward<Op>(op)(std::get<Is>(std::forward<Bare<Tuple>>(t)))...};
};

template <typename Op, typename Tuple>
Bare<Tuple> execute_all(Op&& op, Tuple&& t) {
  return f_all_dispatch(op, t, std::make_index_sequence<std::tuple_size<Bare<Tuple>>::value>{});
}
