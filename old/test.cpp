#include "skiplist.h"
#include <string>

int main() {
  SkipListKV::SkipList<int, std::string> kv;
  kv.insert(1, "aaaa");
  kv.insert(4, "bbbb");
  kv.insert(2, "23435");
  return 0;
}
