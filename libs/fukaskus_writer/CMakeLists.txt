cmake_minimum_required(VERSION 4.0)
project(fukaskus-writer VERSION 0.1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(nlohmann_json REQUIRED)


add_library(fukaskus-writer
    src/specification_reader.cpp
    src/bundle_writer_32.cpp
    # src/bundle_creator.cpp
)

# if (NOT TARGET fukaskus-common)
#     add_subdirectory(../fukaskus_common ${CMAKE_CURRENT_BINARY_DIR}/fukaskus_common_build)
# endif()

target_include_directories(fukaskus-writer 
    PUBLIC ./include)

target_link_libraries(fukaskus-writer
    # PRIVATE fukaskus-common
    PRIVATE nlohmann_json::nlohmann_json)