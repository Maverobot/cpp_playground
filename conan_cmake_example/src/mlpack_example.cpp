// Includes all relevant components of mlpack.
#include <mlpack/core.hpp>
// Convenience.
using namespace mlpack;
int main(int argc, char *argv[]) {

  // Loads the data.
  arma::Mat<double> data;
  data::Load(argv[1], data, true, false);

  // Saves the first 10 lines
  data::Save("data_head.csv", arma::mat(data.head_rows(10)), true);
  return 0;
}
