#pragma once

#include "node.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>

#define MAXLEVEL 32
#define PROBABILITY 0.25

namespace SkipListKV {
template <typename K, typename V> class SkipList {
  int m_Level;
  int m_nodeCount;
  Node<K, V> *m_header;

  int randomLevel();

public:
  std::mutex m_mtx;

  Node<K, V> *search(K key) const;
  bool insert(K key, V val);
  bool remove(K key);
  void displayList();
  void dumpFile();
  void loadFile();
  int listSize();

  SkipList();
  ~SkipList();
  void clear(Node<K, V> *);
};

} // namespace SkipListKV

template <typename K, typename V> SkipListKV::SkipList<K, V>::SkipList() {
  this->m_Level = 0;
  this->m_nodeCount = 0;
  this->m_header = new Node<K, V>(K(), V(), MAXLEVEL);
}

template <typename K, typename V> SkipListKV::SkipList<K, V>::~SkipList() {
  clear(m_header);
}

template <typename K, typename V>
void SkipListKV::SkipList<K, V>::clear(Node<K, V> *node) {
  if (node) {
    for (int i = 0; i < MAXLEVEL; ++i) {
      clear(node->forward[i]);
    }
  }
  delete node;
}

template <typename K, typename V>
int SkipListKV::SkipList<K, V>::randomLevel() {
  int lv = 1;
  while ((random() & 0xFFFF) < (PROBABILITY * 0xFFFF))
    ++lv;
  return (lv < MAXLEVEL) ? lv : MAXLEVEL;
}

template <typename K, typename V>
SkipListKV::Node<K, V> *SkipListKV::SkipList<K, V>::search(K key) const {
  std::unique_lock<std::mutex> _(this->m_mtx);
  Node<K, V> *node = this->m_header;
  for (int i = m_Level; i >= 0; --i)
    while (node->forward[i] && node->forward[i]->key < key)
      node = node->forward[i];

  node = node->forward[0];
  if (node && node->key == key)
    return node;
  else
    return nullptr;
}

template <typename K, typename V>
bool SkipListKV::SkipList<K, V>::insert(K key, V val) {
  std::unique_lock<std::mutex> _(this->m_mtx);
  Node<K, V> *node = this->m_header;
  Node<K, V> *update[MAXLEVEL];
  for (int i = m_Level; i >= 0; --i) {
    while (node->forward[i] && node->forward[i]->key < key)
      node = node->forward[i];
    update[i] = node;
  }

  node = node->forward[0];
  if (node && node->key == key) {
    _.unlock();
    return false;
  }

  int nodeLevel = randomLevel();
  if (nodeLevel > m_Level) {
    nodeLevel = ++m_Level;
    update[nodeLevel] = m_header;
  }

  Node<K, V> *newNode = new Node<K, V>(key, val, nodeLevel);
  for (int i = 0; i < nodeLevel; ++i) {
    newNode->forward[i] = update[i]->forward[i];
    update[i]->forward[i] = newNode;
  }
  ++m_nodeCount;

  return true;
}

template <typename K, typename V>
bool SkipListKV::SkipList<K, V>::remove(K key) {
  std::unique_lock<std::mutex> _(this->m_mtx);
  Node<K, V> *node = this->m_header;
  Node<K, V> *update[MAXLEVEL];
  for (int i = m_Level; i >= 0; --i) {
    while (node->forward[i] && node->forward[i]->key < key)
      node = node->forward[i];
    update[i] = node;
  }
  node = node->forward[0];
  if (node && node->key != key) {
    _.unlock();
    return false;
  }

  for (int i = 0; i <= m_Level; ++i) {
    if (update[i]->forward[i] != node)
      break;
    update[i]->forward[i] = node->forward[i];
  }

  delete node;

  while (m_Level > 0 && m_header->forward[m_Level] == nullptr)
    --m_Level;

  --m_nodeCount;
  return true;
}

template <typename K, typename V>
void SkipListKV::SkipList<K, V>::displayList() {
  std::cout << "=========displayList=========" << std::endl;
  Node<K, V> *node = m_header->forward[0];
  while (node)
    std::cout << " (" << node->key << ", " << node->value << ")\n";
}

template <typename K, typename V> void SkipListKV::SkipList<K, V>::dumpFile() {
  std::cout << "======DumpFile======" << std::endl;
  Node<K, V> *node = m_header->forward[0];

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
template <typename K, typename V> void SkipListKV::SkipList<K, V>::loadFile() {
  std::cout << "======LoadFile======" << std::endl;
  std::ifstream file_in("data.bin");
  if (!file_in.is_open()) {
    std::cerr << "Failed to open file for reading." << std::endl;
    return;
  }
  std::string line;
  while (std::getline(file_in, line)) {
    std::string keyStr = line.substr(0, line.find(':'));
    std::string valueStr = line.substr(line.find(':') + 1, line.length());
    if (keyStr.empty() || valueStr.empty())
      continue;
    std::stringstream keyStream;
    keyStream << keyStr;
    std::stringstream valueStream;
    valueStream << valueStr;
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
template <typename K, typename V> int SkipListKV::SkipList<K, V>::listSize() {
  return m_nodeCount;
}
