#项目编译环境
cmake_minimum_required (VERSION 2.8)
project (binary_vector)
include_directories(source/inc)
# 编译动态链接库
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
aux_source_directory(${CMAKE_BINARY_DIR}/source/src DIR_LIB_SRCS)
add_library(vector SHARED ${DIR_LIB_SRCS})
install(TARGETS   vector
    LIBRARY DESTINATION  CMAKE_LIBRARY_OUTPUT_DIRECTORY  
)  
# 编译连接生成demo
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)
link_directories(${CMAKE_BINARY_DIR}/build/lib)
link_libraries(vector)
aux_source_directory(test DIR_SRCS)
add_executable(Demo_Test ${DIR_SRCS})

#测试
enable_testing()
add_test (TestRuns ${CMAKE_BINARY_DIR}/build/bin/Demo_Test)