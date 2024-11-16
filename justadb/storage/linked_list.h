#pragma once

#include "justadb/storage/disk.h"
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

  auto get(auto &&predicate) -> JNodePtr<T> {
    JNodePtr<T> current = head_;
    while (current != nullptr) {
      if (predicate(current->data())) {
        return current;
      }
      current = current->next();
    }

    return nullptr;
  }

  // TODO: Find a way to more strictly type the compare function.
  void remove(auto &&predicate) {
    if (!head_) {
      return;
    }

    if (predicate(head_->data())) {
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

      if (predicate(current->data())) {
        current.reset();
        previous->set_next(next);
      }

      previous = next;
    }
  }

  auto print() -> void {
    JNodePtr<T> current = head_;
    while (current != nullptr) {
      current->print();
      std::cout << " ";
      current = current->next();
    }
  }

  auto saveToDisk() -> void {
    Disk<T> disk("data.dat");
    JNodePtr current = head_;
    std::streampos offset = 0;
    while (current != nullptr) {
      disk.writeToFile(offset, *current->data());
      offset += sizeof(T);
      current = current->next();
    }
  }

  auto forceClean() -> void {
    std::cout << "FORCE CLEAN - " << std::endl;
    head_.reset();
    tail_.reset();
  }

  auto loadFromDisk() -> void {
    forceClean();
    std::streampos offset = 0;

    Disk<T> disk("data.dat");
    for (int i = 0; i < 10; i++) {
      std::cout << "Offset - " << offset << std::endl;
      std::expected<T, DiskError> dataWithError =
          disk.readFromFile(offset, offset + (std::streampos)sizeof(T));
      if (dataWithError.has_value()) {
        std::cout << "Data - " << *dataWithError << std::endl;
      }
      offset += sizeof(T);
    }
  };

private:
  JNodePtr<T> head_;
  JNodePtr<T> tail_;
};

} // namespace Storage
