project(GenericRepo)

cmake_minimum_required(VERSION 3.2)

#
# Directory with modules
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")

#
# Set path
#
set(CMAKE_SYSROOT ${STAGING_DIR_TARGET})

set(LIB_PATH ${CMAKE_SYSROOT}/usr/lib)
set(INC_PATH ${CMAKE_SYSROOT}/usr/include)

MESSAGE(STATUS "---------------------------------------------------------------")
MESSAGE(STATUS "      STAGING_DIR_TARGET: " ${STAGING_DIR_TARGET})
MESSAGE(STATUS "                LIB_PATH: " ${LIB_PATH})
MESSAGE(STATUS "                INC_PATH: " ${INC_PATH})
MESSAGE(STATUS "---------------------------------------------------------------")

#
# Extra compiler / linker options
#
IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Werror -Wformat=2")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

#
# Configuration variables
#


#
# Packages to use
#
find_package(LibJsonCpp REQUIRED)
find_package(Threads REQUIRED)

#
# Include directories
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(${LIBJSONCPP_FOUND})
  include_directories(generic_repo ${LIBJSONCPP_INCLUDE_DIRS})
ENDIF(${LIBJSONCPP_FOUND})

IF(${INC_PATH})
  include_directories(${INC_PATH})
ENDIF(${INC_PATH})

#
# Sources to compile
#
set(REPO_SOURCES
    src/main.cpp
)

#
# Targets
#
add_executable(generic_repo ${REPO_SOURCES})

set_property(TARGET generic_repo PROPERTY CXX_STANDARD 17)

target_link_libraries(generic_repo ${CMAKE_THREAD_LIBS_INIT})

if(${LIBJSONCPP_FOUND})
  target_link_libraries(generic_repo ${LIBJSONCPP_LIBRARIES})
endif(${LIBJSONCPP_FOUND})

IF(${LIB_PATH})
  target_link_libraries(generic_repo ${LIB_PATH})
ENDIF(${LIB_PATH})
