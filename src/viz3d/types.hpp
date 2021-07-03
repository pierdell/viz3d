#ifndef OPENGL_LIDAR_SFM_TYPES_HPP
#define OPENGL_LIDAR_SFM_TYPES_HPP

#include <vector>

#include <Eigen/Dense>
#include <Eigen/StdVector>

namespace viz {

    template<int DIM, typename Scalar = float>
    using glMatrix = Eigen::Matrix<Scalar, DIM, DIM, Eigen::ColMajor>;
    typedef glMatrix<4> glMatrix4f;
    typedef glMatrix<3> glMatrix3f;

    typedef std::vector<glMatrix4f, EIGEN_ALIGNED_ALLOCATOR<glMatrix4f>> ArrayM4f;
    typedef std::vector<Eigen::Vector3f, EIGEN_ALIGNED_ALLOCATOR<Eigen::Vector3f>> ArrayV3f;

    // TODO Perform conversions from Matrix4d to Matrix4f and other relevant

    // Returns a Map to a std::vector of Eigen Vectors
    template<typename Scalar, int DIM, int Options_ = Eigen::ColMajor>
    Eigen::Map<Eigen::Matrix<Scalar, -1, DIM, Options_>>
    EigenMap(std::vector<Eigen::Matrix<Scalar, DIM, 1>,
            EIGEN_ALIGNED_ALLOCATOR<Eigen::Matrix<Scalar, DIM, 1>>> &rows) {
        CHECK(!rows.empty());
        return Eigen::Map<Eigen::Matrix<Scalar, -1, DIM, Options_>>(&(rows[0][0]), rows.size(), DIM);
    };

} // namespace viz

#endif //OPENGL_LIDAR_SFM_TYPES_HPP
