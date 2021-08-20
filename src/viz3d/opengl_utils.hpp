#ifndef LIDAR_SFM_OPENGL_UTILS_HPP
#define LIDAR_SFM_OPENGL_UTILS_HPP

#include <Eigen/Dense>

#if defined(_WIN32) || defined(WIN32) || defined(__WIN32__)
#define _USE_MATH_DEFINES
#include <math.h>
#endif

namespace viz {


    // Converts degrees to radians
    inline double rad(double degrees) { return degrees * M_PI / 180.0; };

    // Returns a Perspective Projection matrix which computes 3D coordinates in the clip space
    Eigen::Matrix4d perspective_matrix(double fov, double z_near, double z_far, double aspect_ratio = 1.0);

    // Returns whether the current thread is the Graphics Thread
    bool IsGraphicThread();


}

#endif //LIDAR_SFM_OPENGL_UTILS_HPP
