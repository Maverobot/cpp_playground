
#include <iostream>
#include <map>
#include <memory>

/* Why factory pattern?
 * - For virtual constructor, which does not exist natively in C++. You want to
 *   leave this exact knowledge up to another entity. For instance, instead of
 *   invoking "new" directly, you might call a virtual function "create"  of
 *   some higher-level object, thus allowing clients to change behavior through
 *   polymorphism.
 * - String -> type. You do have the type knowledge, but not in a form that is
 *   expressible in C++. For instance, you might have a string containing
 *   "Derived" , so you actually know you have to create an object of type
 *   "Derived", but you cannot pass a string containing a type name to "new"
 *   instead of a type name.
 *
 * Reference: Page 198, Modern C++ Design by Andrei Alexandrescu.
 **/

template <typename IdentifierType, typename AbstractProduct>
class DefaultFactoryError {
protected:
  static AbstractProduct *onUnknownType(const IdentifierType &id) {
    return nullptr;
  }
};

template <typename AbstractProduct, typename IdentifierType,
          typename ProductCreator = AbstractProduct *(*)(),
          template <typename, typename> class FactoryErrorPolicy =
              DefaultFactoryError>
class Factory : public FactoryErrorPolicy<IdentifierType, AbstractProduct> {
public:
  Factory() = default;
  bool registerCreator(const IdentifierType &id, ProductCreator creator) {
    return associations_.insert({id, creator}).second;
  };
  bool unregisterCreator(const IdentifierType &id) {
    return associations_.erase(id) == 1;
  }
  AbstractProduct *createObject(const IdentifierType &id) {
    auto i = associations_.find(id);
    if (i != associations_.end()) {
      return (i->second)();
    }
    return ErrorPolicy::onUnknownType(id);
  }

private:
  using AssocMap = std::map<IdentifierType, ProductCreator>;
  using ErrorPolicy = FactoryErrorPolicy<IdentifierType, AbstractProduct>;
  AssocMap associations_;
};

// Example dummy classes
struct Fruit {
  virtual void info() const {
    std::cout << "This is an abstract fruit" << std::endl;
  }
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

int main(int argc, char *argv[]) {
  Factory<Fruit, int> fruit_factory;
  fruit_factory.registerCreator(0, []() -> Fruit * { return new Apple(); });
  fruit_factory.registerCreator(1, []() -> Fruit * { return new Banana(); });
  fruit_factory.registerCreator(2, []() -> Fruit * { return new Pear(); });

  // Create apple, banana and pear
  std::unique_ptr<Fruit> apple_ptr(fruit_factory.createObject(0));
  std::unique_ptr<Fruit> banana_ptr(fruit_factory.createObject(1));
  std::unique_ptr<Fruit> pear_ptr(fruit_factory.createObject(2));

  // Check info
  apple_ptr->info();
  banana_ptr->info();
  pear_ptr->info();
  return 0;
}
