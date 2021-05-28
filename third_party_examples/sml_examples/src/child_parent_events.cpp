#include <boost/sml.hpp>
#include <cassert>
#include <cstdio>

namespace sml = boost::sml;

struct release {
  bool child;
};
struct timeout {};

const auto release_child = [](const release& r) { return r.child; };

struct SimpleLogger {
  template <class SM, class TEvent>
  void log_process_event(const TEvent&) {
    printf("[%s][process_event] %s\n", sml::aux::get_type_name<SM>(),
           sml::aux::get_type_name<TEvent>());
  }

  template <class SM, class TGuard, class TEvent>
  void log_guard(const TGuard&, const TEvent&, bool result) {
    printf("[%s][guard] %s %s %s\n", sml::aux::get_type_name<SM>(),
           sml::aux::get_type_name<TGuard>(), sml::aux::get_type_name<TEvent>(),
           (result ? "[OK]" : "[Reject]"));
  }

  template <class SM, class TAction, class TEvent>
  void log_action(const TAction&, const TEvent&) {
    printf("[%s][action] %s %s\n", sml::aux::get_type_name<SM>(),
           sml::aux::get_type_name<TAction>(), sml::aux::get_type_name<TEvent>());
  }

  template <class SM, class TSrcState, class TDstState>
  void log_state_change(const TSrcState& src, const TDstState& dst) {
    printf("[%s][transition] %s -> %s\n", sml::aux::get_type_name<SM>(), src.c_str(), dst.c_str());
  }
};

struct parent {
  struct child {};
  auto operator()() const {
    using namespace sml;
    return make_transition_table(
        *sml::state<child> + event<release>[release_child] = "intermediate"_s,
        "intermediate"_s + event<timeout> = X);
  }
};

struct hello_world {
  auto operator()() const {
    using namespace sml;
    return make_transition_table(*sml::state<parent> + event<release> = X);
  }
};

int main() {
  using namespace sml;

  SimpleLogger simple_logger;

  {
    printf("==========================================\n");
    printf("\033[32;1mchild processes events before parent. \033[0m\n");
    sm<hello_world, logger<SimpleLogger> > sm(simple_logger);
    sm.process_event(release{.child = true});
    assert(sm.is(state<parent>));                              // released
    assert(sm.is<decltype(state<parent>)>("intermediate"_s));  // released
  }

  {
    printf("==========================================\n");
    printf(
        "\033[32;1mparent processes the same events which is rejected by guard in child. "
        "\033[0m\n");
    sm<hello_world, logger<SimpleLogger> > sm(simple_logger);
    sm.process_event(release{.child = false});
    assert(sm.is(X));  // released
  }
}
