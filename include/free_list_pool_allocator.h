#pragma once
#include <cstddef>
#include <memory>
#include <random>


// Pool for Allocator
template<const std::size_t kBlockSize = 64>
class MemoryPool {
public:
  MemoryPool(std::size_t total_size_ = 1024 * 1024)
    : memory_(static_cast<std::byte *>(::operator new(total_size_))),
      free_list_(nullptr),
      total_size_(total_size_) {
    const std::size_t kBlockCount = total_size_ / kBlockSize;
    for (std::size_t i{}; i < kBlockCount - 1; ++i) {
      auto current = reinterpret_cast<Block *>(memory_ + i * kBlockSize);
      auto next = reinterpret_cast<Block *>(memory_ + (i + 1) * kBlockSize);
      current->next = next;
    }
    reinterpret_cast<Block *>(memory_ + (kBlockCount - 1) * kBlockSize)->next = nullptr;
    free_list_ = reinterpret_cast<Block *>(memory_);
  }

  std::byte *allocate_block();

  void deallocate_block(std::byte *p, std::size_t num_of_blocks_to_dealloc);

  ~MemoryPool() {
    ::operator delete(memory_);
  }

private:
  std::byte *memory_;
  std::size_t total_size_;

  struct Block {
    Block *next;
  };

  Block *free_list_;
};


// Actual Allocator class
template<typename T>
class FreeListPoolAllocator {
public:
  using value_type = T;

  FreeListPoolAllocator() : memory_pool_(std::make_shared<MemoryPool<> >()) {
  }

  template<typename U>
  FreeListPoolAllocator(const FreeListPoolAllocator<U> &other)
    : memory_pool_(other.memory_pool_) {
  }

  T *allocate(std::size_t num_of_blocks_to_alloc);

  void deallocate(T *p, std::size_t num_of_blocks_to_dealloc);

private:
  std::shared_ptr<MemoryPool<> > memory_pool_;

  template<typename>
  friend class FreeListPoolAllocator;
};

#include "free_list_pool_allocator.tpp"
