// Copyright 2017 James E. King III
// Distributed under the Boost Software License, Version 1.0.
// (See https://www.boost.org/LICENSE_1_0.txt)
//  mkuuid.cpp example

#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iomanip>
#include <iostream>

template <typename T>
inline std::string int_to_hex(T val, size_t width = sizeof(T) * 2) {
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(width) << std::hex << (val | 0);
  return ss.str();
}
int main(void) {
  boost::uuids::random_generator gen;
  auto uuid = gen();

  for (size_t i = 0; i < uuid.size(); i++) {
    std::cout << int_to_hex(uuid.data[i]) << std::endl;
  }

  std::cout << boost::uuids::to_string(uuid) << std::endl;
  std::cout << boost::lexical_cast<std::string>(uuid) << std::endl;
  return 0;
}
