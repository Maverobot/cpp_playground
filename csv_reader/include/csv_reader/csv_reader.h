#pragma once

#include <fstream>
#include <sstream>
#include <vector>

// parse a token from a csv line
template <typename T>
auto parse_token(std::istringstream &ss, char sep = ',') -> T {
  T result;
  std::string token;
  std::getline(ss, token, sep);
  std::stringstream stoken(token);
  stoken >> result;
  return result;
}

// load a csv file
template <typename T>
auto load_csv_file(const std::string &csv_path, bool has_header = true)
    -> std::vector<T> {
  std::vector<T> items;
  std::ifstream data(csv_path);
  std::string line;
  if (has_header)
    std::getline(data, line);
  while (std::getline(data, line)) {
    items.emplace_back(line);
  }
  return items;
}
