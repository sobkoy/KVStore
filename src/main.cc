#include <iostream>
//#include <list>
//#include <chrono>

#include "free_list_pool_allocator.h"
#include "key_value_store.h"


int main() {
  // const size_t kN = 1'000'000;

  /*try {
    FreeListPoolAllocator<int> allocator(100 * 1024 * 1024);
    std::list<int, FreeListPoolAllocator<int> > list(allocator);
    std::cout << *list.begin() << std::endl;
    list.emplace_back(322);
    list.emplace_back(48);
    list.emplace_back(93);
    list.emplace_back(54);
    std::cout << *list.begin() << std::endl;
  } catch (std::bad_alloc& e) {
    std::cerr << e.what() << std::endl;
  }*/


  // while (true) {
  //   auto start = std::chrono::high_resolution_clock::now();
  //
  //   {
  //     std::list<std::string> list;
  //     //FreeListPoolAllocator<std::string> allocator(100 * 1024 * 1024);
  //     //std::list<std::string, FreeListPoolAllocator<std::string> > list(allocator);
  //     for (std::size_t i{}; i < kN; ++i) {
  //       list.push_back("A");
  //     }
  //
  //     for (std::size_t i{}; i < kN / 2; ++i) {
  //       list.pop_back();
  //     }
  //
  //     for (std::size_t i{}; i < kN / 2; ++i) {
  //       list.push_back("A");
  //     }
  //
  //   }
  //
  //   auto end = std::chrono::high_resolution_clock::now();
  //   double ms = std::chrono::duration<double, std::milli>(end - start).count();
  //   std::cout << ms << " ms" << std::endl;
  // }

  KeyValueStore store;
  store.Put("keyString", "valueString");
  const char* info = store.Get("keyString");

  store.Put("keyString2", "qwerty1234");
  const char* info2 = store.Get("keyString2");

  std::cout << info[0] << std::endl;
  std::cout << info2[3] << std::endl;

  store.Erase("keyString2");
  std::cout << info[0] << std::endl;
  std::cout << info2[3] << std::endl;

  store.Put("keyString3", "qwe");
  const char* info3 = store.Get("keyString3");
  std::cout << info3[1] << std::endl;

  store.Clear();

  std::cout << "adssadsadasdsad" << std::endl;

  return 0;
}
