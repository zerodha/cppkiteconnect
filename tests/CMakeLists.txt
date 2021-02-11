# Locate GTest
find_package(GTest REQUIRED)
find_package(GMock REQUIRED)

# file(GLOB_RECURSE source_list "${CMAKE_SOURCE_DIR}/tests/*.cpp")
# set(RJTest_source_list "${CMAKE_SOURCE_DIR}/tests/rjtest.cpp")
# set(kiteTest_source_list "${CMAKE_SOURCE_DIR}/tests/kitetest.cpp")

# set(BINARY_NAME kitepp_test)

# add_executable(${BINARY_NAME} ${source_list})
# add_test(NAME ${BINARY_NAME} COMMAND ${BINARY_NAME})

# include_directories(${BINARY_NAME} ${GTEST_INCLUDE_DIRS})
# target_link_libraries(${BINARY_NAME} PUBLIC OpenSSL::SSL OpenSSL::Crypto ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})

#rapidjson-test
set(RJTest_source_list "${CMAKE_SOURCE_DIR}/tests/unit/rjtest.cpp")
set(RJTest_BINARY_NAME RJTest)

add_executable(${RJTest_BINARY_NAME} ${RJTest_source_list})
add_test(NAME rapidjson-test COMMAND ${RJTest_BINARY_NAME})

include_directories(${RJTest_BINARY_NAME} ${GTEST_INCLUDE_DIRS})
target_link_libraries(${RJTest_BINARY_NAME} PUBLIC ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})

#kite-test
set(kiteTest_source_list "${CMAKE_SOURCE_DIR}/tests/unit/kitetest.cpp")
set(kiteTest_BINARY_NAME kiteTest)

add_executable(${kiteTest_BINARY_NAME} ${kiteTest_source_list})
add_test(NAME kite-test COMMAND ${kiteTest_BINARY_NAME})

#include_directories(${kiteTest_BINARY_NAME} ${GTEST_INCLUDE_DIRS})
include_directories(${kiteTest_BINARY_NAME} ${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})
#target_link_libraries(${kiteTest_BINARY_NAME} PUBLIC OpenSSL::SSL OpenSSL::Crypto ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
target_link_libraries(${kiteTest_BINARY_NAME} PUBLIC OpenSSL::SSL OpenSSL::Crypto ${GTEST_BOTH_LIBRARIES} ${GMOCK_BOTH_LIBRARIES} pthread)
#FIXME will fail on windows while linking pthread. there's a solutuion that on SO and etc

#kws-test
#FIXME fix build
link_directories("/opt/cpplibs/uWebSockets-0.14.8")
find_package(ZLIB REQUIRED)
set(kWSTest_source_list "${CMAKE_SOURCE_DIR}/tests/unit/kwstest.cpp")
set(kWSTest_BINARY_NAME kWSTest)

add_executable(${kWSTest_BINARY_NAME} ${kWSTest_source_list})
add_test(NAME kiteWS-test COMMAND ${kWSTest_BINARY_NAME})

include_directories(${kWSTest_BINARY_NAME} ${GTEST_INCLUDE_DIRS})

target_link_libraries(${kWSTest_BINARY_NAME} PUBLIC OpenSSL::SSL OpenSSL::Crypto ZLIB::ZLIB uv uWS ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
