#include <iostream>

int main(int argc, char* argv[]) {
  std::cout << "A" << std::endl;
  std::cout << 'A' + char{1} << std::endl;
  std::cout << std::string{1, 'A' + char{1}} << std::endl;
  std::cout << std::string{'A' + char{1}} << std::endl;
  return 0;
}
