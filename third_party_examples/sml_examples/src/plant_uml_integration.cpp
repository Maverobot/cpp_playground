// $CXX -std=c++14 plant_uml.cpp
// ./plant_uml_integration > example.uml && plantuml example.uml && feh -Z example.png

#include <cxxabi.h>
#include <boost/sml.hpp>
#include <cassert>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <typeinfo>

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

template <class... Ts>
std::string name(std::tuple<Ts...> tup) {
  const char* sep = "";
  std::ostringstream oss;
  (((oss << sep << demangle(typeid(std::decay_t<Ts>).name())), sep = ", "), ...);
  return oss.str();
}

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};

struct guard {
  bool operator()() const { return true; }
} guard;

struct action {
  void operator()() {}
} action;

struct another_action {
  bool operator()(double input) { return false; }
} another_action;

struct sub {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
        *"sub_idle"_s + event<e3> /action = "s1"_s,
        "s1"_s + event<e4> / action = X
    );
    // clang-format on
  }
};

struct plant_uml {
  auto operator()() const noexcept {
    using namespace sml;
    return make_transition_table(
        // clang-format off
        *"idle"_s + event<e1> = state<sub>,
        "idle"_s + event<e5> = state<sub>,
        state<sub> + event<e2> [ guard ] / (action, another_action)= "s2"_s,
        "s2"_s + event<e3> [ guard ] = "idle"_s,
        "s2"_s + event<e4> / action = X,

        *"idle2"_s + event<e2> = "s17"_s,
        "s17"_s + event<e3> = X
        // clang-format on
    );
  }
};

template <typename>
struct is_sub_state_machine : std::false_type {};

template <class T, class... Ts>
struct is_sub_state_machine<boost::sml::back::sm<boost::sml::back::sm_policy<T, Ts...>>>
    : std::true_type {};

template <typename T>
struct cleanable : std::false_type {};

template <class T, class... Ts>
struct cleanable<boost::sml::back::sm<boost::sml::back::sm_policy<T, Ts...>>> : std::true_type {};

template <class... Ts>
struct cleanable<boost::sml::front::seq_<boost::ext::sml::aux::zero_wrapper<Ts, void>...>>
    : std::true_type {};

template <typename T>
struct clean_action_name {
  using type = T;
};

template <class... Ts>
struct clean_action_name<boost::sml::front::seq_<boost::ext::sml::aux::zero_wrapper<Ts, void>...>> {
  using type = std::tuple<Ts...>;
};

template <typename T>
struct clean_state_name {
  using type = T;
};

template <class T, class... Ts>
struct clean_state_name<boost::sml::back::sm<boost::sml::back::sm_policy<T, Ts...>>> {
  using type = T;
};

using strset_t = std::set<std::string>;

template <class T>
void dump_transition(strset_t& substates_handled, int& starts) noexcept {
  auto src_state = std::string{sml::aux::string<typename T::src_state>{}.c_str()};
  auto dst_state = std::string{sml::aux::string<typename T::dst_state>{}.c_str()};

  if (dst_state == "terminate") {
    dst_state = "[*]";
  }

  // Clean the artifacts in the substate type names
  if constexpr (is_sub_state_machine<typename T::src_state>::value &&
                cleanable<typename T::src_state>::value) {
    src_state = name<typename clean_state_name<typename T::src_state>::type>();
  }
  if constexpr (is_sub_state_machine<typename T::dst_state>::value &&
                cleanable<typename T::dst_state>::value) {
    dst_state = name<typename clean_state_name<typename T::dst_state>::type>();
  }

  if (T::initial) {
    std::cout << (starts++ ? "--\n" : "") << "[*] --> " << src_state << std::endl;
  }

  if constexpr (is_sub_state_machine<typename T::dst_state>::value) {
    auto [loc, suc] = substates_handled.insert(dst_state);

    if (suc) {
      std::cout << "\nstate " << dst_state << " {\n";
      int new_starts{0};
      dump_transitions(substates_handled, new_starts, typename T::dst_state::transitions{});
      std::cout << "}\n";
    }
  }

  std::cout << src_state << " --> " << dst_state;

  const auto has_event = !sml::aux::is_same<typename T::event, sml::anonymous>::value;
  const auto has_guard = !sml::aux::is_same<typename T::guard, sml::front::always>::value;
  const auto has_action = !sml::aux::is_same<typename T::action, sml::front::none>::value;

  if (has_event || has_guard || has_action) {
    std::cout << " :";
  }

  if (has_event) {
    std::cout << " " << boost::sml::aux::get_type_name<typename T::event>();
  }

  if (has_guard) {
    std::cout << " [" << boost::sml::aux::get_type_name<typename T::guard::type>() << "]";
  }

  if (has_action) {
    if constexpr (cleanable<typename T::action::type>::value) {
      std::cout << " / " << name(typename clean_action_name<typename T::action::type>::type{});
    } else {
      std::cout << " / " << boost::sml::aux::get_type_name<typename T::action::type>();
    }
  }

  std::cout << std::endl;
}

template <template <class...> class T, class... Ts>
void dump_transitions(strset_t& substates_handled, int& starts, const T<Ts...>&) noexcept {
  int _[]{0, (dump_transition<Ts>(substates_handled, starts), 0)...};
  (void)_;
}

template <class SM>
void dump(const SM&) noexcept {
  std::set<std::string> substates_handled;  // guarantee only one dump per sub-machine
  int starts{0};                            // '--' is required between ortho states

  std::cout << "@startuml" << std::endl << std::endl;
  dump_transitions(substates_handled, starts, typename SM::transitions{});
  std::cout << std::endl << "@enduml" << std::endl;
}

int main() {
  sml::sm<plant_uml> sm;
  dump(sm);
}
