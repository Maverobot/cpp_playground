int main() {
  // OK
  int* p = new int;
  delete p;

  // Memory leak
  int* q = new int;
  // no delete
}

// valgrind --leak-check=full --show-leak-kinds=all ./memory_leak
