#include <array>
#include <cstddef>
#include <memory_resource>
#include <vector>

/**
 * This example demonstrates how to use a monotonic buffer resource to manage
 * memory for a vector. The monotonic buffer resource is a memory resource that
 * allocates memory from a fixed-size buffer. When the buffer is full, the
 * monotonic buffer resource throws an exception.
 */
int main(int argc, char* argv[]) {
  std::array<std::byte, 1000> storage;
  std::pmr::monotonic_buffer_resource resource(storage.data(), storage.size(),
                                               std::pmr::null_memory_resource());
  std::pmr::vector<int> v({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, &resource);
  printf("After construction - size: %lu, capacity: %lu\n", v.size(), v.capacity());
  // Try to exceed buffer size
  for (size_t i = 0; i < 1000; ++i) {
    v.push_back(i);
    printf("In for loop - i: %lu, size: %lu, capacity: %lu\n", i, v.size(), v.capacity());
  }

  return 0;
}

/**
 * Assuming a conventional doubling-growth policy for std::vector and that an int is 4 bytes, here’s
 * one way to reason it out:
 *
 * 1. Initially, the vector is constructed with 11 ints (11 * 4 = 44 bytes).
 *
 * 2. When the first push_back (i = 0) is performed, the vector’s size equals its capacity (11),
 *    and it reallocates. Typically, the new capacity doubles to 22, requiring 22 * 4 = 88 bytes.
 *    Since the monotonic_buffer_resource hands out memory sequentially, the old block's memory is
 * not reclaimed.
 *
 * 3. Total allocated so far: 44 bytes (initial) + 88 bytes = 132 bytes.
 *
 * 4. At push_back when i reaches 11 (adding 11 more elements to reach a total size of 22),
 *    a reallocation occurs again, doubling capacity to 44 (44 * 4 = 176 bytes).
 *    Cumulative allocation becomes: 132 + 176 = 308 bytes.
 *
 * 5. Another reallocation happens when the vector’s size reaches 44, which occurs at push_back when
 * i reaches 33. The capacity doubles to 88, requiring 88 * 4 = 352 bytes. Running total becomes:
 * 308 + 352 = 660 bytes.
 *
 * 6. Finally, when the vector size reaches 88, the next push_back triggers another reallocation.
 *    This happens when i equals 77 (because we started with 11, then added 66 more until size
 * becomes 77, and then push_back makes the size 88). Doubling the capacity from 88 gives a new
 * capacity of 176, which requires 176 * 4 = 704 bytes.
 *
 * 7. Before this allocation, the cumulative memory allocated is 660 bytes. Adding the new 704 bytes
 * gives: 660 + 704 = 1364 bytes, which exceeds the fixed buffer size of 1000 bytes.
 *
 * Since the underlying monotonic_buffer_resource is configured with std::pmr::null_memory_resource
 * as the upstream (which throws on allocation failure), the allocation request fails, and an
 * exception (typically std::bad_alloc) is thrown.
 *
 * Thus, the program throws when i equals 77.
 */
