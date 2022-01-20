#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <vector>

#include "type.hpp"

// Make std::array printable with std::cout
template <class T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& arr) {
  copy(arr.cbegin(), arr.cend(), std::ostream_iterator<T>(o, " "));
  return o;
}

/**
 * Link: https://stackoverflow.com/a/59984931/6018272
 *
 * Explanation: I will call the original unwrap from OP UNWRAP in the following
 * to differentiate with my version. We want to invoke the reference_wrapper
 * specification of UNWRAP whenever std::decay_t<T> is a std::reference_wrapper.
 * Now this could be simply accomplished if we always invoke UNWRAP with
 * std::decay_t<T> instead of T.
 *
 * The problem with this is, that if T is not a reference_wrapper, this will
 * remove all qualifications, i.e. UNWRAP<std::decay_t<const int>> is int when
 * we would want it to be const int.
 *
 * To get around this we define us template<class type_t, class orig_t> struct
 * unwrap_impl. We want to always pass this the decayed type for the first
 * argument and the original type (before decaying) as the second argument.
 * Then, we can pass for the general case orig_t as the result type (as done by
 * using type = orig_t).
 *
 * For the specification, we define template<class type_t, class V> struct
 * unwrap_impl<std::reference_wrapper<type_t>, V>. This will apply whenever
 * type_t is a reference_wrapper, i.e. when the orignal type is some
 * qualification of a reference_wrapper. We don't care about the second argument
 * (which will be the original type), so we just ignore it. Then we take the
 * inner type of the reference_wrapper as type (using type = type_t;).
 *
 * Then we call unwrap_impl by defining basically template<class type_t> unwrap
 * = detail::unwrap_impl<std::decay_t<type_t>, type_t>; (this is pseudocode but
 * I think this makes it more clear.
 */

namespace detail {
template <typename type_t, class orig_t>
struct unwrap_impl {
  using type = orig_t;
};

template <typename type_t, class V>
struct unwrap_impl<std::reference_wrapper<type_t>, V> {
  using type = type_t;
};
}  // namespace detail

template <class T>
struct unwrap {
  using type = typename detail::unwrap_impl<std::decay_t<T>, T>::type;
};

template <typename type_t>
using unwrap_t = typename unwrap<type_t>::type;

template <typename container_t>
auto range(container_t& container) {
  return std::tuple{std::begin(container), std::end(container)};
};
template <typename container_t>
auto range(const container_t& container) {
  return std::tuple{std::begin(container), std::end(container)};
};

template <typename container_t, typename predicate_t>
auto where(const container_t& container, predicate_t predicate) {
  auto [b, e] = range(container);
  using type = unwrap_t<std::remove_reference_t<std::remove_const_t<decltype(*b)>>>;
  using reference = std::reference_wrapper<type>;

  std::vector<reference> result{};

  std::copy_if(b, e, std::back_inserter(result), predicate);

  // std::cout << __PRETTY_FUNCTION__ << "\n"
  //           << '\t'
  //           << "decltype(*b) = " << demangle(typeid(decltype(*b)).name())
  //           << '\n'
  //           << '\t' << "type = " << demangle(typeid(type).name()) << '\n'
  //           << '\t' << "reference = " << demangle(typeid(reference).name())
  //           << '\n';

  return result;
}

int main() {
  std::vector v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

  std::cout << "even numbers: " << where(v, [](auto n) { return n % 2 == 0; }) << std::endl;

  std::cout << "odd numbers: " << where(v, [](auto n) { return n % 2 == 1; }) << std::endl;

  std::cout << "x & 0b10: " << where(v, [](auto n) { return n & 0b10; }) << std::endl;

  std::cout << "x & 0b01: " << where(v, [](auto n) { return n & 0b01; }) << std::endl;

  return 0;
}
