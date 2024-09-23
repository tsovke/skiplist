#include "node.h"

template <typename K, typename V>
SkipListKV::Node<K, V>::Node(const K k, const V v, int level)
    : key(k), value(v), level(level), forward(level + 1, nullptr) {}

template <typename K, typename V> SkipListKV::Node<K, V>::~Node() {
  for (auto *ptr : forward)
    if (ptr != nullptr)
      delete ptr;
}
