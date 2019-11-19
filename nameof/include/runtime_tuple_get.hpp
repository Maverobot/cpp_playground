#pragma once

#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

template <typename Tuple,
          typename Indices =
              std::make_index_sequence<std::tuple_size<Tuple>::value>>
struct runtime_get_func_table;

template <typename Tuple, size_t... Indices>
struct runtime_get_func_table<Tuple, std::index_sequence<Indices...>> {
  using return_type = typename std::tuple_element<0, Tuple>::type &;
  using get_func_ptr = return_type (*)(Tuple &) noexcept;
  static constexpr get_func_ptr table[std::tuple_size<Tuple>::value] = {
      &std::get<Indices>...};
};

template <typename Tuple, size_t... Indices>
constexpr typename runtime_get_func_table<
    Tuple, std::index_sequence<Indices...>>::get_func_ptr
    runtime_get_func_table<Tuple, std::index_sequence<Indices...>>::table
        [std::tuple_size<Tuple>::value];

template <typename Tuple>
constexpr typename std::tuple_element<
    0, typename std::remove_reference<Tuple>::type>::type &
runtime_get(Tuple &&t, size_t index) {
  using tuple_type = typename std::remove_reference<Tuple>::type;
  if (index >= std::tuple_size<tuple_type>::value)
    throw std::runtime_error("Out of range");
  return runtime_get_func_table<tuple_type>::table[index](t);
}
