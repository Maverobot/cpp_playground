
#include <csv_reader/csv_reader.h>
#include <indices_trick_std.h>
#include <iostream>
#include <nameof.hpp>
#include <print_tuple.h>
#include <string>
#include <tuple>

#include <type_traits>

struct LineParser {
  LineParser(const std::string &csv_line) : line(csv_line) {}
  template <typename T> T operator()(T &&) const {
    std::istringstream iss(line);
    T res = parse_token<T>(iss);
    std::cout << "type: " << NAMEOF_TYPE(T) << ", res: " << res << std::endl;
    return res;
  }
  std::string line;
};

struct SensorData {
  SensorData() = default;
  SensorData(const std::string &csv_line) {
    LineParser line_parser(csv_line);
    std::cout << "line: " << csv_line << std::endl;
    data = execute_all(line_parser, data);
  }
  std::tuple<std::string, int, int, double, double, double, double, double,
             double, double, double, double, double>
      data{};
};

int main(int argc, char *argv[]) {

  auto sensor_data = load_csv_file<SensorData>(argv[1]);
  std::tuple<double, int> test{1.99, 2};
  std::cout << test << std::endl;

  std::get<9>(sensor_data[0].data) = 10;
  std::cout << sensor_data[0].data << std::endl;

  std::cout << std::tuple<double, int>{1.1, 1} << std::endl;

  return 0;
}
