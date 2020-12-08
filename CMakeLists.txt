cmake_minimum_required(VERSION 3.10.2)

project(kitepp) 

set(CMAKE_CXX_STANDARD 14)
string(APPEND CMAKE_CXX_FLAGS " -pthread")

set(LIBSDIR "/opt/cpplibs")

set(BOOSTVER "1_71_0")
set(Boost_NO_SYSTEM_PATHS TRUE)
set(BOOST_ROOT "${LIBSDIR}/boost_${BOOSTVER}")
set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}")
set(BOOST_LIBRARY_DIRS "${BOOST_ROOT}/stage/lib")
find_package(Boost REQUIRED filesystem chrono)

file(GLOB_RECURSE source_list "${CMAKE_SOURCE_DIR}/src/*.cpp" "{CMAKE_SOURCE_DIR}/src/*.hpp" "{CMAKE_SOURCE_DIR}/src/*.c")

include_directories(${BOOST_INCLUDE_DIRS} "${LIBSDIR}/nlohmann" "${LIBSDIR}/PicoSHA2" "${CMAKE_SOURCE_DIR}/include" )

find_package(cpprestsdk REQUIRED)

add_executable(kitepp ${source_list})
target_link_libraries(kitepp fmt cpprestsdk::cpprest ${Boost_LIBRARIES})
