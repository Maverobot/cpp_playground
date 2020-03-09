// requires: C++14
#include <struct_magic.h>

#include <array>
#include <iostream>
#include <iterator>
#include <string>

struct Data {
  double speed;
  float velocity;
  double acceleration;
  int some_integer;
};

template <typename T, size_t n>
std::ostream& operator<<(std::ostream& o, const std::array<T, n>& arr) {
  std::copy(arr.cbegin(), arr.cend(), std::ostream_iterator<T>(o, " "));
  return o;
}

int main() {
  std::cout << "=====================\n";
  // Util functions test
  std::cout << "int double double are same types: "
            << struct_magic::detail::all_same<int, double, double>::value << std::endl;
  std::cout << "int int int are same types: "
            << struct_magic::detail::all_same<int, int, int>::value << std::endl;

  std::cout << "=====================\n";
  // Structs
  Data v1{100., 11., 21., 3};
  Data v2{200., 12., 22., 5};
  using namespace boost::pfr::ops;
  std::cout << "v1: " << v1 << std::endl;
  std::cout << "v2: " << v2 << std::endl;
  std::cout << "=====================\n";

  // Transform each field of one struct with a lambda function
  auto v1_double = struct_magic::transform([](const auto& v) { return v * 2; }, v1);
  std::cout << "v1 * 2: " << v1_double << std::endl;

  std::cout << "=====================\n";
  // Use corresponding fields from two structs to create a new struct with a lambda function
  auto v3 = struct_magic::transform(
      [i = 0](auto& a, auto& b) mutable {
        std::cout << "index= " << i++ << "\n";
        return a + b;
      },
      v1, v2);
  std::cout << "v3 = v1 + v2: " << v3 << std::endl;
  std::cout << "=====================\n";

  // Transform the struct with a lambda function
  std::cout << "v1 before: " << v1 << std::endl;
  struct_magic::apply(
      [i = 0](auto& item) mutable {
        item = i;
        i++;
      },
      v1);
  std::cout << "v1 after: " << v1 << std::endl;
  std::cout << "=====================\n";

  // std::cout << res << std::endl;
}
