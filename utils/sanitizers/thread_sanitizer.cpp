#include <future>

int main(int argc, char* argv[]) {
  bool flag = false;

  auto fut = std::async(std::launch::async, [&] { flag = true; });

  flag = false;

  fut.wait();
  return 0;
}
