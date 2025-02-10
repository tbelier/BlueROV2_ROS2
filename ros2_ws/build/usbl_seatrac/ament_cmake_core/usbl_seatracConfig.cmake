# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_usbl_seatrac_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED usbl_seatrac_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(usbl_seatrac_FOUND FALSE)
  elseif(NOT usbl_seatrac_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(usbl_seatrac_FOUND FALSE)
  endif()
  return()
endif()
set(_usbl_seatrac_CONFIG_INCLUDED TRUE)

# output package information
if(NOT usbl_seatrac_FIND_QUIETLY)
  message(STATUS "Found usbl_seatrac: 0.0.0 (${usbl_seatrac_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'usbl_seatrac' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${usbl_seatrac_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(usbl_seatrac_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${usbl_seatrac_DIR}/${_extra}")
endforeach()
