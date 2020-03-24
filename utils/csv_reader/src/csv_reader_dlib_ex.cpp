#include <algorithm>
#include <csv_reader/csv_reader.h>
#include <iostream>
#include <string>

#include <dlib/matrix.h>

namespace my_dlib {
template <typename T, size_t num_rows> struct MatrixData {
  MatrixData() = default;
  MatrixData(const std::string &csv_line) {
    std::istringstream iss(csv_line);
    for (size_t idx_element = 0; idx_element < num_rows; idx_element++) {
      data(idx_element, 0) = parse_token<T>(iss);
    }
  }
  dlib::matrix<T, num_rows, 1> data;
};

} // namespace my_dlib

int main(int argc, char *argv[]) {

  /*
  auto sensor_data = load_csv_file<my_dlib::MatrixData<double, 13>>(argv[1]);
  std::cout << sensor_data[0].data << std::endl;
  std::cout << sensor_data[1].data << std::endl;
  */

  std::ifstream ifs(argv[1]);
  dlib::matrix<double> my_matrix;
  dlib::set_all_elements(my_matrix, 0);
  std::cout << "test matrix: " << my_matrix << std::endl;
  ifs >> my_matrix;
  std::cout << "test matrix: " << my_matrix << std::endl;

  return 0;
}
