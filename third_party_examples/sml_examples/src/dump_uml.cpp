// $CXX -std=c++17 plant_uml.cpp
// ./dump_uml > hello.pu && plantuml . && feh -Z hello.png

#include "sm_to_dump_example.h"

#include <cxxabi.h>
#include <boost/sml.hpp>
#include <cassert>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <typeinfo>

namespace sml = boost::sml;

inline std::string demangle(const char* name) {
  int status = -4;  // some arbitrary value to eliminate the compiler warning

  // enable c++11 by passing the flag -std=c++11 to g++
  std::unique_ptr<char, void (*)(void*)> res{abi::__cxa_demangle(name, NULL, NULL, &status),
                                             std::free};
  return (status == 0) ? res.get() : name;
}

template <class T>
std::string name(T&&) {
  return demangle(typeid(std::decay_t<T>).name());
}

template <class T>
std::string name() {
  return demangle(typeid(std::decay_t<T>).name());
}

template <class T>
std::string base_name() {
  const std::string& full_name = name<T>();
  if (full_name.back() != '>') {
    return full_name.substr(full_name.find_last_of(':') + 1);
  }
  return full_name;
}

/**
 * Returns a string containing the list of demangled names of the given tuple element types.
 */
template <class T>
struct tuple_types_to_string {
  static std::string value(const char* delimiter = "") {
    static const std::string name_list = base_name<T>();
    return name_list;
  }
};
template <class... Ts>
struct tuple_types_to_string<std::tuple<Ts...>> {
  static std::string value(const char* delimiter = ", ") {
    const char* sep = "";
    std::ostringstream oss;
    ((((oss << sep << base_name<Ts>())), sep = delimiter), ...);
    static const std::string name_list = oss.str();
    return name_list;
  }
};

/**
 * Flattens a given nested tuple type. For example, it flattens `std::tuple<std::tuple<A, B>, C>` to
 * `std::tuple<A, B, C>`.
 */
template <typename T>
struct flatten_tuple_helper {
  using type = std::tuple<T>;
};
template <typename T>
using flatten_tuple_helper_t = typename flatten_tuple_helper<T>::type;
template <typename... Ts>
struct flatten_tuple_helper<std::tuple<Ts...>> {
  using type = decltype(std::tuple_cat(std::declval<flatten_tuple_helper_t<Ts>>()...));
};

template <typename T>
struct flatten_tuple {
  using type = T;
};
template <typename... Ts>
struct flatten_tuple<std::tuple<Ts...>> {
  using type = flatten_tuple_helper_t<std::tuple<Ts...>>;
};
template <typename T>
using flatten_tuple_t = typename flatten_tuple<T>::type;

/**
 * Checks if a given type is a sub state.
 */
template <typename>
struct is_sub_state_machine : std::false_type {};
template <class T, class... Ts>
struct is_sub_state_machine<boost::sml::back::sm<boost::sml::back::sm_policy<T, Ts...>>>
    : std::true_type {};

/**
 * Checks if a type should be cleaned.
 */
template <typename T>
struct cleanable : std::false_type {};
template <class T, class... Ts>
struct cleanable<boost::sml::back::sm<boost::sml::back::sm_policy<T, Ts...>>> : std::true_type {};

/**
 * Get the string representation of a given boost::sml guard type.
 */
template <typename T>
struct guard_name {
  static std::string name() { return base_name<T>(); }
  constexpr static bool is_leaf = true;
};

template <class TFirst, class TSecond>
struct guard_name<boost::sml::front::or_<boost::ext::sml::aux::zero_wrapper<TFirst, void>,
                                         boost::ext::sml::aux::zero_wrapper<TSecond, void>>> {
  static std::string name() {
    return std::string("(") + guard_name<TFirst>::name() + " || " + guard_name<TSecond>::name() +
           ")";
  }
  constexpr static bool is_leaf = false;
};
template <class TFirst, class TSecond>
struct guard_name<boost::sml::front::and_<boost::ext::sml::aux::zero_wrapper<TFirst, void>,
                                          boost::ext::sml::aux::zero_wrapper<TSecond, void>>> {
  static std::string name() {
    return guard_name<TFirst>::name() + " && " + guard_name<TSecond>::name();
  }
  constexpr static bool is_leaf = false;
};
template <typename T>
struct guard_name<boost::sml::front::not_<boost::ext::sml::aux::zero_wrapper<T, void>>> {
  static std::string name() {
    if constexpr (guard_name<T>::is_leaf) {
      return "!" + guard_name<T>::name();
    } else {
      return "!(" + guard_name<T>::name() + ")";
    }
  }
};

/**
 * Removes the type boilerplates of a given boost::sml action type.
 */
