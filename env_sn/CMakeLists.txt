## CMAKE build
cmake_minimum_required(VERSION 3.2)

project(env_sn)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(VERSION "0.0.1")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DOPT_VERSION=\\\"${VERSION}\\\"")

file(GLOB_RECURSE SOURCE_FILES env_sn.cpp)

add_executable(env_sn ${SOURCE_FILES})
