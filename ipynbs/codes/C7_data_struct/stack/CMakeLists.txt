cmake_minimum_required (VERSION 2.8)
project (exception)
include_directories(${CMAKE_BINARY_DIR}/include)

# 编译动态链接库
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
aux_source_directory(${CMAKE_BINARY_DIR}/src DIR_LIB_SRCS)
add_library(exception SHARED ${DIR_LIB_SRCS})
install(TARGETS  exception
    LIBRARY DESTINATION  CMAKE_LIBRARY_OUTPUT_DIRECTORY  
)

# 编译连接生成demo
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
link_directories(${CMAKE_BINARY_DIR}/lib)
link_libraries(exception)
aux_source_directory(test DIR_SRCS)
add_executable(exception_test ${DIR_SRCS})

#测试
enable_testing()
add_test(exception_test ${CMAKE_BINARY_DIR}/bin/exception_test)