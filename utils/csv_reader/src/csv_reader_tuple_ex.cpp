
#include <csv_reader/csv_reader.h>
#include <indices_trick_std.h>
#include <iostream>
#include <nameof.hpp>
#include <print_tuple.h>
#include <string>
#include <tuple>

#include <type_traits>

class LineParser {
public:
  LineParser(const std::string &csv_line) : iss(csv_line) {}
  template <typename T> T operator()(T &&) { return parse_token<T>(iss); }

private:
  std::istringstream iss;
};

struct SensorData {
  SensorData() = default;
  SensorData(const std::string &csv_line) {
    LineParser line_parser(csv_line);
    data = execute_all(line_parser, data);
  }
  std::tuple<std::string, int, int, double, double, double, double, double,
             double, double, double, double, double>
      data{};
};

int main(int argc, char *argv[]) {

  auto sensor_data = load_csv_file<SensorData>(argv[1]);
  std::cout << sensor_data[0].data << std::endl;
  std::cout << sensor_data[1].data << std::endl;

  return 0;
}
