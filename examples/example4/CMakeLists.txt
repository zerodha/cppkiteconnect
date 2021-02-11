find_package(ZLIB REQUIRED)

file(GLOB_RECURSE source_list "${CMAKE_SOURCE_DIR}/example4.cpp")

#FIXME fix
link_directories("/opt/cpplibs/uWebSockets-0.14.8")

add_executable(example4 ${source_list})
target_link_libraries(example4 PUBLIC pthread OpenSSL::SSL OpenSSL::Crypto ZLIB::ZLIB uv uWS)
