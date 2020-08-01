#include <iostream>
#include <string_view>

#include "boost/di.hpp"

namespace di = boost::di;

template <typename T, typename Tag>
class StrongTyped {
 public:
  StrongTyped(T const& value) : value_(value) {}
  StrongTyped(T&& value) : value_(std::move(value)) {}

  operator T() const { return value_; }

  T& get() { return value_; }
  T const& get() const { return value_; }

 private:
  T value_;
};

struct Speaker {
  using Name = StrongTyped<std::string_view, struct SpeakerName>;
  Speaker(Name name) : name(name) {}
  std::string name;
  void speak() const { std::cout << name << " speaks.\n"; }
};

struct Audience {
  using Names = StrongTyped<std::vector<std::string>, struct AudienceNames>;
  Audience(Names names) : names(std::move(names)) {}
  std::vector<std::string> names;
  void listen() const {
    for (const auto& name : names) {
      std::cout << name << " ";
    }

    std::cout << "listen.\n";
  }
};

struct CppConTalk {
  using SpecialMessage = StrongTyped<std::string_view, struct CppConTalkSpecialMessage>;
  CppConTalk(Speaker& speaker, Audience& audience, SpecialMessage special_message)
      : speaker(speaker), audience(audience), message(special_message) {}

  void run() {
    speaker.speak();
    audience.listen();
    std::cout << "Special message: " << message.get() << "\n";
  }

  Speaker& speaker;
  Audience& audience;
  SpecialMessage message;
};

int main(int argc, char* argv[]) {
  auto injector = di::make_injector(
      di::bind<Speaker::Name>().to<Speaker::Name>({"Kris"}),
      di::bind<CppConTalk::SpecialMessage>().to<CppConTalk::SpecialMessage>({"Hello World"}),
      di::bind<Audience::Names>().to<Audience::Names>({{"Paul", "Daniel", "Alex"}}));
  auto cpp_con_talk = injector.create<CppConTalk>();
  cpp_con_talk.run();

  return 0;
}
