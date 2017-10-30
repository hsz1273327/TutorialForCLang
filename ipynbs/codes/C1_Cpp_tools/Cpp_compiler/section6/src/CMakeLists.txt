
include_directories(../include)
aux_source_directory(. DIR_LIB_SRCS)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
add_library(Vector ${DIR_LIB_SRCS})