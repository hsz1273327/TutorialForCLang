#项目编译环境
cmake_minimum_required (VERSION 3.17)
project (myvector
    VERSION 0.0.0
    DESCRIPTION "简单测试编译包"
    LANGUAGES CXX
)

# 设置源码位置
file(GLOB SRC "${CMAKE_CURRENT_LIST_DIR}/src/*.cc")
# 准备存放位置
file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/dist/lib)
# 编译为iobject对象减少重复编译
add_library(objlib OBJECT ${SRC})
target_include_directories(objlib
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc
)
set_target_properties(objlib PROPERTIES
    POSITION_INDEPENDENT_CODE 1
)
# 编译静态库
add_library(${PROJECT_NAME} STATIC)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    PRIVATE $<TARGET_OBJECTS:objlib>
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc
)
set_target_properties(${PROJECT_NAME} PROPERTIES 
    OUTPUT_NAME ${PROJECT_NAME}
    CLEAN_DIRECT_OUTPUT 1
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/dist/lib
    PUBLIC_HEADER ${CMAKE_CURRENT_LIST_DIR}/inc/binary_vector.hpp
)
# 编译动态库
option(WITHSHARE "build share lib also?" off)
if(WITHSHARE)
    message("also build share library")
    # shared and static libraries built from the same object files
    add_library(${PROJECT_NAME}_shared SHARED)
    target_sources(${PROJECT_NAME}_shared
        PRIVATE $<TARGET_OBJECTS:objlib>
    )
    target_include_directories(${PROJECT_NAME}_shared
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc
    )
    set_target_properties(${PROJECT_NAME}_shared PROPERTIES 
        OUTPUT_NAME ${PROJECT_NAME}
        CLEAN_DIRECT_OUTPUT 1
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/dist/lib
    )

    # 安装
    install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_shared
        ARCHIVE
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
else()
    message("only build static library")
    # 安装
    install(TARGETS ${PROJECT_NAME}
        ARCHIVE
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

#测试

message(NOTICE "下载外部依赖")
include(FetchContent)
## catch2
message(NOTICE "下载Catch2")
FetchContent_Declare(Catch2
  TLS_VERIFY     FALSE
  URL  https://github.com/catchorg/Catch2/archive/refs/tags/v2.13.5.tar.gz
)
FetchContent_MakeAvailable(Catch2)

# 准备存放位置
file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/dist/test)
add_executable(Test_${PROJECT_NAME} EXCLUDE_FROM_ALL)
# 设置源码位置
file(GLOB TESTSRC "${CMAKE_CURRENT_LIST_DIR}/test/test_*.cc")
target_sources(Test_${PROJECT_NAME} 
    PRIVATE ${TESTSRC}
)
# 设置头文件
target_include_directories(Test_${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/inc
)
# 设置链接库
target_link_libraries(Test_${PROJECT_NAME}
  PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
  PRIVATE Catch2::Catch2
)
# 设置可执行文件的存放位置
set_target_properties(Test_${PROJECT_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/dist/test
)
enable_testing()
add_test(Test_TDD ${CMAKE_CURRENT_LIST_DIR}/dist/test/Test_${PROJECT_NAME} Test_TDD)
add_test(Test_BDD ${CMAKE_CURRENT_LIST_DIR}/dist/test/Test_${PROJECT_NAME} Test_BDD)
add_test(Test_ALL ${CMAKE_CURRENT_LIST_DIR}/dist/test/Test_${PROJECT_NAME})