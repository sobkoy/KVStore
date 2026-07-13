#pragma once

#include <functional>
#include <string_view>

#include "entry.h"

class HashMap {
public:
  HashMap(const std::size_t size = 1000)
    : buckets_count_(size),
      buckets_(new Entry*[buckets_count_]{}) {}

  HashMap(const HashMap&) = delete;

  HashMap& operator=(const HashMap&) = delete;

  ~HashMap() {
    // remember to call from kv-store
    delete [] buckets_;
  }

  Entry* FindEntry(const std::string_view key) {
    const std::size_t index = HashFunction(key);
    Entry* current = buckets_[index];
    while (current != nullptr) {
      if (std::string_view(current->key) == key) {
        return current;
      }
      current = current->next;
    }
    return nullptr;
  }

  Entry* Erase(const std::string_view key) {
    // returns where we need to dealloc mem
    const std::size_t index = HashFunction(key);
    // no element
    if (buckets_[index] == nullptr) {
      return nullptr;
    }
    // 1 element
    Entry* current = buckets_[index];
    if (current->key == key) {
      Entry* temp = current;
      buckets_[index] = current->next;
      return temp;
    }


    // more than 1 element
    Entry* previous = current;
    current = current->next;
    while (current != nullptr) {
      if (current->key == key) {
        Entry* temp = current;
        previous->next = current->next;
        return temp;
      }
      previous = current;
      current = current->next;
    }

    return nullptr;
  }

  //
  //
  // Next step is to implement Clear (Foreach), so start with KVStore itself
  //
  //

  void Insert(Entry* entry) {
    std::size_t index = HashFunction(entry->key);
    if (buckets_[index] == nullptr) {
      buckets_[index] = entry;
    } else {
      entry->next = buckets_[index];
      buckets_[index] = entry;
    }
  }

  template<typename Func>
  void Clear(Func&& Destroy) {
    for (std::size_t index{}; index < buckets_count_; ++index) {
      Entry* current = buckets_[index];
      while (current != nullptr) {
        Entry* next = current->next;
        Destroy(current);
        current = next;
      }
      buckets_[index] = nullptr;
    }
  }

private:
  std::size_t buckets_count_;
  Entry** buckets_;

  [[nodiscard]] std::size_t HashFunction(const std::string_view key) const {
    return std::hash<std::string_view>{}(key) % buckets_count_;
  }
};
