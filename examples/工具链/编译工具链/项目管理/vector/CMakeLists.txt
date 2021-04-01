#项目编译环境
cmake_minimum_required (VERSION 3.17)
project (vector
    VERSION 0.0.0
    DESCRIPTION "简单测试"
    LANGUAGES C
)
include_directories(inc)
# 编译动态链接库
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
aux_source_directory(${CMAKE_BINARY_DIR}/src DIR_LIB_SRCS)

# 定义目标
add_library(vector STATIC ${DIR_LIB_SRCS})
target_compile_definitions(vector)
target_compile_features(vector)
target_include_directories(vector 
    PUBLIC inc
)

install(TARGETS   vector
    LIBRARY DESTINATION  CMAKE_LIBRARY_OUTPUT_DIRECTORY  
)  
# 编译连接生成demo
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/test)
link_directories(${CMAKE_BINARY_DIR}/build/lib)
link_libraries(vector)
aux_source_directory(test DIR_SRCS)
add_executable(vectorTest ${DIR_SRCS})
#测试
enable_testing()
add_test (DemoRuns ${CMAKE_BINARY_DIR}/build/test/vectorTest)