cmake_minimum_required(VERSION 3.14)
project(VIZ3D VERSION 1.0.0)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Whether to build tests with GTest (requires GTest)" OFF)
option(BUILD_VTK "Whether vtk was built from the sources" ON)

include(cmake/global_vars.cmake)
include(cmake/external.cmake)
include(cmake/test.cmake)

set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:${CMAKE_INSTALL_PREFIX}/lib")

add_subdirectory(src)

if (BUILD_TESTS)
    find_package(GTest REQUIRED)
    add_subdirectory(test)
endif ()