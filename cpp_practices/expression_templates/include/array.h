#pragma once

#include <cassert>
#include <iostream>

#include "sarray.h"

template <typename T, typename Rep = SArray<T>>
class Array {
 private:
  Rep rep_;

 public:
  explicit Array(std::size_t size) : rep_(size){};

  Array(const Rep& rep) : rep_(rep){};

  Array(const std::initializer_list<T>& values) : rep_(values.size()) {
    for (size_t idx = 0; idx < values.size(); idx++) {
      rep_[idx] = *(values.begin() + idx);
    }
  }

  Array& operator=(const Array& rhs) {
    assert(size() == rhs.size());
    // Check for self-assignment!
    if (this == &rhs) {
      return *this;
    }
    rep_ = rhs.rep();
    return *this;
  }

  template <typename Expr2>
  Array& operator=(const Array<T, Expr2>& rhs) {
    assert(size() == rhs.size());
    for (std::size_t idx = 0; idx < size(); idx++) {
      rep_[idx] = rhs[idx];
    }
    return *this;
  }

  template <typename Expr2>
  bool operator==(const Array<T, Expr2>& rhs) const {
    assert(size() == rhs.size());
    for (std::size_t idx = 0; idx < size(); idx++) {
      if (rep_[idx] != rhs[idx]) {
        return false;
      }
    }
    return true;
  }

  auto operator[](std::size_t idx) const {
    assert(idx < size());
    return rep_[idx];
  }

  T& operator[](std::size_t idx) {
    assert(idx < size());
    return rep_[idx];
  }

  std::size_t size() const noexcept { return rep_.size(); }

  const Rep& rep() const noexcept { return rep_; }
  Rep& rep() noexcept { return rep_; }
};
