#include <array>
#include <iostream>
#include <iterator>
#include <tuple>

// The header for all the magic
#include <utility>

// Make std::array printable with std::cout
template <class T, std::size_t N>
std::ostream &operator<<(std::ostream &o, const std::array<T, N> &arr) {
  copy(arr.cbegin(), arr.cend(), std::ostream_iterator<T>(o, " "));
  return o;
}

/**
 * Task: call function on each element of a tuple and save returned value in a
 * std::array
 */
template <typename T> int f(T) { return sizeof(T); }
std::tuple<int, char, double> tup{42, 'a', 1.99};

/**
 * Indices trick (a detailed solution)
 */
template <std::size_t... Is> struct Indices {};
template <typename Tuple, typename std::size_t... Is>
std::array<int, sizeof...(Is)> f_all_dispatch(Tuple &&t, Indices<Is...>) {
  return std::array<int, sizeof...(Is)>{
      {f(std::get<Is>(std::forward<Tuple>(t))...)}};
};

// Indices builder
template <std::size_t N, std::size_t... Is> struct build_indices {
  typedef typename build_indices<N - 1, N, Is...>::type type;
};
template <std::size_t... Is> struct build_indices<0, Is...> {
  typedef Indices<0, Is...> type;
};

template <typename T>
using Bare =
    typename std::remove_cv<typename std::remove_reference<T>::type>::type;

template <typename Tuple>
using IndicesFor =
    typename build_indices<std::tuple_size<Bare<Tuple>>::value>::type;

template <typename Tuple>
std::array<int, std::tuple_size<Tuple>::value> f_all(Tuple t) {
  return f_all_dispatch(t, IndicesFor<Tuple>());
}

int main(int argc, char *argv[]) {

  // Naive implementation with brute force
  std::array<int, 3> arr{f(std::get<0>(tup)), f(std::get<1>(tup)),
                         f(std::get<2>(tup))};

  std::cout << "Naive: " << arr << std::endl;

  // A complex version of indices trick
  auto res1 = f_all(tup);
  std::cout << "Complex: " << res1 << std::endl;

  return 0;
}
