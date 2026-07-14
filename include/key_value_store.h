#pragma once

#include <string_view>

#include "free_list_pool_allocator.h"
#include "hash_map.h"


class KeyValueStore {
public:
  bool Put(std::string_view key, std::string_view value);

  [[nodiscard]] const char* Get(std::string_view key) const;

  bool Erase(std::string_view key);

  void Clear() {
    hash_table_.Clear(
      [this](Entry* entry) { DestroyEntry(entry); }
    );
  }

private:
  Entry* CreateEntry(const std::string_view key, const std::string_view value) {
    Entry* entry = allocator_.allocate(1);
    new(entry) Entry(key, value);
    return entry;
  }

  void DestroyEntry(Entry* entry) {
    allocator_.deallocate(entry, 1);
  }

  HashMap hash_table_;
  FreeListPoolAllocator<Entry> allocator_;
};
