#pragma once

#include <vector>
template <typename K, typename V> class Node {
  K key;
  V value;
  std::vector<Node<K, V> *>
      forward; // Vector pointing to nodes at the next level
public:
  // Constructor
  Node(const K &_key, const V &_val, int level)
      : key(_key), value(_val),
        forward(std::vector<Node<K, V> *>(level, nullptr)) {}

  // Get the key
  const K &GetKey() const { return key; }
  K &GetKey() { return key; }

  // Get the value
  const K &GetValue() const { return value; }
  K &GetValue() { return value; }

  // Set the value
  void SetValue(const V &_val) { value = _val; }

  // Get the vector of forward pointers
  const std::vector<Node<K, V> *> &getForward() const { return forward; }
  std::vector<Node<K, V> *> &getForward() { return forward; }

  // Get the forward pointer at a specific index
  Node<K, V> *getFowardAt(int _index) const { return forward[_index]; }
  Node<K, V> *getFowardAt(int _index) { return forward[_index]; }

  // Set the forward pointer at a specific index
  void setFowardAt(int _index, Node<K, V> *_node) { forward[_index] = _node; }
};
