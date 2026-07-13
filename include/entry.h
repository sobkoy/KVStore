#pragma once
#include <cstring>
#include <stdexcept>
#include <string_view>


struct Entry {
  static constexpr std::size_t kKeySize = 16;
  static constexpr std::size_t kValueSize = 40;

  Entry (std::string_view key, std::string_view value) {
    std::memset(this->key, '\0', sizeof(this->key));
    std::memset(this->value, '\0', sizeof(this->value));
    std::memcpy(this->key, key.data(), key.size());
    std::memcpy(this->value, value.data(), value.size());
  }
  char key[kKeySize];
  char value[kValueSize];
  Entry* next;
};
