#include <cstring>
#include <iostream>
#include <list>
#include <ostream>
#include <string>
#include <vector>

#include <free_list_pool_allocator.h>


int main() {

  /*std::vector<int, FreeListPoolAllocator<int>> vec{12};
  std::cout << *vec.begin() << std::endl;*/

  /*FreeListPoolAllocator<char> allocator;

  char* mystring = allocator.allocate(1);
  std::strcpy(mystring, "Hello");
  char* mystring2 = allocator.allocate(1);
  *mystring2 = 'W';
  std::cout << mystring << "\t" << mystring2 << std::endl;*/

  std::list<std::string, FreeListPoolAllocator<std::string>> lst{"mystr123"};
  std::cout << *lst.begin() << std::endl;

  return 0;
}
