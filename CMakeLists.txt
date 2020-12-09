cmake_minimum_required(VERSION 3.0)

project(swt VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

add_subdirectory(src_swt)
add_subdirectory(src_cswt)
#add_subdirectory(src_swtp)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
add_subdirectory(tests)
endif()
