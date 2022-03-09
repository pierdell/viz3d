set(CPP_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
set(H_PREFIX ${VIZ3D_INCLUDE_DIR}/viz3d)

VIZ3D_ADD_SRC(
        ${CPP_PREFIX}/ui.cpp ${H_PREFIX}/ui.h
        ${H_PREFIX}/vtk_window.h ${CPP_PREFIX}/vtk_window.cpp)