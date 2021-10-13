set(CPP_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
set(H_PREFIX ${VIZ3D_INCLUDE_DIR}/viz3d)

VIZ3D_ADD_SRC(
        ${CPP_PREFIX}/engine.cpp  ${H_PREFIX}/engine.h
        ${CPP_PREFIX}/model.cpp ${H_PREFIX}/model.h
        ${CPP_PREFIX}/camera.cpp ${H_PREFIX}/camera.h
        ${CPP_PREFIX}/opengl_utils.cpp ${H_PREFIX}/opengl_utils.h
        ${CPP_PREFIX}/shader.cpp ${H_PREFIX}/shader.h
        ${H_PREFIX}/stb_image.h ${H_PREFIX}/stb_image_write.h
        ${CPP_PREFIX}/image.cpp ${H_PREFIX}/image.h
        ${H_PREFIX}/types.h)



