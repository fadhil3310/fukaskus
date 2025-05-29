cmake_minimum_required(VERSION 4.0)
project(fukaskus VERSION 0.1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(cxxopts REQUIRED)
# find_package(nlohmann_json REQUIRED)

add_executable(fukaskus
    src/main.cpp)

add_subdirectory(libs/fukaskus_writer)
add_subdirectory(libs/fukaskus_reader)
target_link_libraries(fukaskus
    PRIVATE fukaskus-writer
    PRIVATE fukaskus-reader)

target_link_libraries(fukaskus PRIVATE cxxopts::cxxopts)
# target_link_libraries(fukaskus nlohmann_json::nlohmann_json)