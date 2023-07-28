#include <ctime>
#include <functional>
#include <iostream>
#include <iterator>
#include <random>
#include <unordered_set>
#include <vector>

template <class T>
std::ostream& operator<<(std::ostream& o, const std::vector<T>& arr) {
  copy(arr.cbegin(), arr.cend(), std::ostream_iterator<T>(o, ""));
  return o;
}

std::vector<int> reservoir_sampling(int n, int k) {
  std::vector<int> reservoir(k);

  // Initialize the reservoir with the first k elements
  for (int i = 0; i < k; i++) {
    reservoir[i] = (i);
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  auto distrib = std::bind(std::uniform_int_distribution<int>(0, RAND_MAX), gen);

  // Iterate through the rest of the elements
  for (int i = k; i < n; i++) {
    int j = distrib() % (i + 1);
    if (auto found_iter = std::find(reservoir.begin(), reservoir.end(), j);
        found_iter != reservoir.end()) {
      *found_iter = i;
    }
  }

  return reservoir;
}

int main(int argc, char* argv[]) {
  std::cout << "reservoir_sampling(5, 5): " << reservoir_sampling(5, 5) << "\n";
  std::cout << "reservoir_sampling(6, 5): " << reservoir_sampling(6, 5) << "\n";
  std::cout << "reservoir_sampling(10, 5): " << reservoir_sampling(10, 5) << "\n";
  return 0;
}
