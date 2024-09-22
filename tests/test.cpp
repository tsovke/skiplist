#include <assert.h>
#include <iostream>
#include <string>

#include "../src/skiplist.h"
void testSkiplist() {
  SkipList<int, std::string> sl;

  // Test insertions
  sl.insert(1, "one");
  sl.insert(2, "two");
  sl.insert(3, "three");

  // Test search
  assert(sl.search(1) != nullptr);
  assert(sl.search(1)->GetValue() == "one");

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
