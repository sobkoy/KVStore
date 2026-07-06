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



  std::list<std::string, FreeListPoolAllocator<std::string>> lst2 = lst;
  std::cout << *lst2.begin() << std::endl;


  std::list<std::string, FreeListPoolAllocator<std::string>> lst3 = {"my_3_text"};
  std::cout << *lst3.begin() << std::endl;

  lst.erase(lst.begin());

  //lst.erase(lst3.begin());


  lst.insert(lst.begin(), lst3.begin(), lst3.end());

  lst.push_back("last_string_to_lst1");

  std::list<std::string, FreeListPoolAllocator<std::string>> lst4;

  lst4 = std::move(lst);




  std::cout << "---" << std::endl;
  //std::cout << *lst.begin() << std::endl;
  //std::cout << &*lst.begin() << std::endl;
  //std::cout << *std::next(lst.begin()) << std::endl;
  //std::cout << &*std::next(lst.begin()) << std::endl;
  std::cout << *lst2.begin() << std::endl;
  std::cout << &*lst2.begin() << std::endl;
  std::cout << *lst3.begin() << std::endl;
  std::cout << &*lst3.begin() << std::endl;
  std::cout << *lst4.begin() << std::endl;
  std::cout << &*lst4.begin() << std::endl;
  std::cout << *std::next(lst4.begin()) << std::endl;
  std::cout << &*std::next(lst4.begin()) << std::endl;
  std::cout << "---" << std::endl;




  return 0;






}
