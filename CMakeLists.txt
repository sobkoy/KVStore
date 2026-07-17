cmake_minimum_required(VERSION 4.2)
project(KVStore)

set(CMAKE_CXX_STANDARD 20)

add_executable(kv_store src/store/main.cc
        src/store/hash_map.cc
        src/store/key_value_store.cc)

target_include_directories(kv_store
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include/store
)

target_compile_options(kv_store PRIVATE -ftime-report)


# Server part
add_executable(server src/server/main.cc
        src/server/server.cc)

target_include_directories(server
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include/server
)