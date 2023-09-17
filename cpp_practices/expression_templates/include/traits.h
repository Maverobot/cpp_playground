#pragma once
#include "scaler.h"

template <typename T>
struct Traits {
  using ExprRef = const T&;
};

template <typename T>
struct Traits<Scaler<T>> {
  /**
   * Scaler nodes are bound within the operator functions and might not live until the end of the
   * evaluation of the complete expression.
   *
   * TODO: not understood yet...
   */
  using ExprRef = Scaler<T>;
};
