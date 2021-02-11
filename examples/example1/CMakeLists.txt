#find openSSL
find_package(OpenSSL REQUIRED)

add_executable(example1 "${CMAKE_CURRENT_SOURCE_DIR}/example1.cpp")
target_link_libraries(example1 PUBLIC OpenSSL::SSL OpenSSL::Crypto)

