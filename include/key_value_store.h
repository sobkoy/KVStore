#pragma once

#include <free_list_pool_allocator.h>
#include <hash_map.h>

class KeyValueStore {
public:
  Entry* CreateEntry(const std::string_view key, const std::string_view value) {
    Entry* entry = allocator_.allocate(1);
    new(entry) Entry(key, value);
    return entry;
  }



  bool Put(const std::string_view key, const std::string_view value) {
    if (key.size() >= Entry::kKeySize || value.size() >= Entry::kValueSize) {
      throw std::invalid_argument("key or value is too big");
    }

    Entry* entry = hash_table_.FindEntry(key);
    if (entry != nullptr) {
      std::memcpy(entry->value, value.data(), value.size());
      return false;
    }

    hash_table_.Insert(CreateEntry(key, value));
    return true;
  }





private:
  HashMap hash_table_;
  FreeListPoolAllocator<Entry> allocator_;
};
