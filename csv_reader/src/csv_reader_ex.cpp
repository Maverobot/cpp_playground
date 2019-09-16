
#include <csv_reader/csv_reader.h>
#include <iostream>
#include <string>

struct SensorData {
  SensorData() = default;
  SensorData(const std::string &csv_line) {
    std::istringstream iss(csv_line);
    row_id = parse_token<std::string>(iss);
    series_id = parse_token<int>(iss);
    measurement_number = parse_token<int>(iss);
    orientation_X = parse_token<double>(iss);
    orientation_Y = parse_token<double>(iss);
    orientation_Z = parse_token<double>(iss);
    orientation_W = parse_token<double>(iss);
    angular_velocity_X = parse_token<double>(iss);
    angular_velocity_Y = parse_token<double>(iss);
    angular_velocity_Z = parse_token<double>(iss);
    linear_acceleration_X = parse_token<double>(iss);
    linear_acceleration_Y = parse_token<double>(iss);
    linear_acceleration_Z = parse_token<double>(iss);
  }
  std::string row_id{};
  int series_id{};
  int measurement_number{};
  double orientation_X{}, orientation_Y{}, orientation_Z{}, orientation_W{},
      angular_velocity_X{}, angular_velocity_Y{}, angular_velocity_Z{},
      linear_acceleration_X{}, linear_acceleration_Y{}, linear_acceleration_Z{};
};

int main(int argc, char *argv[]) {

  auto data = load_csv_file<SensorData>(argv[1]);

  std::cout << "row_id: " << data[0].row_id << std::endl;
  std::cout << "linear acce: " << data[1].linear_acceleration_Z << std::endl;

  return 0;
}
