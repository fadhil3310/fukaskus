cmake_minimum_required(VERSION 3.30)
project(Fukaskus VERSION 0.1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_WRITER "Build Fukaskus::Writer" ON)
option(BUILD_READER "Build Fukaskus::Reader" ON)
option(BUILD_CLI "Build FukaskusCLI" ON)

# Check if BUILD_WRITER/BUILD_READER is turned off while BUILD_CLI is turned on.
if (BUILD_CLI AND NOT BUILD_WRITER OR NOT BUILD_READER)
    message(FATAL_ERROR, "FukaskusCLI depends on Fukaskus::Writer and Fukaskus::Reader")
endif()
    
if(BUILD_WRITER)
    add_subdirectory(libs/fukaskus_writer)
    add_library(Fukaskus::Writer ALIAS Writer)
    list(APPEND LibsBuilt "Writer")
endif()
if(BUILD_READER) 
    add_subdirectory(libs/fukaskus_reader)
    add_library(Fukaskus::Reader ALIAS Reader)
    list(APPEND LibsBuilt "Reader")
endif()

    
include(CMakePackageConfigHelpers)


install(TARGETS ${LibsBuilt}
    EXPORT FukaskusTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# if(BUILD_WRITER)
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/fukaskus_writer/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/writer)
# endif()
# if(BUILD_READER)
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/libs/fukaskus_reader/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/reader)
# endif()

install(EXPORT FukaskusTargets
    FILE FukaskusTargets.cmake
    NAMESPACE Fukaskus::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fukaskus
)


configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/FukaskusConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/FukaskusConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fukaskus
)

# install(TARGETS Fukaskus
#     EXPORT FukaskusTargets
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#     INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )


# configure_package_config_file(
#     cmake/FukaskusConfig.cmake.in
#     ${CMAKE_CURRENT_BINARY_DIR}/FukaskusConfig.cmake
#     INSTALL_DESTINATION lib/cmake/fukaskus
# )

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/FukaskusConfig.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fukaskus
)


if (BUILD_CLI)
    add_subdirectory(cli)
endif()

# target_link_libraries(fukaskus
#     PRIVATE fukaskus-writer
#     PRIVATE fukaskus-reader)

# target_link_libraries(fukaskus PRIVATE cxxopts::cxxopts)
# target_link_libraries(fukaskus nlohmann_json::nlohmann_json)