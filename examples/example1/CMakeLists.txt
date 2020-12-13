file(GLOB_RECURSE source_list "${CMAKE_SOURCE_DIR}/example1.cpp")

add_executable(example1 ${source_list})
target_link_libraries(example1 cpprestsdk::cpprest)
