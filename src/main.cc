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






  /*
  FreeListPoolAllocator<char> allocator;

  char* mystring = allocator.allocate(1);
  std::strcpy(mystring, "Hello");
  char* mystring2 = allocator.allocate(1);
  *mystring2 = 'W';
  std::cout << mystring << "\t" << mystring2 << std::endl;


  allocator.deallocate(mystring2, 1);


  char* mystring3 = allocator.allocate(1);
  std::strcpy(mystring3, "Hello3");
  char* mystring4 = allocator.allocate(1);
  *mystring4 = 'L';
  std::cout << mystring3 << "\t" << mystring4 << std::endl;

  std::strcpy(mystring, "qwerty1234567qwerty");
  std::cout << mystring << std::endl;
  */




  std::list<std::string, FreeListPoolAllocator<std::string>> lst{"mystr123"};
  std::cout << *lst.begin() << std::endl;

  return 0;
}
