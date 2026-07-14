#include "hash_map.h"

Entry* HashMap::FindEntry(const std::string_view key) const {
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

void HashMap::Insert(Entry* entry) {
  std::size_t index = HashFunction(entry->key);
  if (buckets_[index] == nullptr) {
    buckets_[index] = entry;
  } else {
    entry->next = buckets_[index];
    buckets_[index] = entry;
  }
}

Entry* HashMap::Erase(const std::string_view key) {
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