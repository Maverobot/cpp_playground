#include <ceres/ceres.h>
#include <ceres/internal/scoped_ptr.h>

#include <memory>

struct CostFunctor {
  template <typename T> bool operator()(T const *const x, T *residual) const {
    residual[0] = T(100.0) - x[0] * x[0] + T(20) * cos(2 * M_PI * x[0]);
    return true;
  }
};

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);

  // change the initial value to see different local minima
  const double initial_x = 0.1;
  double x = initial_x;

  ceres::Problem problem;

  ceres::CostFunction *cost_function(
      new ceres::AutoDiffCostFunction<CostFunctor, 1, 1>(new CostFunctor));

  problem.AddResidualBlock(cost_function, NULL, &x);

  // Run the solver
  ceres::Solver::Options options;
  options.linear_solver_type = ceres::DENSE_QR;
  options.minimizer_progress_to_stdout = true;
  ceres::Solver::Summary summary;
  ceres::Solve(options, &problem, &summary);

  // std::cout << summary.BriefReport() << std::endl;
  std::cout << summary.FullReport() << std::endl;
  std::cout << "x : " << initial_x << " -> " << x << std::endl;
  return 0;
}
