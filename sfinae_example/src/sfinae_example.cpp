#include <iostream>    // for cout and endl
#include <type_traits> // for true_type and false_type

// SFINAE out partial specialization
template <typename...> using VoidT = void;
template <typename, typename = VoidT<>>
struct isDefaultConstructableT : std::false_type {};
template <typename T>
struct isDefaultConstructableT<T, VoidT<decltype(T())>> : std::true_type {};

/*
  Be aware: with the following code SFINAE will not work, since the second
  template will never be valid.

  template <typename, typename = void>
  struct isDefaultConstructableT : std::false_type {};
  template <typename T>
  struct isDefaultConstructableT<T, decltype(T())> : std::true_type {};

*/

// Test classes
struct NoConstructorClass {
  NoConstructorClass() = delete;
};
struct NormalClass {};

int main(int argc, char *argv[]) {
  if constexpr (std::is_convertible<isDefaultConstructableT<NormalClass>,
                                    std::true_type>::value) {
    std::cout << "NormalClass is default constructable" << std::endl;
  } else {
    std::cout << "NormalClass is not default constructable" << std::endl;
  }

  if constexpr (std::is_convertible<isDefaultConstructableT<NoConstructorClass>,
                                    std::true_type>::value) {
    std::cout << "NoConstructorClass is default constructable" << std::endl;
  } else {
    std::cout << "NoConstructorClass is not default constructable" << std::endl;
  }

  return 0;
}
