#include <iostream>
#include <vector>

#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>
using std::cout;

int main() {
  using namespace ranges;
  auto res1 = views::ints(1, unreachable) |
              views::transform([](int i) { return i * i; }) | views::take(10);

  auto res2 = res1 | view::reverse;

  for (auto e : res2) {
    std::cout << e << " ";
  }
  std::cout << std::endl;

  std::cout << res1 << std::endl;
  std::cout << res2 << std::endl;

  int sum = accumulate(res1, 0);
  std::cout << "sum = " << sum << std::endl;
}
///[accumulate_ints]
