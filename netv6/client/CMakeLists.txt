## 
cmake_minimum_required(VERSION 3.2)

project(n6client)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB_RECURSE SOURCE_FILES_INFO n6client.cpp)

add_executable(n6client ${SOURCE_FILES_INFO})
