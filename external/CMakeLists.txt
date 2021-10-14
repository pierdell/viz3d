cmake_minimum_required(VERSION 3.14)
project(VIZ3D-EXTERNAL)

option(BUILD_GLOG "Whether to build GLOG" ON)
option(BUILD_EIGEN "Whether to build EIGEN" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(ExternalProject)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/global_vars.cmake)
set(GLOBAL_RUNTIME_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

if (BUILD_GLOG)
    # INSTALL GLOG
    set(GLOG_DEF "glog")
    ExternalProject_Add(
            ${GLOG_DEF}
            PREFIX ${GLOG_DEF}

            GIT_REPOSITORY https://github.com/google/glog
            GIT_TAG v0.5.0

            BUILD_ALWAYS OFF
            INSTALL_DIR ${INSTALL_ROOT}/glog
            CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${INSTALL_ROOT}/glog

            STEP_TARGETS glog-build
            BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config ${CMAKE_BUILD_TYPE} --target install)
endif ()

if (BUILD_EIGEN)
    # INSTALL EIGEN
    ExternalProject_Add(
            Eigen3
            PREFIX Eigen3

            GIT_REPOSITORY https://gitlab.com/libeigen/eigen
            GIT_TAG 3.3.7

            BUILD_ALWAYS OFF
            INSTALL_DIR ${INSTALL_ROOT}/Eigen3
            CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${INSTALL_ROOT}/Eigen3

            BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config ${CMAKE_BUILD_TYPE} --target install)
endif ()

# INSTALL GLAD
ExternalProject_Add(
        glad
        PREFIX glad

        GIT_REPOSITORY https://github.com/Dav1dde/glad
        GIT_TAG v0.1.34

        BUILD_ALWAYS OFF
        INSTALL_DIR ${INSTALL_ROOT}/glad
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${INSTALL_ROOT}/glad
        -DGLAD_INSTALL=ON


        BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config ${CMAKE_BUILD_TYPE} --target install)


# INSTALL GLFW
ExternalProject_Add(
        glfw
        PREFIX glfw

        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.3.3

        BUILD_ALWAYS OFF
        INSTALL_DIR ${INSTALL_ROOT}/glfw
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${INSTALL_ROOT}/glfw

        BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config ${CMAKE_BUILD_TYPE} --target install)
