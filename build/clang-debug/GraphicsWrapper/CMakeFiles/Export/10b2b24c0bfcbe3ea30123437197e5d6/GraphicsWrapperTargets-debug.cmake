#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "GraphicsWrapper::GraphicsWrapper" for configuration "Debug"
set_property(TARGET GraphicsWrapper::GraphicsWrapper APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(GraphicsWrapper::GraphicsWrapper PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libGraphicsWrapper.a"
  )

list(APPEND _cmake_import_check_targets GraphicsWrapper::GraphicsWrapper )
list(APPEND _cmake_import_check_files_for_GraphicsWrapper::GraphicsWrapper "${_IMPORT_PREFIX}/lib/libGraphicsWrapper.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
