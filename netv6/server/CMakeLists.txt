## 
cmake_minimum_required(VERSION 3.2)

project(n6server)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB_RECURSE SOURCE_FILES_INFO n6server.cpp)

add_executable(n6server ${SOURCE_FILES_INFO})