template <typename T>
struct clean_action_name {
  using type = T;
};
template <class TRest, class TEnd>
struct clean_action_name<boost::sml::front::seq_<boost::ext::sml::aux::zero_wrapper<TRest, void>,
                                                 boost::ext::sml::aux::zero_wrapper<TEnd, void>>> {
  using type = std::tuple<typename clean_action_name<TRest>::type, TEnd>;
};
template <typename T>
using clean_action_name_t = typename clean_action_name<T>::type;

/**
 * Removes the type boilerplates of a given boost::sml state type.
 */
template <typename T>
struct clean_state_name {
  using type = T;
};
template <class T, class... Ts>
struct clean_state_name<boost::sml::back::sm<boost::sml::back::sm_policy<T, Ts...>>> {
  using type = T;
};

class UMLDumper {
 public:
  UMLDumper(size_t max_depth = 0) : max_depth_(max_depth) {}

  template <class SM>
  void dump(const SM&) noexcept {
    std::set<std::string> substates_handled;  // guarantee only one dump per sub-machine
    int starts{0};                            // '--' is required between ortho states

    std::cout << "@startuml" << std::endl << std::endl;
    dump_transitions(substates_handled, starts, 0, typename SM::transitions{});
    std::cout << std::endl << "@enduml" << std::endl;
  }

 private:
  size_t max_depth_;

  using strset_t = std::set<std::string>;

  template <class T>
  void dump_transition(strset_t& substates_handled, int& starts, size_t depth) noexcept {
    auto src_state = std::string{sml::aux::string<typename T::src_state>{}.c_str()};
    auto dst_state = std::string{sml::aux::string<typename T::dst_state>{}.c_str()};

    if (dst_state == "terminate") {
      dst_state = "[*]";
    }

    // Clean the artifacts in the substate type names
    if constexpr (is_sub_state_machine<typename T::src_state>::value &&
                  cleanable<typename T::src_state>::value) {
      src_state = base_name<typename clean_state_name<typename T::src_state>::type>();
    }
    if constexpr (is_sub_state_machine<typename T::dst_state>::value &&
                  cleanable<typename T::dst_state>::value) {
      dst_state = base_name<typename clean_state_name<typename T::dst_state>::type>();
    }

    if (T::initial) {
      std::cout << (starts++ ? "--\n" : "") << "[*] --> " << src_state << std::endl;
    }

    if constexpr (is_sub_state_machine<typename T::dst_state>::value) {
      auto [loc, suc] = substates_handled.insert(dst_state);

      if (suc) {
        std::cout << "\nstate " << dst_state << " {\n";
        int new_starts{0};
        dump_transitions(substates_handled, new_starts, depth + 1,
                         typename T::dst_state::transitions{});
        std::cout << "}\n";
      }
    }

    const auto has_event = !sml::aux::is_same<typename T::event, sml::anonymous>::value;
    const auto has_guard = !sml::aux::is_same<typename T::guard, sml::front::always>::value;
    const auto has_action = !sml::aux::is_same<typename T::action, sml::front::none>::value;

    const auto is_entry =
        sml::aux::is_same<typename T::event, sml::back::on_entry<sml::_, sml::_>>::value;
    const auto is_exit =
        sml::aux::is_same<typename T::event, sml::back::on_exit<sml::_, sml::_>>::value;

    const auto has_destination =
        !sml::aux::is_same<typename T::dst_state, sml::front::internal>::value;

    std::cout << src_state;

    if (has_destination) {
      std::cout << " --> " << dst_state;
    }

    if (has_event || has_guard || has_action) {
      std::cout << " :";
    }

    if (has_event) {
      auto event = base_name<typename T::event>();
      if (is_entry) {
        event = "on_entry";
      } else if (is_exit) {
        event = "on_exit";
      }
      std::cout << " " << event;
    }

    if (has_guard) {
      std::cout << " [" << guard_name<typename T::guard::type>::name() << "]";
    }

    if (has_action) {
      std::cout << " / "
                << tuple_types_to_string<
                       flatten_tuple_t<clean_action_name_t<typename T::action::type>>>::value();
    }

    std::cout << std::endl;
  }

  template <template <class...> class T, class... Ts>
  void dump_transitions(strset_t& substates_handled,
                        int& starts,
                        size_t depth,
                        const T<Ts...>&) noexcept {
    if (depth > max_depth_) {
      return;
    }
    int _[]{0, (dump_transition<Ts>(substates_handled, starts, depth), 0)...};
    (void)_;
  }
};

class C1 {};
class C2 {};
class C3 {};
class C4 {};
class C5 {};

int main() {
  sml::sm<some::extra::ns::plant_uml> sm;

  UMLDumper dumper(1);
  dumper.dump(sm);
}
