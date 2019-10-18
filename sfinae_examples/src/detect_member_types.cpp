#include <iostream>    // defines cout and endl
#include <type_traits> // defines true_type, false_type and void_t
#include <utility>

#include <vector>

#define DEFINE_HAS_TYPE(MemType)                                               \
  template <typename, typename = std::void_t<>>                                \
  struct HasTypeT_##MemType : std::false_type {};                              \
  template <typename T>                                                        \
  struct HasTypeT_##MemType<T, std::void_t<typename T::MemType>>               \
      : std::true_type {};

/*
  Tip:
    ## operator in macro takes two tokens and form them into a single token.
  Example:
    #define TypeInt i##nt
    TypeInt a; // same as "int a;"
 */

DEFINE_HAS_TYPE(value_type);
DEFINE_HAS_TYPE(char_type);

#define DEFINE_HAS_MEMBER(Member)                                              \
  template <typename, typename = std::void_t<>>                                \
  struct HasMemberT_##Member : std::false_type {};                             \
  template <typename T>                                                        \
  struct HasMemberT_##Member<T, std::void_t<decltype(T::Member)>>              \
      : std::true_type {};

DEFINE_HAS_MEMBER(size)
DEFINE_HAS_MEMBER(first)
DEFINE_HAS_MEMBER(begin)

int main(int argc, char *argv[]) {
  std::cout << std::boolalpha;

  // HasTypeT test
  std::cout << "int::value_type: " << HasTypeT_value_type<int>::value
            << std::endl;
  std::cout << "std::vector<double>::value_type: "
            << HasTypeT_value_type<std::vector<double>>::value << std::endl;

  std::cout << "std::io_stream::value_type: "
            << HasTypeT_value_type<std::iostream>::value << std::endl;
  std::cout << "std::io_stream::char_type: "
            << HasTypeT_char_type<std::iostream>::value << std::endl;

  std::cout << std::endl;

  // HasMemberT test
  std::cout << "std::vector<double>::size: "
            << HasMemberT_size<std::vector<double>>::value << std::endl;
  std::cout << "int::first: " << HasMemberT_first<int>::value << std::endl;
  std::cout << "std::pair<int,int>::first: "
            << HasMemberT_first<std::pair<int, int>>::value << std::endl;

  std::cout << "std::vector<double>::begin: "
            << HasMemberT_begin<std::vector<double>>::value << std::endl;

  return 0;
}
