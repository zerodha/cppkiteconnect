# Locate GTest
find_package(GTest REQUIRED)

file(GLOB_RECURSE source_list "${CMAKE_SOURCE_DIR}/tests/*.cpp")

set(BINARY_NAME kitepp_test)

add_executable(${BINARY_NAME} ${source_list})
add_test(NAME ${BINARY_NAME} COMMAND ${BINARY_NAME})

include_directories(${BINARY_NAME} ${GTEST_INCLUDE_DIRS})
target_link_libraries(${BINARY_NAME} PUBLIC OpenSSL::SSL OpenSSL::Crypto ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})