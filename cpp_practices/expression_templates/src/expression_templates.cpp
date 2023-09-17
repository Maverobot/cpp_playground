#include <iostream>
#include "array.h"
#include "operators.h"

int main(int argc, char* argv[]) {
  Array<double> x(5), y(5);

  for (size_t i = 0; i < y.size(); i++) {
    x[i] = i;
    y[i] = i;
  }

  x = 1.2 * x + x * y;

  for (size_t i = 0; i < x.size(); i++) {
    std::cout << "x " << i << "= " << x[i] << "\n";
  }
  return 0;
}
