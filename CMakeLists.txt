cmake_minimum_required(VERSION 3.10)
project(CppKiteConnect)

# set variables
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

# detect Linux (for optional linking of libuv)
if(UNIX AND NOT APPLE)
        message(STATUS "environment set to linux")
        set(LINUX TRUE)
endif()

# set project include directory
include_directories("${CMAKE_SOURCE_DIR}/include")

# find deps
set(LINUX_AND_UV_NOT_FOUND false)

if(BUILD_EXAMPLES OR BUILD_TESTS)
        find_package(Threads REQUIRED)
        find_package(OpenSSL REQUIRED)
        find_package(ZLIB REQUIRED)
        find_library(UWS_LIB uWS REQUIRED)
        find_path(UWS_INCLUDE uWS REQUIRED)

        if(DEFINED LINUX)
                find_library(UV_LIB uv)
                find_path(UV_INCLUDE "uv.h")
        else()
                find_library(UV_LIB uv REQUIRED)
                find_path(UV_INCLUDE "uv.h" REQUIRED)
        endif()

        if(BUILD_TESTS)
                find_package(GTest REQUIRED)
                find_package(GMock REQUIRED)
        endif()

        if((NOT UV_LIB OR NOT UV_INCLUDE) AND DEFINED LINUX)
                set(LINUX_AND_UV_NOT_FOUND true)
                message(STATUS "couldn't find libuv")
        endif()
endif()

# build examples
if(BUILD_EXAMPLES)
        function(build_exmaple example_name)
                add_executable(${example_name} "${CMAKE_SOURCE_DIR}/examples/${example_name}.cpp")

                if(LINUX_AND_UV_NOT_FOUND)
                        target_include_directories(${example_name} PUBLIC ${UWS_INCLUDE})
                        target_link_libraries(${example_name} PUBLIC Threads::Threads OpenSSL::SSL OpenSSL::Crypto ZLIB::ZLIB ${UWS_LIB})
                else()
                        target_include_directories(${example_name} PUBLIC ${UWS_INCLUDE} ${UV_INCLUDE})
                        target_link_libraries(${example_name} PUBLIC Threads::Threads OpenSSL::SSL OpenSSL::Crypto ZLIB::ZLIB ${UWS_LIB} ${UV_LIB})
                endif()
        endfunction(build_exmaple)

        build_exmaple(example1)
        build_exmaple(example2)
        build_exmaple(example3)
        build_exmaple(example4)
endif()

# build tests
if(BUILD_TESTS)
        include(CTest)
        option(CMAKE_USE_WIN32_THREADS_INIT "using WIN32 threads" ON)
        option(gtest_disable_pthreads "disabling pthreads in gtest" ON)

        # kite-test
        set(KITE_TEST_BINARY_NAME kiteTest)
        file(GLOB test_files
                "${CMAKE_SOURCE_DIR}/tests/unit/kite/*.cpp"
        )
        add_executable(${KITE_TEST_BINARY_NAME} ${test_files})
        target_include_directories(${KITE_TEST_BINARY_NAME} PUBLIC ${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})
        target_link_libraries(${KITE_TEST_BINARY_NAME} PUBLIC OpenSSL::SSL OpenSSL::Crypto ${GTEST_BOTH_LIBRARIES} ${GMOCK_BOTH_LIBRARIES} Threads::Threads)
        add_test(NAME kite-test COMMAND ${KITE_TEST_BINARY_NAME})

        # ticker-test
        set(TICKER_TEST_BINARY_NAME tickerTest)
        add_executable(${TICKER_TEST_BINARY_NAME} "${CMAKE_SOURCE_DIR}/tests/unit/tickertest.cpp")

        if(LINUX_AND_UV_NOT_FOUND)
                target_include_directories(${TICKER_TEST_BINARY_NAME} PUBLIC ${UWS_INCLUDE} ${GTEST_INCLUDE_DIRS})
                target_link_libraries(${TICKER_TEST_BINARY_NAME} PUBLIC OpenSSL::SSL OpenSSL::Crypto ZLIB::ZLIB ${UWS_LIB} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} Threads::Threads)
        else()
                target_include_directories(${TICKER_TEST_BINARY_NAME} PUBLIC ${UV_INCLUDE} ${UWS_INCLUDE} ${GTEST_INCLUDE_DIRS})
                target_link_libraries(${TICKER_TEST_BINARY_NAME} PUBLIC OpenSSL::SSL OpenSSL::Crypto ZLIB::ZLIB ${UV_LIB} ${UWS_LIB} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} Threads::Threads)
        endif()

        add_test(NAME ticker-test COMMAND ${TICKER_TEST_BINARY_NAME})
endif()

# generate docs
if(BUILD_DOCS)
        find_package(Doxygen)

        if(DOXYGEN_FOUND)
                # set io files and configure doxyfile
                set(DOXY_FILE Doxyfile.in)
                set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/cmake/templates/${DOXY_FILE})
                set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.out)
                configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

                # ! don't add to the `all` target. doing so will build build docs everytime.
                add_custom_target(docs
                        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        COMMENT "generating documentation with doxygen"
                        VERBATIM)
        else(DOXYGEN_FOUND)
                message(FATAL_ERROR "doxygen is required for generating documentation")
        endif(DOXYGEN_FOUND)
endif()