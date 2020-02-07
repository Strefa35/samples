## CMAKE build
cmake_minimum_required(VERSION 3.2)

project(dir)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

set(VERSION "0.0.1")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOPT_VERSION=\\\"${VERSION}\\\"")

include_directories(/usr/include/)

file(GLOB_RECURSE SOURCE_FILES dir.c)

add_executable(dir ${SOURCE_FILES})
