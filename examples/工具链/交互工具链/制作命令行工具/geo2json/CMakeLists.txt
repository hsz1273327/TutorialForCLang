#项目编译环境
cmake_minimum_required (VERSION 3.17)
project (geo2json
    VERSION 0.0.0
    DESCRIPTION "地理信息csv转到json格式"
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
