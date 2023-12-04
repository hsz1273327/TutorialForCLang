#项目编译环境
cmake_minimum_required (VERSION 3.17)
project (myvector
    VERSION 0.0.0
    DESCRIPTION "简单测试编译包"
    LANGUAGES C
)

# 设置源码位置
file(GLOB SRC "${CMAKE_CURRENT_LIST_DIR}/src/*.c")
# 准备存放位置
file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib)
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
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib
    PUBLIC_HEADER ${CMAKE_CURRENT_LIST_DIR}/inc/binary_vector.h
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
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build/lib
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

# #测试
# # 编译连接生成demo
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/test)
# link_directories(${CMAKE_BINARY_DIR}/build/lib)
# link_libraries(vector)
# aux_source_directory(test DIR_SRCS)
# add_executable(vectorTest ${DIR_SRCS})
# enable_testing()
# add_test (DemoRuns ${CMAKE_BINARY_DIR}/build/test/vectorTest)