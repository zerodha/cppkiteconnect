# Find GTest
find_package(GTest REQUIRED)

#Find GMock
find_package(GMock REQUIRED)

#rapidjson-test
set(RJTest_BINARY_NAME RJTest)

add_executable(${RJTest_BINARY_NAME} "${CMAKE_SOURCE_DIR}/tests/unit/rjtest.cpp")
add_test(NAME rapidjson-test COMMAND ${RJTest_BINARY_NAME})

target_include_directories(${RJTest_BINARY_NAME} PUBLIC ${GTEST_INCLUDE_DIRS})
target_link_libraries(${RJTest_BINARY_NAME} PUBLIC ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})

#find openSSL
find_package(OpenSSL REQUIRED)

#kite-test
set(kiteTest_BINARY_NAME kiteTest)

#Find Threading library (GMock requires threads)
find_package (Threads REQUIRED)

option(CMAKE_USE_WIN32_THREADS_INIT "using WIN32 threads" ON)
option(gtest_disable_pthreads "Disable uses of pthreads in gtest." ON)

add_executable(${kiteTest_BINARY_NAME} "${CMAKE_SOURCE_DIR}/tests/unit/kitetest.cpp")
add_test(NAME kite-test COMMAND ${kiteTest_BINARY_NAME})

target_include_directories(${kiteTest_BINARY_NAME} PUBLIC ${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})
target_link_libraries(${kiteTest_BINARY_NAME} PUBLIC OpenSSL::SSL OpenSSL::Crypto ${GTEST_BOTH_LIBRARIES} ${GMOCK_BOTH_LIBRARIES} Threads::Threads)

#find zlib
find_package(ZLIB REQUIRED)

#find libuv (optional on Linux)
find_library(UV_LIB uv)
find_path(UV_INCLUDE "uv.h")

#find uWS
find_library(UWS_LIB uWS REQUIRED)
find_path(UWS_INCLUDE uWS REQUIRED)

#kws-test
set(kWSTest_BINARY_NAME kWSTest)

add_executable(${kWSTest_BINARY_NAME} "${CMAKE_SOURCE_DIR}/tests/unit/kwstest.cpp")
add_test(NAME kiteWS-test COMMAND ${kWSTest_BINARY_NAME})

target_include_directories(${kWSTest_BINARY_NAME} PUBLIC ${UV_INCLUDE} ${UWS_INCLUDE} ${GTEST_INCLUDE_DIRS})
target_link_libraries(${kWSTest_BINARY_NAME} PUBLIC OpenSSL::SSL OpenSSL::Crypto ZLIB::ZLIB ${UV_LIB} ${UWS_LIB} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
