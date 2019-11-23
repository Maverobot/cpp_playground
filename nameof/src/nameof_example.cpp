
#include <index_of_type.hpp>
#include <indices_trick_std.h>
#include <nameof.hpp>

#include <algorithm>
#include <iostream>
#include <tuple>
// Task 1: Concatenate types
template <typename...> struct concat;

template <template <typename...> class TypeList, typename... Ts,
          typename... TsNew>
struct concat<TypeList<Ts...>, TsNew...> {
  using type = TypeList<Ts..., TsNew...>;
};

template <template <typename...> class TypeList, typename... Ts, typename T>
struct concat<T, TypeList<Ts...>> {
  using type = TypeList<T, Ts...>;
};

template <template <typename...> class TypeList, typename... T1s,
          typename... T2s>
struct concat<TypeList<T1s...>, TypeList<T2s...>> {
  using type = TypeList<T1s..., T2s...>;
};

// Task 2: Try to use type traits to exact unique types from a type list
template <typename...> struct unique_types;

template <typename T, typename... Ts> struct type_in_list {
  static constexpr bool value =
      sizeof...(Ts) != 0 && ((std::is_same_v<T, Ts>) || ...);
};

template <template <typename...> class Tlist, class T, class... Trest>
struct unique_types<Tlist<T, Trest...>> {
  using type = std::conditional_t<
      type_in_list<T, Trest...>::value,
      typename unique_types<Tlist<Trest...>>::type,
      typename concat<T, typename unique_types<Tlist<Trest...>>::type>::type>;
};

template <template <typename...> class Tlist> struct unique_types<Tlist<>> {
  using type = Tlist<>;
};

template <typename...> struct unique_type_list;
template <template <typename...> class Tlist, typename... Ts>
struct unique_type_list<Tlist<Ts...>> {
  using type = typename unique_types<Tlist<Ts...>>::type;
};

int main(int, char **) {
  using type1 = concat<concat<>, double>::type;
  using type2 = concat<type1, int>::type;
  using type3 = concat<type2, double, float>::type;
  using type4 = concat<type3, float, double, int, void>::type;

  std::cout << "  type1: " << nameof::nameof_type<type1>() << std::endl;
  std::cout << "unique1: "
            << nameof::nameof_type<unique_type_list<type1>::type>() << std::endl
            << std::endl;

  std::cout << "  type2: " << nameof::nameof_type<type2>() << std::endl;
  std::cout << "unique2: "
            << nameof::nameof_type<unique_type_list<type2>::type>() << std::endl
            << std::endl;

  std::cout << "  type3: " << nameof::nameof_type<type3>() << std::endl;
  std::cout << "unique3: "
            << nameof::nameof_type<unique_type_list<type3>::type>() << std::endl
            << std::endl;

  std::cout << "  type4: " << nameof::nameof_type<type4>() << std::endl;
  std::cout << "unique4: "
            << nameof::nameof_type<unique_type_list<type4>::type>()
            << std::endl;
}
