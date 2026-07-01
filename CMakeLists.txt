cmake_minimum_required(VERSION 4.2)
project(KVStore)

set(CMAKE_CXX_STANDARD 20)

add_executable(KVStore src/main.cpp
        include/free_list_pool_allocator.h)
