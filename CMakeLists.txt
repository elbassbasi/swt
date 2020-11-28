cmake_minimum_required(VERSION 3.0)

project(swt VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

add_subdirectory(src_swt)
add_subdirectory(src_cswt)
add_subdirectory(src_swtp)
add_subdirectory(tests)
