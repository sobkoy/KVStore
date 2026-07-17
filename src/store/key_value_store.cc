#include "key_value_store.h"

bool KeyValueStore::Put(const std::string_view key, const std::string_view value) {
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

const char* KeyValueStore::Get(const std::string_view key) const {
  const Entry* entry = hash_table_.FindEntry(key);
  if (entry != nullptr) {
    return entry->value;
  }
  return nullptr;
}

bool KeyValueStore::Erase(const std::string_view key) {
  Entry* entry_to_delete = hash_table_.Erase(key);
  if (entry_to_delete != nullptr) {
    DestroyEntry(entry_to_delete);
    return true;
  }
  return false;
}