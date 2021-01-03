file(GLOB_RECURSE source_list "${CMAKE_SOURCE_DIR}/example2.cpp")

add_executable(example2 ${source_list})
target_link_libraries(example2 PUBLIC OpenSSL::SSL OpenSSL::Crypto)
