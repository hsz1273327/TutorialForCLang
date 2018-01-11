cmake_minimum_required (VERSION 2.8)
project (memorypool)
include_directories(${CMAKE_BINARY_DIR}/memorypool/include)

# 编译静态链接库
## 生产实现
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
aux_source_directory(${CMAKE_BINARY_DIR}/memorypool/src DIR_LIB_SRCS)
add_library(memorypool STATIC ${DIR_LIB_SRCS})
install(TARGETS  memorypool
    ARCHIVE DESTINATION  CMAKE_LIBRARY_OUTPUT_DIRECTORY  
)

install(FILES memorypool.h
    DESTINATION ${CMAKE_BINARY_DIR}/build/include
)
