#pragma once

#include <free_list_pool_allocator.h>
#include <hash_map.h>

class KeyValueStore {
public:

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

  const char* Get(const std::string_view key) {
    const Entry* entry = hash_table_.FindEntry(key);
    if (entry != nullptr) {
      return entry->value;
    }
    return nullptr;
  }

  bool Erase(std::string_view key) {
    Entry* entry_to_delete = hash_table_.Erase(key);
    if (entry_to_delete != nullptr) {
      DestryEntry(entry_to_delete);
      return true;
    }
    return false;
  }

  void Clear() {
    hash_table_.Clear(
      [this](Entry* entry) { DestryEntry(entry); }
    );
  }

private:

  Entry* CreateEntry(const std::string_view key, const std::string_view value) {
    Entry* entry = allocator_.allocate(1);
    new(entry) Entry(key, value);
    return entry;
  }

  void DestryEntry(Entry* entry) {
    allocator_.deallocate(entry, 1);
  }

  HashMap hash_table_;
  FreeListPoolAllocator<Entry> allocator_;
};
