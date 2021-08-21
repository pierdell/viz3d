# Install script for directory: /home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/pdell/dev/viz3d/install/Release/Eigen3")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE FILE FILES
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/AdolcForward"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/AlignedVector3"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/ArpackSupport"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/AutoDiff"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/BVH"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/EulerAngles"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/FFT"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/IterativeSolvers"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/KroneckerProduct"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/LevenbergMarquardt"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/MatrixFunctions"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/MoreVectorization"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/MPRealSupport"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/NonLinearOptimization"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/NumericalDiff"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/OpenGLSupport"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/Polynomials"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/Skyline"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/SparseExtra"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/SpecialFunctions"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/Splines"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE DIRECTORY FILES "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/unsupported/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/pdell/dev/viz3d/Eigen3/src/Eigen3-build/unsupported/Eigen/CXX11/cmake_install.cmake")

endif()

