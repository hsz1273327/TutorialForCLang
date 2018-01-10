cmake_minimum_required (VERSION 2.8)
project (memory)
include_directories(${CMAKE_BINARY_DIR}/include ${CMAKE_BINARY_DIR}/thirdpart/include)

# 编译动态链接库
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
aux_source_directory(${CMAKE_BINARY_DIR}/src/memory DIR_LIB_SRCS)
add_library(memory SHARED ${DIR_LIB_SRCS})
install(TARGETS  memory
    LIBRARY DESTINATION  CMAKE_LIBRARY_OUTPUT_DIRECTORY  
)
aux_source_directory(${CMAKE_BINARY_DIR}/src/memcheck DIR_LIB_CHECK_SRCS)
add_library(memcheck SHARED ${DIR_LIB_CHECK_SRCS})
install(TARGETS  memcheck
    LIBRARY DESTINATION  CMAKE_LIBRARY_OUTPUT_DIRECTORY  
)

