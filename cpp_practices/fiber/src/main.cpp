#include <boost/fiber/fiber.hpp>
#include <boost/fiber/operations.hpp>

#include <iostream>

/**
 * Post: https://dilawar.github.io/posts/2021/2021-11-14-example-boost-fiber/
 * Video: https://www.youtube.com/watch?v=e-NUmyBou8Q
 */

void print_a() {
  std::cout << "a";
  boost::this_fiber::yield();
}

void print_b() {
  std::cout << "b";
  boost::this_fiber::yield();
}

int main(int argc, char* argv[]) {
  int i = 0;
  boost::fibers::fiber([&]() {
    do {
      print_a();
      i++;
    } while (i < 20);
  }).detach();

  boost::fibers::fiber([&]() {
    do {
      i++;
      print_b();
    } while (i < 20);
  }).detach();

  printf("X");
  return 0;
}
