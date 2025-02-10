#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "message2::message2__rosidl_generator_py" for configuration ""
set_property(TARGET message2::message2__rosidl_generator_py APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(message2::message2__rosidl_generator_py PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libmessage2__rosidl_generator_py.so"
  IMPORTED_SONAME_NOCONFIG "libmessage2__rosidl_generator_py.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS message2::message2__rosidl_generator_py )
list(APPEND _IMPORT_CHECK_FILES_FOR_message2::message2__rosidl_generator_py "${_IMPORT_PREFIX}/lib/libmessage2__rosidl_generator_py.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
