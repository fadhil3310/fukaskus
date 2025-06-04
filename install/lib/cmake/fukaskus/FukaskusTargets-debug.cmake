#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Fukaskus::Writer" for configuration "Debug"
set_property(TARGET Fukaskus::Writer APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Fukaskus::Writer PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libWriter.a"
  )

list(APPEND _cmake_import_check_targets Fukaskus::Writer )
list(APPEND _cmake_import_check_files_for_Fukaskus::Writer "${_IMPORT_PREFIX}/lib/libWriter.a" )

# Import target "Fukaskus::Reader" for configuration "Debug"
set_property(TARGET Fukaskus::Reader APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Fukaskus::Reader PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libReader.a"
  )

list(APPEND _cmake_import_check_targets Fukaskus::Reader )
list(APPEND _cmake_import_check_files_for_Fukaskus::Reader "${_IMPORT_PREFIX}/lib/libReader.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
