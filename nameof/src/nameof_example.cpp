#include "nameof.hpp"
#include <iostream>
#include <tuple>

class ClassA {};
class ClassB {};
class ClassC {};

// Index of type in tuple
template <class T, class Tuple> struct Index;
template <class T, class... Types> struct Index<T, std::tuple<T, Types...>> {
  static constexpr std::size_t value = 0;
};
template <class T, class U, class... Types>
struct Index<T, std::tuple<U, Types...>> {
  static constexpr std::size_t value =
      1 + Index<T, std::tuple<Types...>>::value;
};
template <typename T, typename Tuple>
constexpr int idx = Index<T, Tuple>::value;

// Type of index in tuple
template <class... Args> struct type_list {
  template <std::size_t N>
  using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
};

template <typename... TypeList> struct TypeMap {
  // Define types and names
  using types_t = std::tuple<TypeList...>;
  using names_t = std::tuple<decltype(nameof::nameof_type<TypeList>())...>;
  names_t names = names_t{nameof::nameof_type<TypeList>()...};
  // getName is probably redundant to nameof::nameof_type
  template <typename T> auto getName() {
    return std::get<Index<T, types_t>::value>(names);
  }

  // Probably wrong here
  template <typename T>
  using getType = typename std::tuple_element<idx<T, names_t>,
                                              std::tuple<TypeList...>>::type;
};

int main(int, char **) {

  using T = const int &;
  T var = 42;
  // Name of variable type.
  std::cout << NAMEOF_TYPE_EXPR(var) << std::endl;
  std::cout << nameof::nameof_type<decltype(var)>() << std::endl;
  std::cout << nameof::nameof_full_type<decltype(var)>() << std::endl;

  // Name of type.
  std::cout << NAMEOF_TYPE(T) << std::endl;
  std::cout << NAMEOF_FULL_TYPE(T) << std::endl;
  std::cout << nameof::nameof_type<T>() << std::endl;
  std::cout << nameof::nameof_full_type<T>() << std::endl;

  auto type_names = TypeMap<ClassA, ClassB, ClassC>().names;

  std::apply(
      [](const auto &... names) { ((std::cout << names << std::endl), ...); },
      type_names);

  using type_map = TypeMap<ClassA, ClassB, ClassC>;

  ;
  // type_map::getType<nameof::cstring<6>("ClassA")> class_a_object;
}
