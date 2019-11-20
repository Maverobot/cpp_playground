
#include <index_of_type.hpp>
#include <indices_trick_std.h>
#include <nameof.hpp>

#include <algorithm>
#include <iostream>
#include <tuple>

class ClassA {};
class ClassB {};
class ClassC {};

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

template <typename T> struct value_match {
  value_match(T target) : target(target){};
  template <typename U> bool operator()(U &&value) const {
    if constexpr (std::is_convertible_v<std::decay_t<T>, std::string_view> &&
                  std::is_convertible_v<std::decay_t<U>, std::string_view>) {
      if (value == target) {
        std::cout << "same type, same value" << std::endl;
        return true;
      }
      std::cout << "same type, different value" << std::endl;
      return false;
    } else {
      std::cout << "different type: " << typeid(T).name() << " vs "
                << typeid(U).name() << std::endl;
      return false;
    }
  }
  T target;
};

template <typename Tuple, typename T>
int getIndexByValue(Tuple &&tup, T &&value) {
  auto flags = indices_trick_std::execute_all(
      value_match(std::forward<T>(value)), std::forward<Tuple>(tup));
  auto iter =
      std::find_if(flags.cbegin(), flags.cend(), [](auto &v) { return v; });
  if (iter != flags.cend()) {
    return std::distance(flags.cbegin(), iter);
  }
  return -1;
}

int main(int, char **) {
  auto type_names = TypeMap<ClassA, ClassB, ClassC>().names;

  std::string test_name("ClassA");

  std::apply(
      [](const auto &... names) { ((std::cout << names << std::endl), ...); },
      type_names);

  std::cout << getIndexByValue(type_names, "ClassA") << std::endl;
  std::cout << getIndexByValue(type_names, "ClassB") << std::endl;
  std::cout << getIndexByValue(type_names, "ClassC") << std::endl;
  ;
}
