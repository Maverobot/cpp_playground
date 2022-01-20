#pragma once

#include <chrono>
#include <iostream>
#include <thread>

template <typename ControlPolicy>
class SmartRate {
 public:
  SmartRate(double r) : desired_interval_(1.0 / r), command_interval_(1.0 / r), control_(1.0 / r) {
    last_t_ = std::chrono::high_resolution_clock::now();
  }
  void sleep() {
    using namespace std::chrono_literals;
    std::this_thread::sleep_until(last_t_ + command_interval_);

    auto now = std::chrono::high_resolution_clock::now();
    real_interval_ = std::chrono::duration_cast<seconds>(now - last_t_);
    std::cout << "desired interval: " << desired_interval_.count()
              << ", command interval: " << command_interval_.count()
              << ", real interval: " << real_interval_.count()
              << " error: " << 1000 * std::abs(desired_interval_.count() - real_interval_.count())
              << " msecs" << std::endl;

    smart_adapt();
    last_t_ = std::chrono::high_resolution_clock::now();
  }

  void setRate(double r) { command_interval_ = seconds(1.0 / r); }

  double getRealRate() const { return 1 / real_interval_.count(); }
  double getDesiredRate() const { return 1 / desired_interval_.count(); }
  double getCommandRate() const { return 1 / command_interval_.count(); }

 private:
  using timepoint = std::chrono::system_clock::time_point;
  using seconds = std::chrono::duration<double, std::ratio<1>>;

  void smart_adapt() {
    command_interval_ =
        seconds(control_.get_command(command_interval_.count(), real_interval_.count()));
  }

  seconds desired_interval_;
  seconds command_interval_;
  seconds real_interval_;
  timepoint last_t_;

  ControlPolicy control_;
};
