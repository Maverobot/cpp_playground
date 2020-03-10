#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <variant>

#include "nameof.hpp"

/**
 * Factory pattern with variant
 **/

template <typename ProductVariant>
class Factory {
 public:
  using IdentifierType = std::string;
  using ProductCreator = std::function<ProductVariant()>;

  Factory() { MapCreator<ProductVariant>()(associations_); }

  ProductVariant createObject(const IdentifierType& id) {
    auto i = associations_.find(id);
    if (i != associations_.end()) {
      return (i->second)();
    }

    std::ostringstream oss;
    oss << "Unknown object type ID " << id << ". ";
    oss << "Registed type IDs are: ";
    for (const auto& pair : associations_) {
      oss << "\n\t" << pair.first;
    }
    throw std::invalid_argument(oss.str());
  }

 private:
  using AssocMap = std::map<IdentifierType, ProductCreator>;
  AssocMap associations_;

  template <typename... T>
  struct MapCreator;
  template <typename... T>
  struct MapCreator<std::variant<T...>> {
    void operator()(AssocMap& map) { ((void)insert<T>(map), ...); };

    template <typename NewT>
    void insert(AssocMap& map) {
      map.insert({std::string(nameof::nameof_type<NewT>()), [] { return NewT(); }});
    }

    // Remove namespace
    std::string getBaseName(const std::string_view& name) {
      return std::string(name.substr(name.find_last_of(':') + 1));
    }
  };
};

// Example dummy classes
struct Apple {
  void info() const { std::cout << "This is an apple" << std::endl; }
};
struct Banana {
  void info() const { std::cout << "This is a banana" << std::endl; }
};
struct Pear {
  void info() const { std::cout << "This is a pear" << std::endl; }
};

namespace fruit_ns {
struct Apple {
  void info() const { std::cout << "This is an apple in fruit_ns" << std::endl; }
};
}  // namespace fruit_ns

int main(int argc, char* argv[]) {
  using Fruit = std::variant<Apple, Banana, Pear, fruit_ns::Apple>;

  // Prepare factory
  Factory<Fruit> fruit_factory;

  // Create apple, banana, pear and some unknown fruit
  Fruit apple(fruit_factory.createObject("Apple"));
  Fruit banana(fruit_factory.createObject("Banana"));
  Fruit pear(fruit_factory.createObject("Pear"));
  Fruit apple_in_ns(fruit_factory.createObject("fruit_ns::Apple"));

  constexpr auto info = [](const auto& fruit) {
    std::visit([](const auto& f) { f.info(); }, fruit);
    ;
  };

  // Check info
  info(apple);
  info(banana);
  info(pear);
  info(apple_in_ns);

  Fruit unknown_fruit(fruit_factory.createObject("SomeUnknowFruit"));

  return 0;
}
