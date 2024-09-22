#pragma once

#include "node.h"
#include <cstddef>
#include <cstdlib>
#include <vector>

template <typename K, typename V> class SkipList {
  Node<K, V> *header; // Header node of the skiplist
  Node<K, V> *footer; // Footer node of the skiplist
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

  SkipList();
  ~SkipList();
};

template <typename K, typename V> int SkipList<K, V>::randomLevel() {
  int level = 1;
  while ((random() & 0xFFFF) < 0x7FFF) // PROBABILITY(0.5) * 0xFFFF
    ++level;
  return (level < MAX_LEVEL) ? level : MAX_LEVEL;
}

template <typename K, typename V>
void SkipList<K, V>::update(Node<K, V> **update, Node<K, V> *node, int level) {}

template <typename K, typename V>
Node<K, V> *SkipList<K, V>::search(const K &key) const {
  Node<K, V> *node = header;
  for (int i = level; i >= 0; --i)
    while (node->getFowardAt(i)->GetKey() < key)
      node = node->getFowardAt(i);

  node = node->getFowardAt(0);
  if (node->GetKey() == key)
    return node;
  else
    return nullptr;
}

template <typename K, typename V>
bool SkipList<K, V>::insert(const K &key, V &val) {
  Node<K, V> *node = header;
  Node<K, V> **update = new Node<K, V> *[MAX_LEVEL];
  for (int i = level; i >= 0; --i) {
    while (node->getFowardAt(i)->GetKey() < key)
      node = node->getFowardAt(i);

    update[i] = node;
  }

  node = node->getForwardAt(0);
  if (node->GetKey() == key)
    return false; // If the key exists, return false.

  int newLevel = randomLevel();
  if (newLevel > level) {
    newLevel = ++level;
    update[newLevel] = header;
  }

  Node<K, V> *newNode = new Node<K, V>(key, val, newLevel);
  // Update the pointers of the predecessor nodes.
  for (int i = 0; i < newLevel; ++i) {
    newNode->setFowardAt(i, update[i]->getFowardAt(i));
    update[i]->setFowardAt(i, newNode);
  }
  ++node_count;

  delete[] update;
}

template <typename K, typename V> bool SkipList<K, V>::remove(const K &key) {
  Node<K, V> *node = header;
  Node<K, V> **update = new Node<K, V> *[MAX_LEVEL];
  for (int i = level; i >= 0; --i) {
    while (node->getFowardAt(i)->GetKey() < key)
      node = node->getFowardAt(i);

    update[i] = node;
  }

  node = node->getForwardAt(0);
  if (node->GetKey() != key)
    return false;

  for (int i = 0; i <= level; ++i) {
    if (update[i]->getFowardAt(i) != node)
      break;

    update[i]->setFowardAt(i, node->getFowardAt(i));
  }

  delete node;
  --node_count;

  while (level > 0 && header->getFowardAt(level) == footer)
    --level;

  return true;
}

template <typename K, typename V> void SkipList<K, V>::displayList() {}

template <typename K, typename V> void SkipList<K, V>::dumpFile() {}

template <typename K, typename V> void SkipList<K, V>::loadFile() {}

template <typename K, typename V> size_t SkipList<K, V>::listSize() {}
