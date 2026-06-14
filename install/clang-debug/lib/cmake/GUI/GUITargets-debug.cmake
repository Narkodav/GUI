#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "GUI::GUI" for configuration "Debug"
set_property(TARGET GUI::GUI APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(GUI::GUI PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libGUI.a"
  )

list(APPEND _cmake_import_check_targets GUI::GUI )
list(APPEND _cmake_import_check_files_for_GUI::GUI "${_IMPORT_PREFIX}/lib/libGUI.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
