cmake_minimum_required (VERSION 2.8)
project (memory)
include_directories(${CMAKE_BINARY_DIR}/source/include)

# 编译静态链接库
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)

## 稽核实现

link_directories(${CMAKE_BINARY_DIR}/source/lib)
link_libraries(exception)
aux_source_directory(${CMAKE_BINARY_DIR}/source/src/memcheck DIR_MEMCHECK_LIB_SRCS)
add_library(memory STATIC ${DIR_MEMCHECK_LIB_SRCS})
set_target_properties(memory PROPERTIES PUBLIC_HEADER 
"source/include/memory.h")

install(TARGETS  memory
    ARCHIVE DESTINATION  ${CMAKE_BINARY_DIR}/build/lib
    PUBLIC_HEADER DESTINATION ${CMAKE_BINARY_DIR}/build/include
)

