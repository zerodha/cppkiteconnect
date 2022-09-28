# Find GTest
find_package(GTest REQUIRED)

# Find GMock
find_package(GMock REQUIRED)

# Find Threading library (GTest & GMock require threads)
find_package(Threads REQUIRED)

option(CMAKE_USE_WIN32_THREADS_INIT "using WIN32 threads" ON)
option(gtest_disable_pthreads "Disable uses of pthreads in gtest." ON)

# find openSSL
find_package(OpenSSL REQUIRED)

# kite-test
set(kiteTest_BINARY_NAME kiteTest)

file(GLOB kiteTestFiles
    "${CMAKE_SOURCE_DIR}/tests/unit/kite/*.cpp"
)

add_executable(${kiteTest_BINARY_NAME} ${kiteTestFiles})
add_test(NAME kite-test COMMAND ${kiteTest_BINARY_NAME})

target_include_directories(${kiteTest_BINARY_NAME} PUBLIC ${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})
target_link_libraries(${kiteTest_BINARY_NAME} PUBLIC OpenSSL::SSL OpenSSL::Crypto ${GTEST_BOTH_LIBRARIES} ${GMOCK_BOTH_LIBRARIES} Threads::Threads)

# find zlib
find_package(ZLIB REQUIRED)

# find libuv (optional on Linux)
if(DEFINED LINUX)
    find_library(UV_LIB uv)
    find_path(UV_INCLUDE "uv.h")
else()
    find_library(UV_LIB uv REQUIRED)
    find_path(UV_INCLUDE "uv.h" REQUIRED)
endif()

# find uWS
find_library(UWS_LIB uWS REQUIRED)
find_path(UWS_INCLUDE uWS REQUIRED)

# kws-test
set(kWSTest_BINARY_NAME kWSTest)

add_executable(${kWSTest_BINARY_NAME} "${CMAKE_SOURCE_DIR}/tests/unit/kwstest.cpp")
add_test(NAME kiteWS-test COMMAND ${kWSTest_BINARY_NAME})

if((NOT UV_LIB OR NOT UV_INCLUDE) AND DEFINED LINUX)
    message("Couldn't find libuv..\nProceding anyway since Linux was detected..")
    target_include_directories(${kWSTest_BINARY_NAME} PUBLIC ${UWS_INCLUDE} ${GTEST_INCLUDE_DIRS})
    target_link_libraries(${kWSTest_BINARY_NAME} PUBLIC OpenSSL::SSL OpenSSL::Crypto ZLIB::ZLIB ${UWS_LIB} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} Threads::Threads)
else()
    target_include_directories(${kWSTest_BINARY_NAME} PUBLIC ${UV_INCLUDE} ${UWS_INCLUDE} ${GTEST_INCLUDE_DIRS})
    target_link_libraries(${kWSTest_BINARY_NAME} PUBLIC OpenSSL::SSL OpenSSL::Crypto ZLIB::ZLIB ${UV_LIB} ${UWS_LIB} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} Threads::Threads)
endif()
