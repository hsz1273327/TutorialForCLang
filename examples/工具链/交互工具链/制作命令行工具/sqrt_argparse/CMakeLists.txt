#项目编译环境
cmake_minimum_required (VERSION 3.17)
project (sqrt_argparse
  VERSION 0.0.0
  DESCRIPTION "使用c++实现的求开方的命令行工具"
  LANGUAGES CXX
)

# 下载依赖
message(NOTICE "下载外部依赖")
include(FetchContent)
## args
message(NOTICE "下载argparse")
FetchContent_Declare(argparse
  # GIT_REPOSITORY https://github.com/p-ranav/argparse.git
  # GIT_TAG        9903a22904fed8176c4a1f69c4b691304b23c78e
  # GIT_SHALLOW    TRUE
  URL  https://github.com/p-ranav/argparse/archive/refs/heads/master.zip
  TLS_VERIFY     FALSE
)
FetchContent_MakeAvailable(argparse)

# 编译目标
file(GLOB SRC "${CMAKE_CURRENT_LIST_DIR}/src/*.cc")
# 准备存放位置
file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/dist/bin)
# 设置编译目标
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} 
    PRIVATE ${SRC}
)
# 设置链接库
target_link_libraries(${PROJECT_NAME}
  PRIVATE argparse::argparse
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