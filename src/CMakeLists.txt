macro(VIZ3D_ADD_SRC)
    foreach (_ARG ${ARGN})
        set(VIZ3D_SRC ${VIZ3D_SRC} ${_ARG})
    endforeach ()
    set(VIZ3D_SRC ${VIZ3D_SRC} PARENT_SCOPE)
endmacro(VIZ3D_ADD_SRC)

# Build VIZ3D static Library
set(VIZ3D_SRC "")
set(VIZ3D_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)
add_subdirectory(viz3d)
add_library(viz3d ${VIZ3D_SRC})
target_link_libraries(viz3d PUBLIC OpenGL::GL glfw glad::glad glog::glog Eigen3::Eigen imgui)
if(NOT WIN32)
   target_link_libraries(viz3d PUBLIC ${CMAKE_DL_LIBS})
endif()
target_include_directories(viz3d PUBLIC ${VIZ3D_INCLUDE_DIR})
target_include_directories(viz3d PRIVATE ${VIZ3D_INCLUDE_DIR})

add_subdirectory(example)

install(TARGETS imgui DESTINATION ${INSTALL_ROOT}/imgui/lib)
install(FILES
        ${imgui_SOURCE_DIR}/imconfig.h
        ${imgui_SOURCE_DIR}/imgui.h
        ${imgui_SOURCE_DIR}/imgui_internal.h
        ${imgui_SOURCE_DIR}/imstb_textedit.h
        ${imgui_SOURCE_DIR}/imstb_truetype.h
        DESTINATION ${INSTALL_ROOT}/imgui/include)

install(TARGETS viz3d DESTINATION ${INSTALL_ROOT}/viz3d/lib)
install(DIRECTORY ${VIZ3D_INCLUDE_DIR} DESTINATION ${INSTALL_ROOT}/viz3d)
# Setup the config file
configure_file(../cmake/project_config.cmake.in
               ${INSTALL_ROOT}/viz3d/lib/cmake/viz3dConfig.cmake @ONLY)



