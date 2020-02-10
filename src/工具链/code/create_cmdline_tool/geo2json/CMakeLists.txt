#项目编译环境
cmake_minimum_required (VERSION 2.8)
project (geo2json)

# 编译连接生成demo
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)
aux_source_directory(${CMAKE_BINARY_DIR}/source/src DIR_SRCS)
add_executable(geo2json ${DIR_SRCS})