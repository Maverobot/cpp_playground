#include <array>
#include <iostream>
#include <iterator>
#include <tuple>

// The header for all the magic
#include <utility>

#include <indices_trick_own.h>

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
struct GetTypeSize {
  template <typename T> int operator()(T &&) const { return sizeof(T); }
};

std::tuple<int, char, float, double> tup{42, 'a', 0.99, 1.99};

int main(int argc, char *argv[]) {

  // Naive implementation with brute force
  std::array<int, 4> arr{
      GetTypeSize{}(std::get<0>(tup)), GetTypeSize{}(std::get<1>(tup)),
      GetTypeSize{}(std::get<2>(tup)), GetTypeSize{}(std::get<3>(tup))};

  std::cout << "Naive: " << arr << std::endl;

  // A complex version of indices trick
  auto res1 = indices_trick_own::execute_all(GetTypeSize{}, tup);
  std::cout << "Complex: " << res1 << std::endl;

  return 0;
}
