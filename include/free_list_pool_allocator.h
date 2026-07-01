#pragma once
#include <cstddef>


template <typename T>
class FreeListPoolAllocator {
public:
  using value_type = T;
  FreeListPoolAllocator(sts::size_t block_size_ = 64, std::size_t total_size_ = 1024 * 1024)
    : memory_(static_cast<char*>(::operator new(total_size_))),
      block_size_(block_size_),
      total_size_(total_size_),
      free_list(nullptr) {
    std::size_t block_count = total_size_ / block_size_; // START HERE!!!

  }

  ~FreeListPoolAllocator() {
    ::operator delete(memory_);
  }

  T* allocate();
  void deallocate(T* p);

private:
  char* memory_;
  std::size_t block_size_;
  std::size_t total_size_;

  struct Block {
    Block* next;
  };

  Block* free_list;

};
