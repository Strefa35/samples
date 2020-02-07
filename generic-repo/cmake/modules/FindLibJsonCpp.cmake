# - Try to find JsonCpp
#
# Once done this will define
#  LIBJSONCPP_FOUND           - System has the component
#  LIBJSONCPP_INCLUDE_DIRS    - Component include directories
#  LIBJSONCPP_LIBRARIES       - Libraries needed to use the component
#  LIBJSONCPP_DEFINITIONS     - Compiler switches required for using component


find_package(PkgConfig)
pkg_check_modules(PC_LIBJSONCPP QUIET libjsoncpp)
set(LIBJSONCPP_DEFINITIONS ${PC_LIBJSONCPP_CFLAGS_OTHER})

find_path(LIBJSONCPP_INCLUDE_DIR
          NAMES json/json.h
          HINTS ${PC_LIBJSONCPP_INCLUDEDIR} ${PC_LIBJSONCPP_INCLUDE_DIRS}
          PATH_SUFFIXES jsoncpp
          )

find_library(LIBJSONCPP_LIBRARY
             NAMES jsoncpp
             HINTS ${PC_LIBJSONCPP_INCLUDEDIR} ${PC_LIBJSONCPP_INCLUDE_DIRS}
             )

message(STATUS "LIBJSONCPP_INCLUDE_DIR = ${LIBJSONCPP_INCLUDE_DIR}")
message(STATUS "LIBJSONCPP_LIBRARY     = ${LIBJSONCPP_LIBRARY}")

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set component to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibJsonCpp DEFAULT_MSG
                                  LIBJSONCPP_LIBRARY
                                  LIBJSONCPP_INCLUDE_DIR)

mark_as_advanced(LIBJSONCPP_INCLUDE_DIR LIBJSONCPP_LIBRARY)

set(LIBJSONCPP_LIBRARIES ${LIBJSONCPP_LIBRARY})
set(LIBJSONCPP_INCLUDE_DIRS ${LIBJSONCPP_INCLUDE_DIR})
