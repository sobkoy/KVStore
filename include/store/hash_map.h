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
    delete [] buckets_;
  }

  [[nodiscard]] Entry* FindEntry(std::string_view key) const;

  void Insert(Entry* entry);

  Entry* Erase(std::string_view key);

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
