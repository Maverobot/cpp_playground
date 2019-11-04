#include "ceres/ceres.h"
#include "glog/logging.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wregister"
#include <matplotlibcpp.h> //NO_LINT
#pragma GCC diagnostic pop

#include <polynomial.h>

using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solve;
using ceres::Solver;

namespace plt = matplotlibcpp;

const std::vector<double> t{2, 7, 11, 13};
const std::vector<double> positions{1, 1, 1, 1};
// TODO: velocities have little impact for now, it seems ceres-solver stops at
// the first local minimum
const std::vector<double> velocities{0, 0, 0, 0};
const int kNumObservations = t.size();

struct QuinticTrajectoryGeneratorResidual {
  QuinticTrajectoryGeneratorResidual(double t, double x, double dx)
      : t_(t), x_(x), dx_(dx) {}
  template <typename T>
  bool operator()(const T *const a, const T *const b, const T *const c,
                  const T *const d, const T *const e, const T *const f,
                  T *residual) const {
    // residual = x - (a*x^5 + b*x^4 + c*x^3 + d*x^2 + e*x + f)
    residual[0] =
        x_ - get_polynomial_functor(a[0], b[0], c[0], d[0], e[0], f[0])(t_);

    // residual = dx - (5*a*x^4 + 4*b*x^3 + 3*c*x^2 + 2*d*x + e)
    residual[1] = dx_ - get_polynomial_functor_derivative(a[0], b[0], c[0],
                                                          d[0], e[0], f[0])(t_);
    return true;
  }

  static constexpr size_t num_residuals = 2;

private:
  const double t_;
  const double x_;
  const double dx_;
};

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  double a = 0;
  double b = 0;
  double c = 0;
  double d = 0;
  double e = 0;
  double f = 0;
  Problem problem;
  for (int i = 0; i < kNumObservations; ++i) {
    problem.AddResidualBlock(
        new AutoDiffCostFunction<
            QuinticTrajectoryGeneratorResidual,
            QuinticTrajectoryGeneratorResidual::num_residuals, 1, 1, 1, 1, 1,
            1>(new QuinticTrajectoryGeneratorResidual(t[i], positions[i],
                                                      velocities[i])),
        NULL, &a, &b, &c, &d, &e, &f);
  }

  Solver::Options options;
  options.max_num_iterations = 25;
  options.linear_solver_type = ceres::DENSE_NORMAL_CHOLESKY;
  options.minimizer_progress_to_stdout = true;
  Solver::Summary summary;
  Solve(options, &problem, &summary);
  std::cout << summary.BriefReport() << std::endl;
  std::cout << "Final   a: " << a << " b: " << b << " c: " << c << " d: " << d
            << " e: " << e << " f: " << f << std::endl;

  auto generator = get_polynomial_functor(a, b, c, d, e, f);

  // Interpolated data
  int n = 5000;
  std::vector<double> x(n), y(n);
  const double t_min = *std::min_element(t.cbegin(), t.cend()) - 1;
  const double t_max = *std::max_element(t.cbegin(), t.cend()) + 1;
  const double interval = (t_max - t_min) / n;
  for (size_t i = 0; i < n; i++) {
    x.at(i) = t_min + i * interval;
    y.at(i) = generator(x.at(i));
  }

  // Set the size of output image to 1200x780 pixels
  plt::figure_size(1200, 780);
  // Plot line from given x and y data. Color is selected automatically.
  plt::named_plot("via points", t, positions, "*");
  plt::named_plot("interpolated points", x, y);
  // Set x-axis to interval [0,1000000]
  plt::xlim(0, 5);
  // Add graph title
  plt::title("Quintic Polynomial Test");
  plt::axis("equal");
  plt::legend();
  plt::show();
  return 0;
}
