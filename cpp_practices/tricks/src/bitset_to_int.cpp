#include <bitset>
#include <iostream>

int main(int argc, char* argv[]) {
  std::bitset<16> bits{static_cast<unsigned long>(-58)};

  std::cout << "int16_t value: " << static_cast<int16_t>(bits.to_ulong());
  return 0;
}
