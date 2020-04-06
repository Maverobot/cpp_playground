#pragma once

#include <algorithm>
#include <functional>
#include <tuple>
#include <utility>
#include <vector>

namespace detail {
template <typename type_t, class orig_t>
struct unwrap_impl {
  using type = orig_t;
};

template <typename type_t, class V>
struct unwrap_impl<std::reference_wrapper<type_t>, V> {
  using type = type_t;
};

template <class T>
struct unwrap {
  using type = typename detail::unwrap_impl<std::decay_t<T>, T>::type;
};

template <typename type_t>
using unwrap_t = typename unwrap<type_t>::type;
}  // namespace detail

template <typename container_t>
auto range(container_t& container) {
  return std::tuple{std::begin(container), std::end(container)};
};
template <typename container_t>
auto range(const container_t& container) {
  return std::tuple{std::begin(container), std::end(container)};
};

template <typename container_t, typename predicate_t>
auto where(const container_t& container, predicate_t predicate) {
  auto [b, e] = range(container);
  using type = detail::unwrap_t<std::remove_reference_t<std::remove_const_t<decltype(*b)>>>;
  using reference = std::reference_wrapper<type>;

  std::vector<reference> result{};

  std::copy_if(b, e, std::back_inserter(result), predicate);
  return result;
}
