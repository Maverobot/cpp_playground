
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <typeindex>

#include "type.hpp"

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

// Behavior on unknown type is returning nullptr
template <typename IdentifierType, typename AbstractProduct>
class FactoryErrorNullptr {
protected:
  static AbstractProduct *onUnknownType(const IdentifierType &id) {
    return nullptr;
  }
};

// Behavior on unknown type is returning nullptr
template <typename IdentifierType, typename AbstractProduct>
class FactoryErrorException {
protected:
  static AbstractProduct *onUnknownType(const IdentifierType &id) {
    throw std::invalid_argument("Unknown object type passed to Factory");
  }
};

// A generic function pointer which can be used to save any function
typedef void (*VoidFunctionType)(void);

template <typename AbstractProduct, typename IdentifierType,
          template <typename, typename> class FactoryErrorPolicy =
              FactoryErrorNullptr>
class Factory : private FactoryErrorPolicy<IdentifierType, AbstractProduct> {
public:
  Factory() = default;

  template <typename T>
  bool registerCreator(const IdentifierType &id, T creator) {
    auto creator_pair =
        std::make_pair(reinterpret_cast<VoidFunctionType>(+creator),
                       std::type_index(typeid(+creator)));
    auto it = associations_.find(id);
    // There exists creator function for this id
    if (it != associations_.cend()) {
      // Add the new creator function as candidate
      if (it->second.add(creator_pair)) {
        return true;
      }
      throw std::runtime_error(
          "the creator function with same signature already exists.");
    }
    return associations_.insert(std::make_pair(id, FunctionTypes(creator_pair)))
        .second;
  };
  bool unregisterCreator(const IdentifierType &id) {
    return associations_.erase(id) == 1;
  }

  template <typename... Args>
  AbstractProduct *createObject(const IdentifierType &id,
                                Args &&... args) const {
    auto i = associations_.find(id);
    if (i != associations_.end()) {
      auto given_signature =
          std::type_index(typeid(AbstractProduct * (*)(Args...)));
      auto generic_func = i->second.get(given_signature, args...);
      auto creator_func =
          reinterpret_cast<AbstractProduct *(*)(Args...)>(generic_func);
      return creator_func(std::forward<Args>(args)...);
    }
    return ErrorPolicy::onUnknownType(id);
  }

private:
  class FunctionTypes {
  public:
    using FunctionType = std::pair<VoidFunctionType, std::type_index>;

    explicit FunctionTypes(FunctionType type) {
      fs_.push_back(std::move(type));
    }

    bool add(FunctionType function_type) {
      // Add if the type does not exist yet
      if (std::find_if(fs_.cbegin(), fs_.cend(), [&function_type](auto &x) {
            return x.second == function_type.second;
          }) != fs_.cend()) {
        return false;
      }
      fs_.push_back(std::move(function_type));
      return true;
    }

    template <typename... Args>
    VoidFunctionType get(const std::type_index &signature, Args... args) const {
      for (auto func_type : fs_) {
        auto saved_signature = func_type.second;
        if (signature == saved_signature) {
          return func_type.first;
        }
      }
      throw std::runtime_error("no matching function.");
    }

  private:
    std::vector<FunctionType> fs_;
  };

  using AssocMap = std::map<IdentifierType, FunctionTypes>;
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
  Apple() = default;
  explicit Apple(int age) {
    std::cout << "created an apple with age of " << age << std::endl;
  }
  explicit Apple(std::string info) {
    std::cout << "created an apple with info: " << info << std::endl;
  }
  void info() const override { std::cout << "This is an apple" << std::endl; }
};
struct Banana : Fruit {
  void info() const override { std::cout << "This is a Banana" << std::endl; }
};
struct Pear : Fruit {
  void info() const override { std::cout << "This is a Pear" << std::endl; }
};

int main(int argc, char *argv[]) {
  // Prepare factory
  Factory<Fruit, std::string> fruit_factory;

  // 3 Apple creators
  fruit_factory.registerCreator("Apple",
                                []() -> Fruit * { return new Apple(); });

  fruit_factory.registerCreator(
      "Apple", [](int age) -> Fruit * { return new Apple(age); });

  fruit_factory.registerCreator(
      "Apple", [](std::string info) -> Fruit * { return new Apple(info); });

  // 1 Banana creator
  fruit_factory.registerCreator("Banana",
                                []() -> Fruit * { return new Banana(); });

  // 1 Pear creator
  fruit_factory.registerCreator("Pear", []() -> Fruit * { return new Pear(); });

  // Create apple, banana, pear and some unknown fruit
  std::unique_ptr<Fruit> apple_ptr(fruit_factory.createObject("Apple"));
  std::unique_ptr<Fruit> apple_ptr2(fruit_factory.createObject("Apple", 8));
  std::unique_ptr<Fruit> apple_ptr3(
      fruit_factory.createObject("Apple", std::string("awesome")));
  std::unique_ptr<Fruit> banana_ptr(fruit_factory.createObject("Banana"));
  std::unique_ptr<Fruit> pear_ptr(fruit_factory.createObject("Pear"));
  std::unique_ptr<Fruit> unknown_fruit_ptr(
      fruit_factory.createObject("Unknown"));

  // Check info
  apple_ptr->info();
  banana_ptr->info();
  pear_ptr->info();

  if (!unknown_fruit_ptr) {
    std::cout << "This fruit does not exist." << std::endl;
  }

  // Factory with exception
  Factory<Fruit, int, FactoryErrorException> fruit_factory_exc;
  std::unique_ptr<Fruit> unknown_fruit2_ptr(fruit_factory_exc.createObject(99));

  return 0;
}
