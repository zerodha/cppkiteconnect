cmake_minimum_required(VERSION 3.10.2)

project(kitepp) 

set(CMAKE_CXX_STANDARD 11)
string(APPEND CMAKE_CXX_FLAGS " -pthread")

file(GLOB_RECURSE source_list "${CMAKE_SOURCE_DIR}/src/main.cpp")
include_directories( "${CMAKE_SOURCE_DIR}/include" )

find_package(cpprestsdk REQUIRED)

add_executable(kitepp ${source_list})
target_link_libraries(kitepp cpprestsdk::cpprest)
