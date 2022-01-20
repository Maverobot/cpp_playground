#include <ncurses.h>
#include <stdio.h>
#include <future>
#include <iostream>
#include <thread>

int main() {
  // Enable standard literals as 2s and ""s.
  using namespace std::literals;

  auto getch_f = [] {
    initscr();
    noecho();
    int c = getch();
    return c;
  };

  bool unpressed = true;

  while (1) {
    auto async_getch = std::async(std::launch::async, getch_f);
    if (async_getch.wait_for(50ms) != std::future_status::ready) {
      if (unpressed) {
        unpressed = false;
        printf("unpressed\n");
      }
      continue;
    }
    auto c = async_getch.get();
    switch (c) {
      case 'w':
        if (!unpressed) {
          unpressed = true;
          printf("up\n");
        }
        break;
      case 's':
        if (!unpressed) {
          unpressed = true;
          printf("down\n");
        }
        break;
      case 'a':
        if (!unpressed) {
          unpressed = true;
          printf("left\n");
        }
        break;
      case 'd':
        if (!unpressed) {
          unpressed = true;
          printf("right\n");
        }
        break;
      default:
        mvprintw(24, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c' ", c, c);
        break;
    }
  }
  return 0;
}
