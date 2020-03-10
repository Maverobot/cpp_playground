#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <variant>

/**
 * Factory pattern with variant
 **/

// Behavior on unknown type is throwing exception
template <typename IdentifierType, typename ProductVariant>
class FactoryErrorException {
 protected:
  static ProductVariant onUnknownType(const IdentifierType& id) {
    throw std::invalid_argument("Unknown object type passed to Factory");
  }
};

// Default behavior on unknown type is returning nullptr

template <typename ProductVariant,
          typename IdentifierType,
          typename ProductCreator = std::function<ProductVariant()>,
          template <typename, typename> class FactoryErrorPolicy = FactoryErrorException>
class Factory : private FactoryErrorPolicy<IdentifierType, ProductVariant> {
 public:
  Factory() = default;
  bool registerCreator(const IdentifierType& id, ProductCreator creator) {
    return associations_.insert({id, creator}).second;
  };
  bool unregisterCreator(const IdentifierType& id) { return associations_.erase(id) == 1; }
  ProductVariant createObject(const IdentifierType& id) {
    auto i = associations_.find(id);
    if (i != associations_.end()) {
      return (i->second)();
    }
    return ErrorPolicy::onUnknownType(id);
  }

 private:
  using AssocMap = std::map<IdentifierType, ProductCreator>;
  using ErrorPolicy = FactoryErrorPolicy<IdentifierType, ProductVariant>;
  AssocMap associations_;
};

// Example dummy classes
struct Fruit {
  virtual void info() const { std::cout << "This is an abstract fruit" << std::endl; }
};

struct Apple : Fruit {
  void info() const override { std::cout << "This is an apple" << std::endl; }
};
struct Banana : Fruit {
  void info() const override { std::cout << "This is a Banana" << std::endl; }
};
struct Pear : Fruit {
  void info() const override { std::cout << "This is a Pear" << std::endl; }
};

int main(int argc, char* argv[]) {
  using Fruit = std::variant<Apple, Banana, Pear>;

  // Prepare factory
  Factory<Fruit, int> fruit_factory;
  fruit_factory.registerCreator(0, []() -> Apple { return Apple(); });
  fruit_factory.registerCreator(1, []() -> Banana { return Banana(); });
  fruit_factory.registerCreator(2, []() -> Pear { return Pear(); });

  // Create apple, banana, pear and some unknown fruit
  Fruit apple(fruit_factory.createObject(0));
  Fruit banana(fruit_factory.createObject(1));
  Fruit pear(fruit_factory.createObject(2));
  // Fruit unknown_fruit(fruit_factory.createObject(3));

  //
  constexpr auto info = [](const auto& fruit) {
    std::visit([](const auto& f) { f.info(); }, fruit);
    ;
  };

  // Check info
  info(apple);
  info(banana);
  info(pear);

  // if (!unknown_fruit) {
  //   std::cout << "This fruit does not exist." << std::endl;
  // }

  // Factory with exception
  // Factory<Fruit, int, std::function<Fruit*()>, FactoryErrorException> fruit_factory_exc;
  // std::unique_ptr<Fruit> unknown_fruit2_ptr(fruit_factory_exc.createObject(99));

  return 0;
}
