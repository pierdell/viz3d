macro(VIZ3D_ADD_SRC)
    foreach (_ARG ${ARGN})
        set(VIZ3D_SRC ${VIZ3D_SRC} ${CMAKE_CURRENT_SOURCE_DIR}/${_ARG})
    endforeach ()
    set(VIZ3D_SRC ${VIZ3D_SRC} PARENT_SCOPE)
endmacro(VIZ3D_ADD_SRC)

# Build VIZ3D static Library
set(VIZ3D_SRC "")
add_subdirectory(viz3d)
add_library(viz3d ${VIZ3D_SRC})
target_link_libraries(viz3d PUBLIC OpenGL::GL glfw glad glog eigen imgui)
target_include_directories(viz3d PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(viz3d PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/viz3d)


# Build Test Executables
add_executable(viz3d_main main_opengl.cpp)
target_link_libraries(viz3d_main PUBLIC viz3d)



