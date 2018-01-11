cmake_minimum_required (VERSION 2.8)
project (memory)
include_directories(${CMAKE_BINARY_DIR}/memory/include)

# 编译静态链接库
## 生产实现
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
aux_source_directory(${CMAKE_BINARY_DIR}/memory/src/memory DIR_LIB_SRCS)
add_library(memory STATIC ${DIR_LIB_SRCS})
install(TARGETS  memory
    ARCHIVE DESTINATION  CMAKE_LIBRARY_OUTPUT_DIRECTORY  
)
## 稽核实现
aux_source_directory(${CMAKE_BINARY_DIR}/memory/src/memcheck DIR_LIB_SRCS)
add_library(memcheck STATIC ${DIR_LIB_SRCS})
install(TARGETS  memcheck
    ARCHIVE DESTINATION  CMAKE_LIBRARY_OUTPUT_DIRECTORY  
)

install(FILES memory.h
    DESTINATION ${CMAKE_BINARY_DIR}/build/include
)
