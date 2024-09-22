#pragma once

#include "node.h"

#include "../data/data.pb.h"

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
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
  bool insert(const K &key, V &val);
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
      node = node->getForwardAt(0);
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
    while (node->getForwardAt(i) && node->getForwardAt(i)->GetKey() < key)
      node = node->getForwardAt(i);

  node = node->getForwardAt(0);
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
    while (node->getForwardAt(i) && node->getForwardAt(i)->GetKey() < key)
      node = node->getForwardAt(i);

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
    newNode->setForwardAt(i, update[i]->getForwardAt(i));
    update[i]->setForwardAt(i, newNode);
  }
  ++node_count;

  delete[] update;
}

template <typename K, typename V> bool SkipList<K, V>::remove(const K &key) {
  Node<K, V> *node = header;
  Node<K, V> **update = new Node<K, V> *[MAX_LEVEL];
  for (int i = level; i >= 0; --i) {
    while (node->getForwardAt(i) && node->getForwardAt(i)->GetKey() < key)
      node = node->getForwardAt(i);

    update[i] = node;
  }

  node = node->getForwardAt(0);
  if (node->GetKey() != key)
    return false;

  for (int i = 0; i <= level; ++i) {
    if (update[i]->getForwardAt(i) != node)
      break;

    update[i]->setForwardAt(i, node->getForwardAt(i));
  }

  delete node;
  --node_count;

  while (level > 0 && header->getForwardAt(level) == nullptr) // footer
    --level;

  return true;
}

template <typename K, typename V> void SkipList<K, V>::displayList() {
  Node<K, V> *node;
  for (int i = level; i >= 0; --i) {
    node = header->getForwardAt(i);
    std::cout << "Level " << i << " : ";
    int tmp = 0;
    while (node) {
      std::cout << "    (" << node->GetKey() << ", " << node->GetValue() << ")";
      if (++tmp == 10) {
        std::cout << "\n";
        tmp = 0;
      }
      node = node->getForwardAt(i);
    }
    std::cout << "\n";
  }
}

template <typename K, typename V> void SkipList<K, V>::dumpFile() {
  std::cout << "======DumpFile======" << std::endl;
  data::KeyValues kv_data;
  Node<K, V> *node = header->getForwardAt(0);
  while (node) {
    data::KeyValuePair *kv = kv_data.add_pairs();
    kv->set_key(std::to_string(node->GetKey()));
    kv->set_value(std::to_string(node->GetValue()));
  }

  std::string serialized;
  kv_data.SerializeToString(&serialized);

  std::ofstream file("data.bin", std::ios::out | std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "Failed to open file for writing." << std::endl;
    return;
  }
  file.write(serialized.data(), serialized.size());
  file.close();
  std::cout << "Data has been written to data.bin." << std::endl;
}

template <typename K, typename V> void SkipList<K, V>::loadFile() {
  std::cout << "======LoadFile======" << std::endl;

  data::KeyValues kv_data;

  std::ifstream file_in("data.bin", std::ios::in | std::ios::binary);
  if (!file_in.is_open()) {
    std::cerr << "Failed to open file for reading." << std::endl;
    return;
  }

  std::string serialized(std::istreambuf_iterator<char>(file_in), {});

  file_in.close();

  if (!kv_data.ParseFromString(serialized)) {
    std::cerr << "Failed to parse serialized data." << std::endl;
    return;
  }

  for (const auto &kv : kv_data.pairs()) {
    std::cout << "Load (" << kv.key() << ", " << kv.value() << ")\n";
    insert(static_cast<K>(kv.key()), static_cast<V>(kv.value()));
  }
  file_in.close();
}

template <typename K, typename V> size_t SkipList<K, V>::listSize() {
  return node_count;
}
