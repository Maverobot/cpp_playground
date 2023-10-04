#include <iostream>

class AnotherBase {
  virtual void update() = 0;
};

template <typename T, template <typename> class CrtpType>
struct Crtp {
 protected:
  T& underlying() { return static_cast<T&>(*this); }
  T const& underlying() const { return static_cast<T const&>(*this); }

 private:
  Crtp() = default;
  friend class CrtpType<T>;
};

template <typename Derived>
class Feature1 : public Crtp<Derived, Feature1> {
 public:
  void update() {
    std::cout << "Feature1:update() for " << Crtp<Derived, Feature1>::underlying().name() << "\n";
  }
};

template <typename Derived>
class Feature2 : public Crtp<Derived, Feature2> {
 public:
  void update() {
    std::cout << "Feature2:update() for " << Crtp<Derived, Feature2>::underlying().name() << "\n";
  }
};

template <template <typename> typename... Features>
class Concrete : public AnotherBase, public Features<Concrete<Features...>>... {
 public:
  Concrete() = default;
  Concrete(Features<Concrete>... features) : Features<Concrete>(features)... {}

  void update() override { (Features<Concrete>::update(), ...); }
  std::string name() const { return "Concrete"; }
};

int main(int argc, char* argv[]) {
  std::cout << "=========================\n";
  {
    Concrete<Feature1> foo;
    foo.update();
  }
  std::cout << "=========================\n";
  {
    Concrete<Feature2> foo;
    foo.update();
  }
  std::cout << "=========================\n";
  {
    Concrete<Feature1, Feature2> foo;
    foo.update();
  }

  return 0;
}
