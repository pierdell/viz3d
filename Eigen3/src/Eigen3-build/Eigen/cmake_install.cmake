# Install script for directory: /home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/Eigen" TYPE FILE FILES
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/Cholesky"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/CholmodSupport"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/Core"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/Dense"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/Eigen"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/Eigenvalues"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/Geometry"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/Householder"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/IterativeLinearSolvers"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/Jacobi"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/LU"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/MetisSupport"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/OrderingMethods"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/PaStiXSupport"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/PardisoSupport"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/QR"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/QtAlignedMalloc"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/SPQRSupport"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/SVD"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/Sparse"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/SparseCholesky"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/SparseCore"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/SparseLU"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/SparseQR"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/StdDeque"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/StdList"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/StdVector"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/SuperLUSupport"
    "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/UmfPackSupport"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/Eigen" TYPE DIRECTORY FILES "/home/pdell/dev/viz3d/Eigen3/src/Eigen3/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

