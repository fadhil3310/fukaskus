cmake_minimum_required(VERSION 3.30)
project(Reader VERSION 0.1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_library(Reader
    src/bundle_reader_32.cpp
)
add_library(Fukaskus::Reader ALIAS Reader)

# if (NOT TARGET fukaskus-common)
#     add_subdirectory(../fukaskus_common ${CMAKE_CURRENT_BINARY_DIR}/fukaskus_common_build)
# endif()

target_include_directories(Reader
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)


# set_target_properties(Reader PROPERTIES
#     OUTPUT_NAME Reader
# )

# install(TARGETS Reader
#     EXPORT FukaskusTargets
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#     INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )

# install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/reader)

# install(EXPORT FukaskusTargets
#     FILE FukaskusTargets.cmake
#     NAMESPACE Fukaskus::
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fukaskus
# )