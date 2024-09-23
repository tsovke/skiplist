#pragma once

#include <vector>
template <typename K, typename V> class Node {
  K key;
  V value;
  int node_level;

public:
  std::vector<Node<K, V> *>
      forward; // Vector pointing to nodes at the next level

  // Constructor
  Node() : key(), value(), node_level(), forward() {}
  // Constructor
  Node(const K &_key, const V &_val, int _level)
      : key(_key), value(_val), node_level(_level),
        forward(std::vector<Node<K, V> *>(node_level, nullptr)) {}

  // Destructor
  ~Node() {
    // Explicity delete all forward pointers to avoid memory leaks
    for (auto &prt : forward)
      if (prt) {
        delete prt;
      }
  }

  // Get the key
  const K &GetKey() const { return key; }
  K &GetKey() { return key; }

  // Get the value
  const V &GetValue() const { return value; }
  const V &GetValue() { return value; }

  // Set the value
  void SetValue(const V &_val) { value = _val; }

  // // Get the vector of forward pointers
  // const std::vector<Node<K, V> *> &getForward() const { return forward; }
  // std::vector<Node<K, V> *> &getForward() { return forward; }

  // // Get the forward pointer at a specific index
  // Node<K, V> *getForwardAt(int _index) const { return forward[_index]; }
  // Node<K, V> *getForwardAt(int _index) { return forward[_index]; }

  // // Set the forward pointer at a specific index
  // void setForwardAt(int _index, Node<K, V> *_node) { forward[_index] = _node;
  // }
};
