#include <iostream>
#include <toml.hpp>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s path_to_toml_file\n", argv[0]);
    return 1;
  }
  const auto data = toml::parse(argv[1]);

  const auto& general = toml::find<toml::table>(data, "General");
  const auto& devices_data = toml::find<std::vector<toml::table>>(data, "Devices");

  std::cout << "General/iterations: " << general.at("iterations") << "\n";

  std::cout << "Devices data: \n";
  for (const toml::table& device_data : devices_data) {
    std::cout << "Device ID " << device_data.at("id") << " - param " << device_data.at("param")
              << "\n";
  }

  return 0;
}
