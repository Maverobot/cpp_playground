#include <matplotlibcpp.h>
#include <iterator>
#include <thread>

namespace plt = matplotlibcpp;

template <typename T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& v) {
  std::copy(v.cbegin(), v.cend(), std::ostream_iterator<T>(o, " "));
  return o;
}

int main() {
  plt::title("Canvas");
  plt::axis("equal");

  // Prepare plotting the tangent.
  plt::Plot plot("linear increaser");
  plt::legend();

  const size_t kMaxVecSize = 100;
  std::vector<double> values;
  std::vector<double> x;
  double v = 0;
  while (true) {
    values.push_back(v);
    x.resize(values.size());
    std::generate(x.begin(), x.end(), [n = 0]() mutable { return n++; });
    plot.update(x, values);
    // Small pause so the viewer has a chance to enjoy the animation.
    plt::pause(0.1);
    v = v + 1;
    plt::xlim(0, 1000);
    plt::ylim(0, 1000);
  }
}
