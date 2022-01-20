#include "ceres/ceres.h"
#include "glog/logging.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wregister"
#include <matplotlibcpp.h>  //NO_LINT
#pragma GCC diagnostic pop

#include <polynomial.h>

using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solve;
using ceres::Solver;

namespace plt = matplotlibcpp;

const int kNumObservations = 4;
const std::vector<double> data_x{2, 3, 4, 5};
const std::vector<double> data_y{2, 1, 1, 1};

struct QuinticPolynomialResidual {
  QuinticPolynomialResidual(double x, double y) : x_(x), y_(y) {}
  template <typename T>
  bool operator()(const T* const a,
                  const T* const b,
                  const T* const c,
                  const T* const d,
                  const T* const e,
                  const T* const f,
                  T* residual) const {
    // residual = y - (a*x^5 + b*x^4 + c*x^3 + d*x^2 + e*x + f)
    residual[0] = y_ - get_polynomial_functor(a[0], b[0], c[0], d[0], e[0], f[0])(x_);
    return true;
  }

 private:
  const double x_;
  const double y_;
};

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  double a = 0.0;
  double b = 0.0;
  double c = 0.0;
  double d = 0.0;
  double e = 0.0;
  double f = 0.0;
  Problem problem;
  for (int i = 0; i < kNumObservations; ++i) {
    problem.AddResidualBlock(
        new AutoDiffCostFunction<QuinticPolynomialResidual, 1, 1, 1, 1, 1, 1, 1>(
            new QuinticPolynomialResidual(data_x[i], data_y[i])),
        NULL, &a, &b, &c, &d, &e, &f);
  }
  Solver::Options options;
  options.max_num_iterations = 25;
  options.linear_solver_type = ceres::DENSE_QR;
  options.minimizer_progress_to_stdout = true;
  Solver::Summary summary;
  Solve(options, &problem, &summary);
  std::cout << summary.BriefReport() << std::endl;
  std::cout << "Final   a: " << a << " b: " << b << " c: " << c << " d: " << d << " e: " << e
            << " f: " << f << std::endl;

  auto generator = get_polynomial_functor(a, b, c, d, e, f);

  // Interpolated data
  int n = 5000;
  std::vector<double> x(n), y(n);
  const double t_min = 0;
  const double t_max = 5;
  const double interval = (t_max - t_min) / n;
  for (size_t i = 0; i < n; i++) {
    x.at(i) = t_min + i * interval;
    y.at(i) = generator(x.at(i));
  }

  // Set the size of output image to 1200x780 pixels
  plt::figure_size(1200, 780);
  // Plot line from given x and y data. Color is selected automatically.
  plt::named_plot("via points", data_x, data_y, "*");
  plt::named_plot("interpolated points", x, y);
  // Set x-axis to interval [0,1000000]
  plt::xlim(0, 5);
  // Add graph title
  plt::title("Cubic Polynomial Test");
  plt::axis("equal");
  plt::legend();
  plt::show();
  return 0;
}
