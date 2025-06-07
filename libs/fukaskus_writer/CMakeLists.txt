cmake_minimum_required(VERSION 3.30)
project(Writer VERSION 0.1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(nlohmann_json REQUIRED)


add_library(Writer
    src/specification_reader.cpp
    src/bundle_writer_32.cpp
    # src/bundle_creator.cpp
)
add_library(Fukaskus::Writer ALIAS Writer)

# if (NOT TARGET fukaskus-common)
#     add_subdirectory(../fukaskus_common ${CMAKE_CURRENT_BINARY_DIR}/fukaskus_common_build)
# endif()

target_include_directories(Writer
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(Writer
    PRIVATE nlohmann_json::nlohmann_json
)


# set_target_properties(Writer PROPERTIES
#     OUTPUT_NAME Writer
# )

# install(TARGETS Writer
#     EXPORT FukaskusTargets
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#     INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )

# install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/writer)

# install(EXPORT FukaskusWriterTargets
#     FILE FukaskusWriterTargets.cmake
#     NAMESPACE Fukaskus::
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fukaskus
# )