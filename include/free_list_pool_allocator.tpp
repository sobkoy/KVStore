#pragma once
#include <new>

template <typename T>
T* FreeListPoolAllocator<T>::allocate(std::size_t num_of_blocks_to_alloc) {
  if (num_of_blocks_to_alloc != 1 || free_list_ == nullptr) {
    throw std::bad_alloc();
  }

  Block* free_block = free_list_;
  free_list_ = free_list_->next;
  return reinterpret_cast<T*>(free_block);
}

template <typename T>
void FreeListPoolAllocator<T>::deallocate(T* p, std::size_t num_of_blocks_to_dealloc) {
  auto new_free_block = reinterpret_cast<Block*>(p);
  new_free_block->next = free_list_;
  free_list_ = new_free_block;
}