# External projects
include(FetchContent)

set(LOG_PREFIX " [VIZ3D] -- ")
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT EXT_INSTALL_ROOT)
	set(EXT_INSTALL_ROOT ${CMAKE_BINARY_DIR}/external/install/${CMAKE_BUILD_TYPE})
	message(INFO "${LOG_PREFIX}Setting the external installation root directory to ${EXT_INSTALL_ROOT}")
endif()

# Find GLOG
if(NOT GLOG_DIR)
	set(GLOG_DIR ${EXT_INSTALL_ROOT}/glog)
endif()
find_package(glog REQUIRED)
message(INFO "${LOG_PREFIX}Successfully Found GLOG")


# Find Eigen
if(NOT EIGEN_DIR)
	set(EIGEN_DIR ${EXT_INSTALL_ROOT}/Eigen3)
endif()
find_package(Eigen3 REQUIRED)
if(NOT TARGET Eigen3::Eigen)
       message(FATAL_ERROR "${LOG_PREFIX}Could not find target Eigen3::Eigen")
endif()
message(INFO "${LOG_PREFIX}Successfully Found Eigen3")

# A Color Map
FetchContent_Declare(
        colormap
        GIT_REPOSITORY https://github.com/jgreitemann/colormap)
if (NOT colormap_POPULATED)
    FetchContent_Populate(colormap)    
    # Include the directories of colormap
    include_directories(${colormap_SOURCE_DIR}/include)
endif ()

# /////////////////////////////////////////////////////////////////////////////////////////////
# OPENGL

# TODO (Set the following as optional)

if (NOT GLAD_DIR)
	set(GLAD_DIR ${EXT_INSTALL_ROOT}/glad/lib/cmake)
endif()
find_package(glad REQUIRED CONFIG PATHS ${GLAD_DIR})
if (NOT TARGET glad::glad)
	message(FATAL_ERROR "${LOG_PREFIX}Could not load target glad")
endif()
message(INFO "${LOG_PREFIX}Successfully Found Glad")

# OpenGL
find_package(OpenGL REQUIRED)
if (NOT TARGET OpenGL::GL)
    message(FATAL_ERROR "${LOG_PREFIX}OpenGL::GL target could not be found")
endif ()


# GLFW : Windowing System
if (NOT GLFW_DIR)
	set(GLFW_DIR ${EXT_INSTALL_ROOT}/glfw/lib/cmake/glfw3)
endif()
find_package(glfw3 REQUIRED CONFIG PATHS ${GLFW_DIR})
if(NOT TARGET glfw)
        message(FATAL_ERROR "${LOG_PREFIX}Target glfw could not be found")
endif()
message(INFO "${LOG_PREFIX}Successfully Found GLFW")


FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://gitlab.com/pdell/imgui
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
    target_link_libraries(imgui PUBLIC OpenGL::GL glfw glad::glad)
    target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
endif ()