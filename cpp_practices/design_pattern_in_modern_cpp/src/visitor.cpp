#include <iostream>
#include <string>
using namespace std;

// 1. Add an accept(Visitor) method to the "element" hierarchy
class Element {
 public:
  virtual void accept(class Visitor& v) = 0;
  virtual string info() const = 0;
};

class Fire : public Element {
 public:
  void accept(Visitor& v) override;
  string info() const override { return "Fire"; }
};

class Ground : public Element {
 public:
  void accept(Visitor& v) override;
  string info() const override { return "Ground"; }
};

class Wind : public Element {
 public:
  void accept(Visitor& v) override;
  string info() const override { return "Wind"; }
};

// 2. Create a "visitor" base class w/ a visit() method for every "element" type
class Visitor {
 public:
  virtual void visit(Element* e) = 0;
};

void Fire::accept(Visitor& v) {
  v.visit(this);
}

void Ground::accept(Visitor& v) {
  v.visit(this);
}

void Wind::accept(Visitor& v) {
  v.visit(this);
}

// 3. Create a "visitor" derived class for each "operation" to do on "elements"
class UpVisitor : public Visitor {
  /*virtual*/ void visit(Element* e) { cout << "do Up on " + e->info() << '\n'; }
};

class DownVisitor : public Visitor {
  /*virtual*/ void visit(Element* e) { cout << "do Down on " + e->info() << '\n'; }
};

int main() {
  Element* list[] = {new Fire(), new Ground(), new Wind()};
  UpVisitor up;      // 4. Client creates
  DownVisitor down;  //    "visitor" objects
  for (size_t i = 0; i < 3; i++) {
    //    and passes each
    list[i]->accept(up);
  }
  //    to accept() calls
  for (size_t i = 0; i < 3; i++) {
    list[i]->accept(down);
  }
}
