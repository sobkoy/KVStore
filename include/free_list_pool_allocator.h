#pragma once
#include <cstddef>


template <typename T>
class FreeListPoolAllocator {
public:
  using value_type = T;
  FreeListPoolAllocator(std::size_t total_size_ = 1024 * 1024, std::size_t block_size_ = 64)
    : memory_(static_cast<char*>(::operator new(total_size_))),
      total_size_(total_size_),
      block_size_(block_size_),
      free_list_(nullptr) {
    if (block_size_ < 64) {
      block_size_ = 64;
    }
    std::size_t block_count = total_size_ / block_size_; // START HERE!!!
    for (std::size_t i{}; i < block_count - 1; ++i) {
      auto current = reinterpret_cast<Block*>(memory_ + i * block_size_);
      auto next = reinterpret_cast<Block*>(memory_ + (i + 1) * block_size_);
      current->next = next;
    }
    reinterpret_cast<Block*>(memory_ + (block_count - 1) * block_size_)->next = nullptr;
    free_list_ = reinterpret_cast<Block*>(memory_);
  }



  ~FreeListPoolAllocator() {
    ::operator delete(memory_);
  }

  template <typename U>
  FreeListPoolAllocator(FreeListPoolAllocator<U> const& other) {}

  T* allocate(std::size_t num_of_blocks_to_alloc);
  void deallocate(T* p, std::size_t num_of_blocks_to_dealloc);

private:
  char* memory_;
  std::size_t total_size_;
  std::size_t block_size_;

  struct Block {
    Block* next;
  };

  Block* free_list_;

};

#include "free_list_pool_allocator.tpp"