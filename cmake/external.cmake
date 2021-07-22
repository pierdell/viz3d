# External projects
include(FetchContent)


# Eigen
FetchContent_Declare(
        Eigen3
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen
        GIT_TAG 3.3.7
)
FetchContent_GetProperties(eigen3)
if (NOT eigen3_POPULATED)
    set(BUILD_TESTING OFF)
    FetchContent_Populate(eigen3)
    set(BUILD_TESTING OFF)
    add_subdirectory(${eigen3_SOURCE_DIR} ${eigen3_BINARY_DIR})
endif ()

# Google Test For Testing
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.8.0)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# A Color Map
FetchContent_Declare(
        colormap
        GIT_REPOSITORY https://github.com/jgreitemann/colormap)
if (NOT colormap_POPULATED)
    FetchContent_Populate(colormap)
    add_subdirectory(${colormap_SOURCE_DIR} ${colormap_BINARY_DIR})
endif ()

# /////////////////////////////////////////////////////////////////////////////////////////////
# OPENGL

# TODO (Set the following as optional)

# GLAD : The OpenGL Loading Library
FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad
        GIT_TAG v0.1.34)
if (NOT glad_POPULATED)
    FetchContent_Populate(glad)
    add_subdirectory(${glad_SOURCE_DIR} ${glad_BINARY_DIR})
endif ()

# OpenGL
find_package(OpenGL REQUIRED)
if (NOT TARGET OpenGL::GL)
    message(FATAL_ERROR "GL could not be found")
endif ()


# GLFW : Windowing System
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.3.3)
if (NOT glfw_POPULATED)
    FetchContent_Populate(glfw)
    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif ()

FetchContent_Declare(
        imgui
        GIT_REPOSITORY git@gitlab.com:pdell/imgui.git
        GIT_TAG docking)

if (NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
    set(_IMGUI_SOURCE_DIR ${imgui_SOURCE_DIR})
    set(FONTS_DIR ${_IMGUI_SOURCE_DIR}/misc/fonts)

    ##################################################################################################################
    # Project Files
    ##################################################################################################################
    set(HEADERS_CXX_FILES
            ${_IMGUI_SOURCE_DIR}/imgui.h
            ${_IMGUI_SOURCE_DIR}/imconfig.h
            ${_IMGUI_SOURCE_DIR}/imgui_internal.h
            ${_IMGUI_SOURCE_DIR}/imstb_rectpack.h
            ${_IMGUI_SOURCE_DIR}/imstb_textedit.h
            ${_IMGUI_SOURCE_DIR}/imstb_truetype.h)

    set(SOURCES_CXX_FILES
            ${_IMGUI_SOURCE_DIR}/imgui.cpp
            ${_IMGUI_SOURCE_DIR}/imgui_draw.cpp
            ${_IMGUI_SOURCE_DIR}/imgui_widgets.cpp
            ${_IMGUI_SOURCE_DIR}/imgui_tables.cpp
            ${_IMGUI_SOURCE_DIR}/imgui_demo.cpp)

    file(GLOB FONTS_FILES ${FONTS_DIR}/*.ttf)

    set(HEADERS_CXX_IMPL_FILES
            ${_IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.h
            ${_IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.h
            )
    set(SOURCES_CXX_IMPL_FILES
            ${_IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
            ${_IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.cpp)


    ##################################################################################################################
    # Target
    ##################################################################################################################
    add_library(imgui
            ${HEADERS_CXX_FILES}
            ${SOURCES_CXX_FILES}
            ${HEADERS_CXX_IMPL_FILES}
            ${SOURCES_CXX_IMPL_FILES}
            ${FONTS_FILES}
            )
    target_include_directories(imgui PUBLIC
            ${_IMGUI_SOURCE_DIR}
            ${_IMGUI_SOURCE_DIR}/backends
            )
    target_link_libraries(imgui PUBLIC OpenGL::GL glfw glad)
    target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
endif ()