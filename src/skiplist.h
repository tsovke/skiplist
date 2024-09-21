#pragma once

#include "node.h"
#include <cstddef>
#include <cstdlib>

template <typename K, typename V> class SkipList {
  Node<K, V> *header; // Header node of the skiplist
  int level;          // Current level of the skiplist
  size_t node_count;

  static const int MAX_LEVEL = 16;
  // static constexpr double PROBABILITY =
  //     0.5; // Probability of increasing level when inserting a new node

  // Helper function: Randomly determine the level of a new node
  int randomLevel();

  // Helper function: Update predecessor nodes
  void update(Node<K, V> **update, Node<K, V> *node, int level);

public:
  Node<K, V> *search(const K &key) const;
  bool insert(const K &key, V &val);
  bool remove(const K &key);
  void displayList();
  void dumpFile();
  void loadFile();
  size_t listSize();
};

template <typename K, typename V> int SkipList<K, V>::randomLevel() {
  int level = 1;
  while ((random() & 0xFFFF) < 0x7FFF) // PROBABILITY(0.5) * 0xFFFF
    ++level;
  return (level < MAX_LEVEL) ? level : MAX_LEVEL;
}

template <typename K, typename V>
void SkipList<K, V>::SkipList<K, V>::SkipList<K, V>::update(Node<K, V> **update,
                                                            Node<K, V> *node,
                                                            int level){
  
}
