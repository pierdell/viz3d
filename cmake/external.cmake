# External projects
include(FetchContent)

# --  Find GLOG
if (NOT GLOG_DIR)
    set(GLOG_DIR ${INSTALL_ROOT}/glog)
endif ()
find_package(glog REQUIRED)
message(INFO "${LOG_PREFIX}Successfully Found GLOG")

# --  Find Eigen
if (NOT EIGEN_DIR)
    set(EIGEN_DIR ${INSTALL_ROOT}/Eigen3)
endif ()
find_package(Eigen3 REQUIRED)
if (NOT TARGET Eigen3::Eigen)
    message(FATAL_ERROR "${LOG_PREFIX}Could not find target Eigen3::Eigen")
endif ()
message(INFO "${LOG_PREFIX}Successfully Found Eigen3")

# -- Color Map
FetchContent_Declare(
        colormap
        GIT_REPOSITORY https://github.com/jgreitemann/colormap)
if (NOT colormap_POPULATED)
    FetchContent_Populate(colormap)
    # Include the directories of colormap
    include_directories(${colormap_SOURCE_DIR}/include)
endif ()

# -- GLAD
if (NOT GLAD_DIR)
    set(GLAD_DIR ${INSTALL_ROOT}/glad/lib/cmake/glad)
    message(INFO " [VIZ3D] -- Setting the glad cmake directory to: ${GLAD_DIR}")
endif ()
find_package(glad REQUIRED CONFIG PATHS ${GLAD_DIR} NO_DEFAULT_PATH)
if (NOT TARGET glad::glad)
    message(FATAL_ERROR "${LOG_PREFIX}Could not load target glad")
endif ()
message(INFO "${LOG_PREFIX}Successfully Found Glad")

# -- OpenGL
find_package(OpenGL REQUIRED)
if (NOT TARGET OpenGL::GL)
    message(FATAL_ERROR "${LOG_PREFIX}OpenGL::GL target could not be found")
endif ()

# -- GLFW : Windowing System
if (NOT GLFW_DIR)
    set(GLFW_DIR ${INSTALL_ROOT}/glfw/lib/cmake/glfw3)
endif ()
find_package(glfw3 REQUIRED CONFIG PATHS ${GLFW_DIR})
if (NOT TARGET glfw)
    message(FATAL_ERROR "${LOG_PREFIX}Target glfw could not be found")
endif ()
message(INFO "${LOG_PREFIX}Successfully Found GLFW")

# Fetch implot dependency (at build time), to integrate the source with imgui in the generated dependency
FetchContent_Declare(
        implot
        GIT_REPOSITORY https://github.com/epezent/implot
        GIT_TAG b47c8ba)

if (NOT implot_POPULATED)
    FetchContent_Populate(implot)
    set(IMPLOT_HDRS
            ${implot_SOURCE_DIR}/implot.h
            ${implot_SOURCE_DIR}/implot_internal.h)
    set(IMPLOT_SRCS
            ${implot_SOURCE_DIR}/implot.cpp
            ${implot_SOURCE_DIR}/implot_demo.cpp
            ${implot_SOURCE_DIR}/implot_items.cpp)
endif ()

# --  Fetch IMGUI dependency (at build time), to compensate for the lack CMakeLists.txt in the project
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/pierdell/imgui
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
            ${_IMGUI_SOURCE_DIR}/imstb_truetype.h
            ${_IMGUI_SOURCE_DIR}/misc/cpp/imgui_stdlib.h
            ${IMPLOT_HDRS})

    set(SOURCES_CXX_FILES
            ${_IMGUI_SOURCE_DIR}/imgui.cpp
            ${_IMGUI_SOURCE_DIR}/imgui_draw.cpp
            ${_IMGUI_SOURCE_DIR}/imgui_widgets.cpp
            ${_IMGUI_SOURCE_DIR}/imgui_tables.cpp
            ${_IMGUI_SOURCE_DIR}/imgui_demo.cpp
            ${_IMGUI_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
            ${IMPLOT_SRCS})

    file(GLOB FONTS_FILES ${FONTS_DIR}/*.ttf)
    set(HEADERS_CXX_IMPL_FILES
            ${_IMGUI_SOURCE_DIR}/backends/imgui_impl_opengl3.h
            ${_IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.h)
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
            ${FONTS_FILES})
    target_include_directories(imgui PUBLIC
            "$<BUILD_INTERFACE:${implot_SOURCE_DIR}>"
            "$<BUILD_INTERFACE:${_IMGUI_SOURCE_DIR}>"
            "$<BUILD_INTERFACE:${_IMGUI_SOURCE_DIR}/backends>"
            "$<INSTALL_INTERFACE:include>")

    target_link_libraries(imgui PUBLIC OpenGL::GL glfw glad::glad)
    target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)
endif ()


# --  VTK
set(VTK_REQUIRED_COMPONENTS
        CommonColor
        CommonComputationalGeometry
        CommonCore
        CommonDataModel
        CommonSystem
        CommonTransforms
        FiltersCore
        FiltersGeneral
        FiltersGeometry
        FiltersModeling
        FiltersSources
        IOGeometry
        IOImage
        IOLegacy
        IOPLY
        IOParallel
        IOXML
        ImagingCore
        ImagingHybrid
        ImagingSources
        InteractionImage
        InteractionStyle
        InteractionWidgets
        RenderingAnnotation
        RenderingContextOpenGL2
        RenderingCore
        RenderingFreeType
        RenderingGL2PSOpenGL2
        RenderingOpenGL2
        OPTIONAL_COMPONENTS
        TestingRendering)

if (BUILD_VTK)
    if (NOT VTK_CMAKE_PATH)
        set(VTK_CMAKE_PATH ${INSTALL_ROOT}/vtk/lib/cmake/vtk-9.1)
        set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:${INSTALL_ROOT}/vtk/lib")
    endif ()
    set(VTK_LIBRARIES "")
    find_package(VTK 9.1 REQUIRED CONFIG COMPONENTS ${VTK_REQUIRED_COMPONENTS} PATHS ${VTK_CMAKE_PATH} NO_DEFAULT_PATH)
else ()
    set(VTK_LIBRARIES "")
    find_package(VTK 9.1 REQUIRED COMPONENTS ${VTK_REQUIRED_COMPONENTS})
    get_property(VTK_COMMON_CORE_LOCATION TARGET VTK::CommonCore PROPERTY LOCATION)
    message (STATUS " [VIZ3D] VTK::CommonCore target found at location : ${VTK_COMMON_CORE_LOCATION}")
endif ()