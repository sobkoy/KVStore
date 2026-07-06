#include <iostream>


// Pool part
template <const std::size_t kBlockSize>
std::byte* MemoryPool<kBlockSize>::allocate_block() {
  if (free_list_ == nullptr) {
    throw std::bad_alloc();
  }
  std::cout << "Pool Allocation " << std::endl;
  Block* free_block = free_list_;
  free_list_ = free_list_->next;
  return reinterpret_cast<std::byte*>(free_block);
}

template <const std::size_t kBlockSize>
void MemoryPool<kBlockSize>::deallocate_block(std::byte* p, const std::size_t num_of_blocks_to_dealloc) {
  std::cout << "Pool Deallocation " << std::endl;
  auto new_free_block = reinterpret_cast<Block*>(p);
  new_free_block->next = free_list_;
  free_list_ = new_free_block;
}

// Allocator part
template <typename T>
T* FreeListPoolAllocator<T>::allocate(std::size_t num_of_blocks_to_alloc) {
  if (num_of_blocks_to_alloc != 1) {
    throw std::bad_alloc();
  }
  std::cout << "Alloc Allocation " << std::endl;
  return reinterpret_cast<T*>(memory_pool_->allocate_block());
}

template <typename T>
void FreeListPoolAllocator<T>::deallocate(T* p, std::size_t num_of_blocks_to_dealloc) {
  std::cout << "Alloc Deallocation " << std::endl;
  memory_pool_->deallocate_block(reinterpret_cast<std::byte*>(p), num_of_blocks_to_dealloc);
}