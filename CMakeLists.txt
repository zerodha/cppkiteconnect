cmake_minimum_required(VERSION 3.11)
project(kitepp) 

#detect Linux (for optional linking of libuv)
if(UNIX AND NOT APPLE)
        set(LINUX TRUE)
endif()

#set variables
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

#set project include directory
include_directories("${CMAKE_SOURCE_DIR}/include")

#build examples
add_subdirectory("examples/example1")
add_subdirectory("examples/example2")
add_subdirectory("examples/example3")
add_subdirectory("examples/example4")

#build tests
include(CTest)
if(BUILD_TESTING)
        add_subdirectory("tests")
endif()

#generate documentation
add_subdirectory("docs")

#download dependancies
if(GET_ALL_DEPS OR GET_UWS)
        add_subdirectory("deps")
endif()



