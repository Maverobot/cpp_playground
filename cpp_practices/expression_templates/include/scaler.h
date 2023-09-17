#pragma once
#include <cstdlib>

template <typename T>
class Scaler {
 public:
  Scaler(const T& v) : scaler_(v) {}

  const T& operator[](std::size_t) const noexcept { return scaler_; }

  std::size_t size() const noexcept { return 0; }

 private:
  // Note that the const reference prolongs the lifetime of a rvalue.
  const T& scaler_;
};
