#pragma once

#include <memory>
#include <stdexcept>

// Preprocessor trick
#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)  // Expand __COUNTER__ or __LINE__
#ifdef __COUNTER__
#define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __COUNTER__)
#else
#define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __LINE__)
#endif

class UncaughtExceptionCounter {
  int getUncaughtExceptionCount() noexcept;
  int exceptionCount_;

 public:
  UncaughtExceptionCounter() : exceptionCount_(std::uncaught_exceptions()) {}
  bool hasNewUncaughtException() noexcept { return std::uncaught_exceptions() > exceptionCount_; }
};

template <typename FunctionType, bool executeOnException>
class ScopeGuardForNewException {
  FunctionType function_;
  UncaughtExceptionCounter ec_;

 public:
  explicit ScopeGuardForNewException(const FunctionType& fn) : function_(fn) {}
  explicit ScopeGuardForNewException(FunctionType&& fn) : function_(std::move(fn)) {}
  ~ScopeGuardForNewException() noexcept(executeOnException) {
    if (executeOnException == ec_.hasNewUncaughtException()) {
      function_();
    }
  }
};

namespace detail {
enum class ScopeGuardOnExit {};
template <typename Fun>
auto operator+(ScopeGuardOnExit, Fun&& fn) {
  return std::shared_ptr<void>(nullptr, std::forward<Fun>(fn));
}

enum class ScopeGuardOnFail {};
template <typename FunctionType>
ScopeGuardForNewException<typename std::decay<FunctionType>::type, true> operator+(
    detail::ScopeGuardOnFail,
    FunctionType&& fn) {
  return ScopeGuardForNewException<typename std::decay<FunctionType>::type, true>(
      std::forward<FunctionType>(fn));
}
}  // namespace detail

#define SCOPE_EXIT \
  const auto ANONYMOUS_VARIABLE(SCOPE_EXIT_STATE) = ::detail::ScopeGuardOnExit() + [&](auto)
#define SCOPE_FAIL \
  const auto ANONYMOUS_VARIABLE(SCOPE_FAIL_STATE) = ::detail::ScopeGuardOnFail() + [&]() noexcept
