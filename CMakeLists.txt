cmake_minimum_required(VERSION 3.14)
project(VIZ3D)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/external.cmake)
include(cmake/test.cmake)

add_subdirectory(src)