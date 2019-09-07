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
struct GetTypeSize {
  template <typename T> int operator()(T &&) const { return sizeof(T); }
};

int main(int argc, char *argv[]) {

  // Naive implementation with brute force
  std::array<int, 4> arr{
      GetTypeSize{}(std::get<0>(tup)), GetTypeSize{}(std::get<1>(tup)),
      GetTypeSize{}(std::get<2>(tup)), GetTypeSize{}(std::get<3>(tup))};
  std::cout << "Naive: " << arr << std::endl;

  // Indices trick with own indices builder
  auto res1 = indices_trick_own::execute_all(GetTypeSize{}, tup);
  std::cout << "Indices trick with own indices builder: " << res1 << std::endl;

  // Indices trick with std::make_index_sequence, std::index_sequence
  auto res2 = indices_trick_std::execute_all(GetTypeSize{}, tup);
  std::cout << "Indices trick with std header <utility>: " << res2 << std::endl;

  return 0;
}
