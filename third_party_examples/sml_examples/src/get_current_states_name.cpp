//
// Copyright (c) 2016-2020 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/sml.hpp>
#include <cassert>
#include <iostream>
#include <memory>

namespace sml = boost::sml;

struct e1 {};
struct e2 {};
struct e3 {};
struct e4 {};
struct e5 {};

struct sub {
  auto operator()() const {
    using namespace sml;
    // clang-format off
    return make_transition_table(
     *"idle"_s + event<e3> = "sub1"_s
    , "sub1"_s + event<e4> = X
    );
    // clang-format on
  }
};

struct composite {
  auto operator()() const {
    using namespace sml;
    // clang-format off
    return make_transition_table(
     *"idle"_s   + event<e1> = "s1"_s
    , "s1"_s     + event<e2> = state<sub>
    , state<sub> + event<e5> = X
    );
    // clang-format on
  }
};

/**
 * Note that this class does not support getting names from orthogonal regions. When used, the
 * format of the state name will look strange.
 */
template <class TSM>
class state_name_visitor {
 public:
  explicit state_name_visitor(const TSM& sm) : sm_{sm} {}

  template <class TSub>
  void operator()(boost::sml::aux::string<boost::sml::sm<TSub>>) const {
    state_name_ += std::string(boost::sml::aux::get_type_name<TSub>()) + ":";
    sm_.template visit_current_states<boost::sml::aux::identity<TSub>>(*this);
  }

  template <class TState>
  void operator()(TState state) const {
    state_name_ += state.c_str();
  }

  std::string get() {
    std::shared_ptr<void> _(nullptr, [this](auto) { state_name_.clear(); });
    return state_name_;
  }

 private:
  const TSM& sm_;
  mutable std::string state_name_;
};

template <class TSM>
std::string get_current_states_name(const TSM& sm) {
  auto visitor = state_name_visitor<decltype(sm)>{sm};
  sm.visit_current_states(visitor);
  return visitor.get();
}

int main() {
  sml::sm<composite> sm{};

  sm.process_event(e1{});
  std::cout << get_current_states_name(sm) << std::endl;  // s1

  sm.process_event(e2{});
  std::cout << get_current_states_name(sm) << std::endl;  // sub:idle

  sm.process_event(e3{});
  std::cout << get_current_states_name(sm) << std::endl;  // sub:sub1

  sm.process_event(e4{});
  std::cout << get_current_states_name(sm) << std::endl;  // sub::terminate

  sm.process_event(e5{});
  std::cout << get_current_states_name(sm) << std::endl;  // terminate

  return 0;
}
