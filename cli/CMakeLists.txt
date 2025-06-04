cmake_minimum_required(VERSION 3.30)
project(FukaskusCLI VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(cxxopts REQUIRED)
find_package(Fukaskus REQUIRED)

add_executable(FukaskusCLI
    main.cpp)

target_link_libraries(FukaskusCLI
    PRIVATE
    cxxopts::cxxopts
    Fukaskus::Writer
    Fukaskus::Reader
)

target_include_directories(FukaskusCLI
    PRIVATE
    # $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../libs/fukaskus-writer/include
    # $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../libs/fukaskus-reader/include
    ${Fukaskus_INCLUDE_DIRS}
)

install(TARGETS FukaskusCLI
    RUNTIME DESTINATION bin
)