cmake_minimum_required (VERSION 2.8)
project (format)
include_directories(${CMAKE_BINARY_DIR}/format/include)

# 编译静态链接库
## 生产实现
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
aux_source_directory(${CMAKE_BINARY_DIR}/format DIR_LIB_SRCS)
add_library(format STATIC ${DIR_LIB_SRCS})
install(TARGETS  format
    ARCHIVE DESTINATION  CMAKE_LIBRARY_OUTPUT_DIRECTORY  
)

install(FILES format.h
    DESTINATION ${CMAKE_BINARY_DIR}/build/include
)
