#include <chrono>
#include <iostream>
#include <thread>

#include <smart_rate/simple_control.h>
#include <smart_rate/smart_rate.h>

#include <dlib/rand.h>
#include <dlib/svm.h>

class KrlsRegression {
public:
  KrlsRegression() : reg_(KernelType(0.1), 0.001){};

  void train(double x, double y) {
    SampleType m;
    m(0) = x;
    reg_.train(m, y);
  }

  double eval(double x) const {
    SampleType m;
    m(0) = x;
    return reg_(m);
  }

private:
  using SampleType = dlib::matrix<double, 1, 1>;
  using KernelType = dlib::radial_basis_kernel<SampleType>;

  dlib::krls<KernelType> reg_;
};

int main(int argc, char *argv[]) {
  auto rate = SmartRate<NoControl>(1000);
  using namespace std::chrono_literals;
  using std::chrono::high_resolution_clock;
  auto prev = high_resolution_clock::now();
  size_t i = 0;

  dlib::rand rng;
  KrlsRegression reg;
  while (true) {

    // do something...
    std::this_thread::sleep_for(100us);

    rate.sleep();

    double r_c = rate.getCommandRate();
    double r = rate.getRealRate();

    if (i < 5000) {
      std::cout << "i = " << i << std::endl;
      reg.train(r, r_c);
      rate.setRate(rate.getDesiredRate() + rng.get_double_in_range(-200, 200));

    } else {
      double opt_rate = reg.eval(1000);
      std::cout << "Sleep rate should be set to: " << opt_rate << std::endl;
      reg.train(r, r_c);
      rate.setRate(opt_rate);
    }
    i++;
  }

  return 0;
}
