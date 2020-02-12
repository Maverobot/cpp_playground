#pragma once

class NoControl {
public:
  NoControl(double x_d) {}

  double get_command(double x_c, double x) { return x_c; }
};

class SimplePControl {
  // Problem: unstable in the early phase
public:
  SimplePControl(double x_d) : x_d(x_d) {}

  double get_command(double x_c, double x) {
    x_c = x_c + k_ * (x_d - x);
    return x_c;
  }

private:
  double x_d;
  double k_{0.1};
};
