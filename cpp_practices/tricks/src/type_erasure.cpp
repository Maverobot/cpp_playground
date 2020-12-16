#include <iostream>

template <typename T>
class smartptr {
 private:
  struct deleter_base {
    virtual ~deleter_base() = default;
    virtual void apply(void*) = 0;
  };

  template <typename Deleter>
  struct deleter : public deleter_base {
    deleter(Deleter d) : d_(d) {}
    void apply(void* p) override { d_(static_cast<T*>(p)); }
    Deleter d_;
  };

  deleter_base* d_;
  T* p_;

 public:
  template <typename Deleter>
  smartptr(T* p, Deleter d) : p_(p), d_(new deleter<Deleter>(d)) {}
  ~smartptr() {
    d_->apply(p_);
    delete d_;
  }

  T* operator->() { return p_; }
  const T operator->() const { return p_; }
};

int main(int argc, char* argv[]) {
  smartptr<double> p(new double(), [](double* p) { std::cout << "deleter called\n"; });
  std::cout << "start\n";
  return 0;
}
