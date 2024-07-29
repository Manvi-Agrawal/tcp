cmake_minimum_required(VERSION 3.22.1)
project(tcp-ip)

# We will use C++ 17 for this Assignment 2
set(CMAKE_CXX_STANDARD 17)


include_directories("${PROJECT_SOURCE_DIR}/src/util/")
include_directories("${PROJECT_SOURCE_DIR}/src/")
include_directories("${PROJECT_SOURCE_DIR}/src/tcp_helpers/")

add_subdirectory(src)

enable_testing()

add_subdirectory(tests)

include(CTest)
