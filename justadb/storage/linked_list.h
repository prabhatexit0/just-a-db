#pragma once

#include <functional>

#include "justadb/storage/node.h"

namespace Storage {

enum class linked_list_error {
  empty_list,
  element_not_found,
};

template <typename T> class JLinkedList {
public:
  JLinkedList() : head_(nullptr), tail_(nullptr) {};

  explicit JLinkedList(JNodePtr<T> source) : head_(source), tail_(source) {};

  // In future use result types for the return type here.
  void add(JNodePtr<T> node) {
    if (!head_) {
      head_ = node;
      tail_ = node;
    } else {
      tail_->set_next(node);
      tail_ = tail_->next();
    }
  }

  // TODO: Find a way to more strictly type the compare function.
  void remove(auto&& compare) {
    if (!head_) {
      return;
    }

    if (compare(head_->data())) {
      JNodePtr<T> temp = head_->next();
      head_.reset();
      head_ = temp;
    }

    JNodePtr<T> previous = head_;
    while (previous != nullptr) {
      JNodePtr<T> current = previous->next();
      JNodePtr<T> next;
      if (current->next()) {
        next = current->next();
      }

      if (compare(current->data())) {
        current.reset();
        previous->set_next(next);
      }

      previous = next;
    }
  }

  void print() {
    JNodePtr<T> current = head_;
    while (current != nullptr) {
      current->print();
      std::cout << " ";
      current = current->next();
    }
  }
private:
  JNodePtr<T> head_;
  JNodePtr<T> tail_;
};

} // namespace Storage
