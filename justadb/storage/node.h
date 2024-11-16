#pragma once

#include <iostream>
#include <memory>

namespace Storage {

using JNodeID = unsigned long long;

template <typename T> class JNode;

template <typename T> class JNode {
public:
  explicit JNode(T value)
      : id_(next_id_++), data_(std::make_shared<T>(value)), next_(nullptr) {}

  auto data() -> std::shared_ptr<T> {
    return data_;
  }

  auto next() -> std::shared_ptr<JNode<T>> {
    return next_;
  }

  void set_next(std::shared_ptr<JNode<T>> node) {
    next_ = node;
  }

  void print() {
    if (data_ != nullptr)
      std::cout << *data_;
  }

private:
  static JNodeID next_id_;
  const unsigned long long id_;
  std::shared_ptr<T> data_;
  std::shared_ptr<JNode<T>> next_;
};

template <typename T> JNodeID JNode<T>::next_id_ = 0;

template <typename T> using JNodePtr = std::shared_ptr<JNode<T>>;
template <typename T> using DataPtr = std::shared_ptr<T>;

} // namespace Storage
