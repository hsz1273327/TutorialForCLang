cmake_minimum_required (VERSION 2.8)
project (exception)
include_directories(${CMAKE_BINARY_DIR}/source/include)

# 编译静态链接库
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
aux_source_directory(${CMAKE_BINARY_DIR}/source/src DIR_LIB_SRCS)
add_library(exception STATIC ${DIR_LIB_SRCS})
install(TARGETS  exception
    ARCHIVE DESTINATION  CMAKE_LIBRARY_OUTPUT_DIRECTORY  
)
install(FILES assert.h except.h
        DESTINATION ${CMAKE_BINARY_DIR}/build/include
)

# 编译连接生成测试
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test/bin)
link_directories(CMAKE_LIBRARY_OUTPUT_DIRECTORY)
link_libraries(exception)
aux_source_directory(${CMAKE_BINARY_DIR}/test/src DIR_SRCS)
add_executable(exception_test ${DIR_SRCS})

#测试
enable_testing()
add_test(exception_test ${CMAKE_BINARY_DIR}/test/bin/exception_test)