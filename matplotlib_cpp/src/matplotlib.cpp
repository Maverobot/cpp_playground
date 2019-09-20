#include <iterator>
#include <matplotlibcpp.h>
#include <thread>

namespace plt = matplotlibcpp;

template <typename T>
std::ostream &operator<<(std::ostream &o, const std::vector<T> &v) {
  std::copy(v.cbegin(), v.cend(), std::ostream_iterator<T>(o, " "));
  return o;
}

int main() {
  // Prepare plotting the tangent.
  plt::Plot plot("plot update");

  std::vector<double> values;
  std::vector<double> x;
  double v = 0;
  while (true) {
    std::cout << "step()" << std::endl;
    values.push_back(v);
    x.resize(values.size());
    std::generate(x.begin(), x.end(), [n = 0]() mutable { return n++; });
    std::cout << x << std::endl;
    std::cout << values << std::endl;
    plot.update(x, values);
    plt::show(false);
    // Small pause so the viewer has a chance to enjoy the animation.
    plt::pause(0.1);
    v = v + 1;
  }
}
