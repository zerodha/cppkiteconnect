cmake_minimum_required(VERSION 3.10)
project(kitepp) 

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

include(CTest)

include_directories("${CMAKE_SOURCE_DIR}/include")

#build examples
add_subdirectory("examples/example1")
add_subdirectory("examples/example2")
add_subdirectory("examples/example3")
add_subdirectory("examples/example4")

#build tests
if(BUILD_TESTING)
        add_subdirectory("tests")
endif()

#generate documentation
add_subdirectory("docs")



