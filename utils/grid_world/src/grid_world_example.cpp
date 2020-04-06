#include <grid_world.h>

int main(int argc, char* argv[]) {
  GridWorld world;
  world.display();
  std::cout << world.step(GridWorld::Action::right) << "\n";
  world.display();
  std::cout << world.step(GridWorld::Action::down) << "\n";
  world.display();
  std::cout << world.step(GridWorld::Action::right) << "\n";
  world.display();
  std::cout << world.step(GridWorld::Action::down) << "\n";
  world.display();
  std::cout << world.step(GridWorld::Action::down) << "\n";
  world.display();
  std::cout << world.step(GridWorld::Action::right) << "\n";
  world.display();
  std::cout << world.step(GridWorld::Action::right) << "\n";
  world.display();
  return 0;
}
