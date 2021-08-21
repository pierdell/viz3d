/**
 * TODO:
 *  - Remove hardcoded parameters into options / camera profiles
 */
#ifndef LIDAR_SFM_CAMERA_HPP
#define LIDAR_SFM_CAMERA_HPP

#include "types.hpp"
#include "opengl_utils.hpp"

namespace viz {

    class ACamera {
    public:

        enum EVENT_KEY {
            FRONT = 0,
            BACK = 1,
            LEFT = 2,
            RIGHT = 3,
            R_LEFT = 4,
            R_RIGHT = 5,
            UP = 6,
            DOWN = 7,

            SWITCH_MODE_0 = 8,
            SWITCH_MODE_1 = 9,
            SWITCH_MODE_2 = 10,
            ACCELERATE = 11,

            RESET_RELATIVE_POSE = 12,
            RESET_ABSOLUTE_POSE = 13

        };

        enum EVENT_ACTION {
            KEY_DOWN = 0,
            KEY_UP = 1,
            REPEAT = 2
        };

        glMatrix4f ProjectionMatrix() const {
            glMatrix4f casted = projection_matrix_.cast<float>();
            return casted;
        }

        glMatrix4f WorldToCam() const {
            Eigen::Matrix4d pose = Eigen::Matrix4d::Identity();
            pose.block<3, 3>(0, 0) = relative_orientation_.transpose();
            pose.block<3, 1>(0, 3) = -relative_orientation_.transpose() * relative_location_;
            pose = pose * absolute_pose_;
            glMatrix4f casted = pose.cast<float>();
            return casted;
        }

        void SetAbsolutePose(const Eigen::Matrix4d& mat) {
            absolute_pose_ = mat;
        }

        void ResetInitialPose() {
            absolute_pose_ = Eigen::Matrix4d::Identity();
        }

        virtual ~ACamera() = 0;

        // Updates the physics of the motion of the camera
        virtual void UpdatePhysics(double lag) = 0;

        // Manages the input of the cursor position
        virtual void CursorPosition(double x_pos, double y_pos) = 0;

        // Processes Camera Events
        virtual void ProcessEvent(EVENT_KEY key, EVENT_ACTION action) = 0;

        // Changes the dimensions of the camera
        virtual void ChangeDimensions(int height, int width) {
            height_ = height;
            width_ = width;
            aspect_ratio_ = (double) height_ / width_;

            SetProjectionMatrix();
        }

        Eigen::Vector3d FrontVector() const {
            return relative_orientation_.block<3, 1>(0, 2);
        }

        Eigen::Vector3d RightVector() const {
            return relative_orientation_.block<3, 1>(0, 0);
        }

        Eigen::Vector3d UpVector() const {
            return relative_orientation_.block<3, 1>(0, 1);
        }

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    protected:
        void SetProjectionMatrix() {
            projection_matrix_ = perspective_matrix(fov_degrees_, z_near_, z_far, aspect_ratio_);
        };

        int height_ = 500, width_ = 500;
        // Camera Pose
        Eigen::Matrix3d relative_orientation_ = Eigen::Matrix3d::Identity();
        Eigen::Vector3d relative_location_ = Eigen::Vector3d(0, 0, 10);

        // Projection parameters
        double fov_degrees_ = 45.0;
        double z_near_ = 0.1;
        double z_far = 10000.0;
        double aspect_ratio_ = (double) height_ / width_;
        Eigen::Matrix4d projection_matrix_;
        Eigen::Matrix4d absolute_pose_ = Eigen::Matrix4d::Identity();
    };

    class FPVCamera : public ACamera {
    public:

        ~FPVCamera() override = default;

        void UpdatePhysics(double ticks_secs) override;

        void ProcessEvent(EVENT_KEY key, EVENT_ACTION action) override;

        void CursorPosition(double x_pos, double y_pos) override;


    private:
        enum CAMERA_STATE {
            NO_MODE = 0,
            FPV_MODE = 1,
            MOVE_MODEL_MODE = 2,
            ORIENTATION_MODE = 3
        };

        void SwitchMode(CAMERA_STATE new_state) {
            if (state_ == new_state)
                state_ = NO_MODE;
            else
                state_ = new_state;
            direction_array_ = Eigen::Vector3d::Zero();
            rotation_array_ = Eigen::Vector3d::Zero();
        }


        CAMERA_STATE state_ = NO_MODE;

        // Parameters of the motion of the camera
        double translation_speed_ = 10.0;
        double angular_speed = 160.0;
        double radius_orientation_ratio = 0.25;

        Eigen::Vector3d direction_array_ = Eigen::Vector3d::Zero();
        Eigen::Vector3d rotation_array_ = Eigen::Vector3d::Zero();
    };

}


#endif //LIDAR_SFM_CAMERA_HPP
