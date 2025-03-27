#pragma once

#include <cassert>
#include <cstdlib>

#include "array.h"
#include "traits.h"

/**
 * Expression template for binary operators
 *
 * @tparam T the type of the elements
 * @tparam OP1 the type of the left operand
 * @tparam OP2 the type of the right operand
 * @tparam FunctionOP the type of the function operator
 *
 * This class uses expression template technique to enable compile-time optimization of binary
 * operators.
 */
template <typename T, typename OP1, typename OP2, typename FunctionOP>
class BinaryOperator {
 private:
  typename Traits<OP1>::ExprRef op1_;
  typename Traits<OP2>::ExprRef op2_;

 public:
  BinaryOperator(const OP1& op1, const OP2& op2) : op1_(op1), op2_(op2) {}
  T operator[](size_t index) const { return FunctionOP()(op1_[index], op2_[index]); }
  std::size_t size() const {
    assert(op1_.size() == 0 || op2_.size() || op1_.size() == op2_.size());
    return op1_.size() == 0 ? op2_.size() : op1_.size();
  }
};

template <typename T, typename OP1, typename OP2>
using Add = BinaryOperator<T, OP1, OP2, std::plus<T>>;

template <typename T, typename OP1, typename OP2>
using Mul = BinaryOperator<T, OP1, OP2, std::multiplies<T>>;

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
