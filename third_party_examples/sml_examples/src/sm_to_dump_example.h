#pragma once

#include <boost/sml.hpp>

#include <iostream>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};

struct guard {
  bool operator()() const { return true; }
} guard;

inline static auto hook_action() {
  return []() { std::cout << "hook_action"; };
}

struct action {
  void operator()() {}
} action;

struct another_action {
  bool operator()(double input) { return false; }
} another_action;

struct special_action {
  bool operator()(double input) { return false; }
} special_action;

struct subsub {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
        *"subsub_idle"_s + on_entry<_> / action,
        "subsub_idle"_s + event<e3> /action = "s1"_s,
        "s1"_s + event<e4> / action = X
    );
    // clang-format on
  }
};

struct sub {
  auto operator()() const noexcept {
    using namespace sml;
    // clang-format off
    return make_transition_table(
        *"sub_idle"_s + event<e3> /action = state<subsub>,
        "subsub"_s + event<e4> / hook_action() = X
    );
    // clang-format on
  }
};

struct plant_uml {
  auto operator()() const noexcept {
    using namespace sml;
    return make_transition_table(
        // clang-format off
        *"idle"_s + on_entry<_> / action,
        "idle"_s + sml::on_exit<_> / another_action,
        "idle"_s + event<e1> = state<sub>,
        "idle"_s + event<e5> = state<sub>,
        state<sub> + event<e2> [ guard /*|| guard && guard*/ ] / (action, another_action, special_action)= "s2"_s,
        "sub1"_s + event<e2> [ guard ] / (action, another_action)= "s2"_s,
        "sub2"_s + event<e2> [ guard ] / action = "s2"_s,
        "s2"_s + event<e3> [ guard ] = "idle"_s,
        "s2"_s + event<e4> / []{std::cout << "received e4 in s2";} = X,

        *"idle2"_s + event<e2> = "s17"_s,
        "s17"_s + event<e3> = X
        // clang-format on
    );
  }
};
