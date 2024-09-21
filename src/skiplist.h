#pragma once

#include "node.h"

template <typename K, typename V> class SkipList {
  Node<K, V> *header; // Header node of the skiplist
  int level;          // Current level of the skiplist
  static constexpr double PROBABILITY =
      0.5; // Probability of increasing level when inserting a new node

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
  int size();
};
