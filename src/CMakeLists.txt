macro(VIZ3D_ADD_SRC)
    foreach (_ARG ${ARGN})
        set(VIZ3D_SRC ${VIZ3D_SRC} ${_ARG})
    endforeach ()
    set(VIZ3D_SRC ${VIZ3D_SRC} PARENT_SCOPE)
endmacro(VIZ3D_ADD_SRC)

# Build VIZ3D static Library
set(VIZ3D_SRC "")
set(VIZ3D_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)
get_filename_component(VIZ3D_INCLUDE_DIR ${VIZ3D_INCLUDE_DIR} ABSOLUTE)

add_subdirectory(viz3d)
add_library(viz3d SHARED ${VIZ3D_SRC})
target_link_libraries(viz3d PUBLIC OpenGL::GL glfw glad::glad glog::glog Eigen3::Eigen imgui ${VTK_LIBRARIES})
if (NOT WIN32)
    target_link_libraries(viz3d PUBLIC ${CMAKE_DL_LIBS})
endif ()


set(VIZ3D_INSTALL_DIR ${INSTALL_ROOT}/viz3d)
set(CMAKE_INSTALL_PREFIX ${VIZ3D_INSTALL_DIR})

target_include_directories(viz3d PUBLIC
        "$<BUILD_INTERFACE:${VIZ3D_INCLUDE_DIR}>"
        "$<INSTALL_INTERFACE:include>")

add_subdirectory(example)
install(FILES
        ${implot_SOURCE_DIR}/implot.h
        ${implot_SOURCE_DIR}/implot_internal.h
        ${imgui_SOURCE_DIR}/imconfig.h
        ${imgui_SOURCE_DIR}/imgui.h
        ${imgui_SOURCE_DIR}/imgui_internal.h
        ${imgui_SOURCE_DIR}/imstb_textedit.h
        ${imgui_SOURCE_DIR}/imstb_truetype.h
        DESTINATION ${VIZ3D_INSTALL_DIR}/include)

install(FILES
        ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h
        DESTINATION ${VIZ3D_INSTALL_DIR}/include/misc/cpp)

if (NOT TARGET glfw)
    message(FATAL_ERROR "Could not find target glfw")
endif ()

include(CMakePackageConfigHelpers)

set(INCLUDE_INSTALL_DIR include/ ... )
set(LIB_INSTALL_DIR lib/ ... )

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/project_config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/viz3dConfig.cmake
        INSTALL_DESTINATION ${VIZ3D_INSTALL_DIR}/lib/cmake)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/viz3dConfigVersion.cmake
        VERSION ${CMAKE_PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

install(TARGETS viz3d imgui DESTINATION ${INSTALL_ROOT}/viz3d/lib EXPORT viz3dTargets)
install(EXPORT viz3dTargets DESTINATION "${VIZ3D_INSTALL_DIR}/lib/cmake")
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/viz3dConfigVersion.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/viz3dConfig.cmake
        DESTINATION ${VIZ3D_INSTALL_DIR}/lib/cmake)
install(DIRECTORY ${VIZ3D_INCLUDE_DIR} DESTINATION ${INSTALL_ROOT}/viz3d)

# Setup the config file
configure_file(../cmake/project_config.cmake.in
        ${INSTALL_ROOT}/viz3d/lib/cmake/viz3dConfig.cmake @ONLY)