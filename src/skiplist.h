#pragma once

#include "node.h"

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

template <typename K, typename V> class SkipList {
  int level; // Current level of the skiplist
  size_t node_count;
  Node<K, V> *header; // Header node of the skiplist
  // Node<K, V> *footer; // Footer node of the skiplist

  static const int MAX_LEVEL = 16;
  // static constexpr double PROBABILITY =
  //     0.5; // Probability of increasing level when inserting a new node

  // Helper function: Randomly determine the level of a new node
  int randomLevel();

  // // Helper function: Update predecessor nodes
  // void update(Node<K, V> **update, Node<K, V> *node, int level);

public:
  Node<K, V> *search(const K &key) const;
  bool insert(const K &key, const V &val);
  bool remove(const K &key);
  void displayList();
  void dumpFile();
  void loadFile();
  size_t listSize();

  SkipList() : level(), node_count() { header = new Node<K, V>(); }
  ~SkipList() {
    Node<K, V> *node = header;
    Node<K, V> *tmp;
    while (node) {
      tmp = node;
      node = node->forward[0];
      delete tmp;
    }
  }
};

template <typename K, typename V> int SkipList<K, V>::randomLevel() {
  int level = 1;
  while ((random() & 0xFFFF) < 0x7FFF) // PROBABILITY(0.5) * 0xFFFF
    ++level;
  return (level < MAX_LEVEL) ? level : MAX_LEVEL;
}

// template <typename K, typename V>
// void SkipList<K, V>::update(Node<K, V> **update, Node<K, V> *node, int level)
// {}

template <typename K, typename V>
Node<K, V> *SkipList<K, V>::search(const K &key) const {
  Node<K, V> *node = header;
  for (int i = level; i >= 0; --i)
    while (node->forward[i] && node->forward[i]->GetKey() < key)
      node = node->forward[i];

  node = node->forward[0];
  if (node && node->GetKey() == key)
    return node;
  else
    return nullptr;
}

template <typename K, typename V>
bool SkipList<K, V>::insert(const K &key, const V &val) {
  Node<K, V> *node = header;
  Node<K, V> **update = new Node<K, V> *[MAX_LEVEL];
  for (int i = level; i >= 0; --i) {
    while (node->forward[i] && node->forward[i]->GetKey() < key)
      node = node->forward[i];

    update[i] = node;
  }

  node = node->forward[0];
  if (node && node->GetKey() == key)
    return false; // If the key exists, return false.

  int newLevel = randomLevel();
  if (newLevel > level) {
    newLevel = ++level;
    update[newLevel] = header;
  }

  Node<K, V> *newNode = new Node<K, V>(key, val, newLevel);
  // Update the pointers of the predecessor nodes.
  for (int i = 0; i < newLevel; ++i) {
    newNode->forward[i] = update[i]->forward[i];
    update[i]->forward[i] = newNode;
  }
  ++node_count;

  delete[] update;
  return true;
}

template <typename K, typename V> bool SkipList<K, V>::remove(const K &key) {
  Node<K, V> *node = header;
  Node<K, V> **update = new Node<K, V> *[MAX_LEVEL];
  for (int i = level; i >= 0; --i) {
    while (node->forward[i] && node->forward[i]->GetKey() < key)
      node = node->forward[i];

    update[i] = node;
  }

  node = node->forward[0];
  if (node && node->GetKey() != key)
    return false;

  for (int i = 0; i <= level; ++i) {
    if (update[i]->forward[i] != node)
      break;

    update[i]->forward[i] = node->forward[i];
  }

  delete node;
  --node_count;

  while (level > 0 && header->forward[level] == nullptr) // footer
    --level;

  return true;
}

template <typename K, typename V> void SkipList<K, V>::displayList() {
  Node<K, V> *node;
  for (int i = level; i >= 0; --i) {
    node = header->forward[i];
    std::cout << "Level " << i << " : ";
    int tmp = 0;
    while (node) {
      std::cout << "    (" << node->GetKey() << ", " << node->GetValue() << ")";
      if (++tmp == 10) {
        std::cout << "\n";
        tmp = 0;
      }
      node = node->forward[i];
    }
    std::cout << "\n";
  }
}

template <typename K, typename V> void SkipList<K, V>::dumpFile() {
  std::cout << "======DumpFile======" << std::endl;
  Node<K, V> *node = header->forward[0];

  std::ofstream file("data.bin");
  if (!file.is_open()) {
    std::cerr << "Failed to open file for writing." << std::endl;
    return;
  }
  while (node) {
    file << node->GetKey() << ':' << node->GetValue() << "\n";
    node = node->forward[0];
  }
  file.flush();
  file.close();
  std::cout << "Data has been written to data.bin." << std::endl;
}

template <typename K, typename V> void SkipList<K, V>::loadFile() {
  std::cout << "======LoadFile======" << std::endl;
  std::ifstream file_in("data.bin");
  std::string line;
  while (std::getline(file_in, line)) {
    std::string keyStr = line.substr(0, line.find(':'));
    std::string valueStr = line.substr(line.find(':') + 1, line.length());
    if (keyStr.empty() || valueStr.empty())
      continue;
    std::istringstream keyStream(keyStr);
    std::istringstream valueStream(valueStr);
    std::cout << "Load (" << keyStr << "," << valueStr << ")\n";
    K key;
    V value;

    if (!(keyStream >> key) || !(valueStream >> value)) {
      throw std::invalid_argument(
          "Failed to convert string to key-value pair.");
    }
    (void)insert(key, value);
  }
  file_in.close();
}

template <typename K, typename V> size_t SkipList<K, V>::listSize() {
  return node_count;
}
