#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "nopayloadclient::nopayloadclient" for configuration ""
set_property(TARGET nopayloadclient::nopayloadclient APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(nopayloadclient::nopayloadclient PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libnopayloadclient.a"
  )

list(APPEND _cmake_import_check_targets nopayloadclient::nopayloadclient )
list(APPEND _cmake_import_check_files_for_nopayloadclient::nopayloadclient "${_IMPORT_PREFIX}/lib/libnopayloadclient.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
