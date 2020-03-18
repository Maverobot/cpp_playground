#include <iostream>
#include <toml.hpp>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s path_to_toml_file\n", argv[0]);
    return 1;
  }
  auto data = toml::parse(argv[1]);

  // find a value with the specified type from a table
  std::string title = toml::find<std::string>(data, "title");

  // convert the whole array into any container automatically
  std::vector<int> nums = toml::find<std::vector<int>>(data, "nums");

  // access with STL-like manner
  if (!data.contains("a") || !data.at("a").contains("b")) {
    data["a"]["b"] = "c";
  }

  // pass a fallback
  std::string name = toml::find_or<std::string>(data, "name", "not found");

  // width-dependent formatting
  std::cout << std::setw(80) << data << std::endl;

  return 0;
}
