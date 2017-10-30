
cmake_minimum_required (VERSION 2.8)
project (Demo1)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
aux_source_directory(src DIR_SRCS)
add_executable(Demo ${DIR_SRCS})