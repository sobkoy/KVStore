#pragma once
#include <cstring>
#include <string_view>

struct Entry {
  static constexpr std::size_t kKeySize = 16;
  static constexpr std::size_t kValueSize = 40;

  Entry (const std::string_view key, const std::string_view value) : key{0}, value{0}, next{nullptr} {
    std::memcpy(this->key, key.data(), key.size());
    std::memcpy(this->value, value.data(), value.size());
  }

  char key[kKeySize];
  char value[kValueSize];
  Entry* next;
};
