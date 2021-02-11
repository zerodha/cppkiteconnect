#find openSSL
find_package(OpenSSL REQUIRED)

add_executable(example2 "${CMAKE_CURRENT_SOURCE_DIR}/example2.cpp")
target_link_libraries(example2 PUBLIC OpenSSL::SSL OpenSSL::Crypto)