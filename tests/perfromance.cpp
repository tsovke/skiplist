#include "../src/skiplist.h"
#include <chrono>
#include <cstdlib>
#include <future>
#include <vector>

SkipList<int, std::string> sl;
void insert_test() {
  std::cout << "task insert\n";
  for (int i = 0; i < 100000; ++i) {
    sl.insert(random() & 0xFFFF, "test");
  }
}
void search_test() {
  for (int i = 0; i < 100000; ++i) {
    sl.search(random() & 0xFFFF);
  }
}

int main() {
  std::vector<std::future<void>> futures;
  auto start = std::chrono::system_clock::now();
  for (int i = 0; i < 16; ++i) {
    std::future<void> future = std::async(std::launch::async, [i] {
      insert_test();
      std::cout << "task " << i << "performed.\n";
    });
    futures.push_back(std::move(future));
  }
  for (auto &future : futures) {
    future.wait();
  }
  auto end = std::chrono::system_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();
  std::cout << "all task completed. spend:" << duration << "milliseconds.\n";
  return 0;
}
