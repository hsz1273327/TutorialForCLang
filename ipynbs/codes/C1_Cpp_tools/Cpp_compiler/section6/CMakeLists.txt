
cmake_minimum_required (VERSION 2.8)
project (binary_vector)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

aux_source_directory(test DIR_SRCS)
add_subdirectory(src)
add_executable(Demo ${DIR_SRCS})