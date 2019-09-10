#include <chrono>
#include <iostream>
#include <thread>
class DummyControl {
public:
  DummyControl(double target, double command) : target_(target) {}

  double get_command(double current) { return target_; }

private:
  double target_;
};

class SimplePControl {
  // Problem: unstable in the early phase
public:
  SimplePControl(double target, double command)
      : target_(target), command_(command) {}

  double get_command(double current) {
    command_ = command_ + k_ * (target_ - current);
    return command_;
  }

private:
  double target_;
  double command_;
  double k_{0.1};
};

template <typename ControlPolicy> class Rate {
public:
  Rate(double r)
      : desired_interval_(1.0 / r), command_interval_(1.0 / r),
        control_(1.0 / r, 1.0 / r) {
    last_t_ = std::chrono::high_resolution_clock::now();
  }
  void sleep() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_until(last_t_ + command_interval_);
    smart_adapt();
    last_t_ = std::chrono::high_resolution_clock::now();
  }

private:
  using timepoint = std::chrono::system_clock::time_point;
  using seconds = std::chrono::duration<double, std::ratio<1>>;

  void updateRate(double r) { command_interval_ = seconds(1.0 / r); }

  void smart_adapt() {
    auto now = std::chrono::high_resolution_clock::now();
    double real_interval =
        std::chrono::duration_cast<seconds>(now - last_t_).count();
    std::cout << "desired interval: " << desired_interval_.count()
              << ", command interval: " << command_interval_.count()
              << ", real interval: " << real_interval << " error: "
              << 1000 * std::abs(desired_interval_.count() - real_interval)
              << " msecs" << std::endl;

    command_interval_ = seconds(control_.get_command(real_interval));
  }

  seconds desired_interval_;
  seconds command_interval_;
  timepoint last_t_;

  ControlPolicy control_;
};

int main(int argc, char *argv[]) {
  auto rates =
      std::make_tuple(Rate<DummyControl>(1000), Rate<SimplePControl>(1000));
  using namespace std::chrono_literals;
  using std::chrono::high_resolution_clock;
  auto prev = high_resolution_clock::now();
  size_t i = 0;
  while (true) {

    // do something...
    std::this_thread::sleep_for(100us);

    if (i < 1000) {
      std::get<0>(rates).sleep();
    } else {
      std::get<1>(rates).sleep();
    }
    i++;
  }

  return 0;
}
