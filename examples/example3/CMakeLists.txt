find_package(ZLIB REQUIRED)

file(GLOB_RECURSE source_list "${CMAKE_SOURCE_DIR}/example3.cpp")

#FIXME fix
link_directories("/opt/cpplibs/uWebSockets-0.14.8")

add_executable(example3 ${source_list})
target_link_libraries(example3 PUBLIC OpenSSL::SSL OpenSSL::Crypto ZLIB::ZLIB uv uWS)
