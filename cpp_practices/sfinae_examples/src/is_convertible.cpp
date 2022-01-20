#include <iostream>     // for cout and endl
#include <type_traits>  // for true_type and false_type
#include <utility>      // for declval

/*
  Tip:
    std::declval<T>() "vitually" returns the value of a certain type T
  without needing the existence of constructors.
 */

// Helper class
template <typename FROM, typename TO>
struct IsConvertibleHelper {
 private:
  // test() trying to call the helper aux(TO) for a FROM passed as F.
  static void aux(TO);
  template <typename F, typename = decltype(aux(std::declval<F>()))>
  static std::true_type test(void*);
  // test() fallback.
  template <typename>
  static std::false_type test(...);

 public:
  using Type = decltype(test<FROM>(nullptr));
  static constexpr bool value = std::is_same<Type, std::true_type>::value;
};

// Final interface
template <typename FROM, typename TO>
using IsConvertible = typename IsConvertibleHelper<FROM, TO>::Type;
template <typename FROM, typename TO>
constexpr bool isConvertible = IsConvertibleHelper<FROM, TO>::value;

// Test classes
struct Base {};
struct Derived : Base {};
struct LonelyClass {};

int main(int argc, char* argv[]) {
  std::cout << std::boolalpha;
  std::cout << "Derived -> Base convertible: " << isConvertible<Derived, Base> << std::endl;
  std::cout << "LonelyClass -> Base convertible: " << isConvertible<LonelyClass, Base> << std::endl;
  std::cout << "int -> int convertible: " << isConvertible<int, int> << std::endl;
  std::cout << "int -> std::string convertible: " << isConvertible<int, std::string> << std::endl;
  std::cout << "char const* -> std::string convertible: "
            << isConvertible<char const*, std::string> << std::endl;
  std::cout << "std::string -> char const* convertible: "
            << isConvertible<std::string, char const*> << std::endl;
  return 0;
}
