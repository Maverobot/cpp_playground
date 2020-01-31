#include <variant>

template <typename... Ts, std::size_t... Is>
std::variant<Ts...> get_impl(std::size_t index, std::index_sequence<Is...>,
                             const std::tuple<Ts...> &t) {
  using getter_type = std::variant<Ts...> (*)(const std::tuple<Ts...> &);
  getter_type funcs[] = {+[](const std::tuple<Ts...> &tuple)
                             -> std::variant<Ts...>{return std::get<Is>(tuple);
}
...
}
;

return funcs[index](t);
}

template <typename... Ts>
std::variant<Ts...> get(std::size_t index, const std::tuple<Ts...> &t) {
  return get_impl(index, std::index_sequence_for<Ts...>(), t);
}
