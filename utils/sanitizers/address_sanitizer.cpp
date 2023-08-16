#include <string.h>
#include <iostream>
#include <string>

void useAfterFree() {
  char* s = new char[100];
  delete[] s;
  strcpy(s, "Hello world!");
  printf("string is: %s\n", s);
}

void memoryLeak() {
  char* s = new char[100];
  strcpy(s, "Hello world!");
  printf("string is: %s\n", s);
}

void heapOverflow() {
  char* s = new char[12];
  strcpy(s, "Hello world!");
  printf("string is: %s\n", s);
  delete[] s;
}

void deleteMismatch() {
  char* cstr = new char[100];
  strcpy(cstr, "Hello World");
  std::cout << cstr << std::endl;

  delete cstr;
}

void accessOutOfBounds() {
  int table[4] = {};
  // return true in one of the first 4 iterations or UB due to out-of-bounds access
  for (int i = 0; i <= 4; i++) {
    if (table[i] == 2)
      return;
  }
}

void uninitializedScaler() {
  bool p;  // uninitialized local variable
  if (p)   // UB access to uninitialized scalar
    std::puts("p is true");
  if (!p)  // UB access to uninitialized scalar
    std::puts("p is false");
}

void nullpointerDereference() {
  int* p = nullptr;
  printf("p value: %d\n", *p);
}

int main(int argc, const char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s [1-4]\n", argv[0]);
    return 1;
  }

  int index = std::stoi(argv[1]);

  switch (index) {
    case 1:
      useAfterFree();
      return 0;
    case 2:
      memoryLeak();
      return 0;
    case 3:
      heapOverflow();
      return 0;
    case 4:
      deleteMismatch();
      return 0;
    case 5:
      accessOutOfBounds();
      return 0;
    case 6:
      uninitializedScaler();
      return 0;
    case 7:
      nullpointerDereference();
      return 0;
  }

  printf("Invalid index. \n");
  printf("Usage: %s [1-7]\n", argv[0]);
  return 1;
}
