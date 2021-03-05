#项目编译环境
cmake_minimum_required (VERSION 2.8)
project (sqrt_opt)

# 编译连接生成demo
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)
aux_source_directory(${CMAKE_BINARY_DIR}/source/src DIR_SRCS)
add_executable(sqrt_opt ${DIR_SRCS})
#测试
enable_testing()
add_test (TEST_VERSION ${CMAKE_BINARY_DIR}/build/bin/sqrt_opt -v)
set_tests_properties (TEST_VERSION
  PROPERTIES PASS_REGULAR_EXPRESSION "v: 0.1.0")

add_test (TEST_36 ${CMAKE_BINARY_DIR}/build/bin/sqrt_opt -s36)
set_tests_properties (TEST_36
  PROPERTIES PASS_REGULAR_EXPRESSION "6.0")
add_test (TEST_2 ${CMAKE_BINARY_DIR}/build/bin/sqrt_opt -s2)
set_tests_properties (TEST_2
  PROPERTIES PASS_REGULAR_EXPRESSION "1.41")