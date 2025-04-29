cmake_minimum_required(VERSION 3.14)
project(PrimeDep)

set(CMAKE_CXX_STANDARD 23)

include(cmake/vendors.cmake)

add_subdirectory(lib)
add_subdirectory(uncooker)
add_subdirectory(packer)
