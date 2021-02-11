#find openSSL
find_package(OpenSSL REQUIRED)

#find zlib
find_package(ZLIB REQUIRED)

#find libuv (optional on Linux)
find_library(UV_LIB uv)
find_path(UV_INCLUDE "uv.h")

#find uWS
find_library(UWS_LIB uWS REQUIRED)
find_path(UWS_INCLUDE uWS REQUIRED)

add_executable(example4 "${CMAKE_CURRENT_SOURCE_DIR}/example4.cpp")
target_include_directories(example4 PUBLIC ${UWS_INCLUDE} ${UV_INCLUDE})
target_link_libraries(example4 PUBLIC pthread OpenSSL::SSL OpenSSL::Crypto ZLIB::ZLIB ${UWS_LIB} ${UV_LIB})
