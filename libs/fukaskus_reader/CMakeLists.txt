cmake_minimum_required(VERSION 4.0)
project(fukaskus-writer VERSION 0.1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_library(fukaskus-reader
    src/bundle_reader_32.cpp
)

# if (NOT TARGET fukaskus-common)
#     add_subdirectory(../fukaskus_common ${CMAKE_CURRENT_BINARY_DIR}/fukaskus_common_build)
# endif()

target_include_directories(fukaskus-reader 
    PUBLIC ./include)

# target_link_libraries(fukaskus-reader
#     PRIVATE fukaskus-common)