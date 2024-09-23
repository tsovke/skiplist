#include <assert.h>
#include <iostream>

#include "../src/skiplist.h"
void testSkiplist() {
  SkipList<int, int> sl;

  // Test insertions
  sl.insert(1, 101);
  sl.insert(2, 102);
  sl.insert(3, 103);

  // Test search
  assert(sl.search(1) != nullptr);
  assert(sl.search(1)->GetValue() == 101);

  // Test removal
  sl.remove(1);
  assert(sl.search(1) == nullptr);

  // Test file operations
  sl.dumpFile();
  sl.loadFile();

  // Test list size
  assert(sl.listSize() == 2);
}

int main() {
  testSkiplist();
  std::cout << "All tests passed!" << std::endl;
  return 0;
}
