set(LOG_PREFIX " [VIZ3D] -- ")
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

if (NOT INSTALL_ROOT)
    if (INSTALL_DIR)
        set(INSTALL_ROOT ${INSTALL_DIR})
    else ()
        set(INSTALL_ROOT ${CMAKE_BINARY_DIR}/external/install/${CMAKE_BUILD_TYPE})
    endif ()

    message(INFO "${LOG_PREFIX}Setting the external installation root directory to ${INSTALL_ROOT}")
endif ()