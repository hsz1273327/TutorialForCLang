#项目编译环境
cmake_minimum_required (VERSION 3.17)
project (sqrt_opt
  VERSION 0.0.0
  DESCRIPTION "使用c语言实现的求开方的命令行工具"
  LANGUAGES C
)


# 设置源码位置
file(GLOB SRC "${CMAKE_CURRENT_LIST_DIR}/src/*.c")
# 准备存放位置
file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/dist/bin)
# 设置编译目标
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} 
    PRIVATE ${SRC}
)
# 设置可执行文件的存放位置
set_target_properties(${PROJECT_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/dist/bin
)

# #测试
# enable_testing()
# add_test (TEST_VERSION ${CMAKE_BINARY_DIR}/build/bin/sqrt_opt -v)
# set_tests_properties (TEST_VERSION
#   PROPERTIES PASS_REGULAR_EXPRESSION "v: 0.1.0")

# add_test (TEST_36 ${CMAKE_BINARY_DIR}/build/bin/sqrt_opt -s36)
# set_tests_properties (TEST_36
#   PROPERTIES PASS_REGULAR_EXPRESSION "6.0")
# add_test (TEST_2 ${CMAKE_BINARY_DIR}/build/bin/sqrt_opt -s2)
# set_tests_properties (TEST_2
#   PROPERTIES PASS_REGULAR_EXPRESSION "1.41")