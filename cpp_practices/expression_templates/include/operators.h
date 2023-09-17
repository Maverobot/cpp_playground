#pragma once

#include <cassert>
#include <cstdlib>

#include "array.h"
#include "traits.h"

template <typename T, typename OP1, typename OP2>
class Add {
 private:
  typename Traits<OP1>::ExprRef op1_;
  typename Traits<OP2>::ExprRef op2_;

 public:
  Add(const OP1& op1, const OP2& op2) : op1_(op1), op2_(op2) {}
  T operator[](size_t index) const { return op1_[index] + op2_[index]; }
  std::size_t size() const {
    assert(op1_.size() == 0 || op2_.size() || op1_.size() == op2_.size());
    return op1_.size() == 0 ? op2_.size() : op1_.size();
  }
};

template <typename T, typename OP1, typename OP2>
class Mul {
 private:
  typename Traits<OP1>::ExprRef op1_;
  typename Traits<OP2>::ExprRef op2_;

 public:
  Mul(const OP1& op1, const OP2& op2) : op1_(op1), op2_(op2) {}
  T operator[](size_t index) const { return op1_[index] * op2_[index]; }
  std::size_t size() const {
    assert(op1_.size() == 0 || op2_.size() || op1_.size() == op2_.size());
    return op1_.size() == 0 ? op2_.size() : op1_.size();
  }
};

template <typename T, typename Rep>
auto operator+(const T& lhs, const Array<T, Rep>& rhs) {
  return Array<T, Add<T, Scaler<T>, Rep>>(Add<T, Scaler<T>, Rep>(Scaler<T>(lhs), rhs.rep()));
}

template <typename T, typename Rep>
auto operator*(const T& lhs, const Array<T, Rep>& rhs) {
  return Array<T, Mul<T, Scaler<T>, Rep>>(Mul<T, Scaler<T>, Rep>(Scaler<T>(lhs), rhs.rep()));
}

template <typename T, typename Rep1, typename Rep2>
auto operator+(const Array<T, Rep1>& lhs, const Array<T, Rep2>& rhs) {
  return Array<T, Add<T, Rep1, Rep2>>(Add<T, Rep1, Rep2>(lhs.rep(), rhs.rep()));
}

template <typename T, typename Rep1, typename Rep2>
auto operator*(const Array<T, Rep1>& lhs, const Array<T, Rep2>& rhs) {
  return Array<T, Mul<T, Rep1, Rep2>>(Mul<T, Rep1, Rep2>(lhs.rep(), rhs.rep()));
}
