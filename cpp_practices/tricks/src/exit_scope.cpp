#include <functional>
#include <iostream>
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

#define xstr(s) str(s)
#define str(s) #s

namespace detail {
enum class ScopeGuardOnExit {};
template <typename Fun>
auto operator+(ScopeGuardOnExit, Fun&& fn) {
  return std::shared_ptr<void>(nullptr, std::forward<Fun>(fn));
}
}  // namespace detail
#define SCOPE_EXIT \
  const auto ANONYMOUS_VARIABLE(SCOPE_EXIT_STATE) = ::detail::ScopeGuardOnExit() + [&](auto)

int main(int argc, char* argv[]) {
  SCOPE_EXIT { std::cout << "scope exit1 \n"; };
  SCOPE_EXIT { std::cout << "scope exit2 \n"; };
  std::cout << "scope end\n";
  return 0;
}
