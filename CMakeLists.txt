cmake_minimum_required(VERSION 4.2)
project(KVStore)

set(CMAKE_CXX_STANDARD 20)

add_executable(KVStore src/main.cc
        src/hash_map.cc
        src/key_value_store.cc)

target_include_directories(KVStore
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(KVStore PRIVATE -ftime-report)