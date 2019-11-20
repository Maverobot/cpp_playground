#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <tuple>

#include <indices_trick_own.h>
#include <indices_trick_std.h>

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
std::tuple<int, char, float, double> tup{42, 'a', 0.99, 1.99};
struct get_type_size {
  template <typename T> int operator()(T &&) const { return sizeof(T); }
};

template <typename T> struct value_match {
  value_match(T target) : target(target){};
  template <typename U> bool operator()(U &&value) const {
    if constexpr (std::is_same_v<std::decay_t<T>, std::decay_t<U>>) {
      if (value == target) {
        return true;
      }
      return false;
    } else {
      return false;
    }
  }
  T target;
};

template <typename Tuple, typename T>
int getIndexByValue(Tuple &&tup, T &&value) {
  auto flags = indices_trick_std::execute_all(
      value_match(std::forward<T>(value)), std::forward<Tuple>(tup));
  auto iter =
      std::find_if(flags.cbegin(), flags.cend(), [](auto &v) { return v; });
  if (iter != flags.cend()) {
    return std::distance(flags.cbegin(), iter);
  }
  return -1;
}

int main(int argc, char *argv[]) {

  // Naive implementation with brute force
  std::array<int, 4> arr{
      get_type_size{}(std::get<0>(tup)), get_type_size{}(std::get<1>(tup)),
      get_type_size{}(std::get<2>(tup)), get_type_size{}(std::get<3>(tup))};
  std::cout << "Naive: " << arr << std::endl;

  // Indices trick with own indices builder
  auto res1 = indices_trick_own::execute_all(get_type_size{}, tup);
  std::cout << "Indices trick with own indices builder: " << res1 << std::endl;

  // Indices trick with std::make_index_sequence, std::index_sequence
  auto res2 = indices_trick_std::execute_all(get_type_size{}, tup);
  std::cout << "Indices trick with std header <utility>: " << res2 << std::endl;

  // Get index of the matching value
  std::cout << "The index of the tuple having the matching value is: "
            << getIndexByValue(tup, 0.99f) << std::endl;

  return 0;
}
