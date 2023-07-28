#include <functional>
#include <iostream>
#include <iterator>
#include <random>

template <class T, std::size_t N>
std::ostream& operator<<(std::ostream& o, const std::array<T, N>& arr) {
  copy(arr.cbegin(), arr.cend(), std::ostream_iterator<T>(o, ""));
  return o;
}

int main(int argc, char* argv[]) {
  constexpr size_t length = 7;
  std::random_device rd;
  std::mt19937 gen(rd());
  auto distrib = std::bind(std::uniform_int_distribution<int>(0, pow(2, length) - 1), gen);

  int random_integer = distrib();
  std::cout << "random_integer: " << random_integer << "\n";
  std::array<bool, length> output;

  for (int i = length - 1; i >= 0; i--) {
    output.at(i) = random_integer & 1;
    random_integer /= 2;
  }

  std::cout << "boolean array: " << output << "\n";
  return 0;
}
