#include <viz3d/opengl_utils.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace viz {


    /* -------------------------------------------------------------------------------------------------------------- */
    Eigen::Matrix4d perspective_matrix(double fov, double z_near, double z_far, double aspect_ratio) {
        Eigen::Matrix4d result = Eigen::Matrix4d::Zero();
        double f = 1.0 / tan((rad(fov)) / 2.0);
        result << f, 0.0, 0.0, 0.0,
                0.0, f / aspect_ratio, 0.0, 0.0,
                0.0, 0.0, (z_far + z_near) / (z_near - z_far), 2.0 * z_far * z_near / (z_near - z_far),
                0.0, 0.0, -1.0, 0.0;
        return result;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    bool IsGraphicThread() {
        return glfwGetCurrentContext() != nullptr;
    }
}
