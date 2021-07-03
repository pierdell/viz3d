#include "camera.hpp"

namespace viz {
    /* -------------------------------------------------------------------------------------------------------------- */
    /// ACamera
    ACamera::~ACamera() = default;

    /* -------------------------------------------------------------------------------------------------------------- */
    void viz::FPVCamera::UpdatePhysics(double ticks_secs) {
        // Update the location of the camera
        auto delta_tr = ticks_secs * translation_speed_;
        location_ -= delta_tr * (orientation_ * direction_array_);

        // Updates the orientation of the camera
        auto delta_rot = ticks_secs * angular_speed;

        auto delta_theta = Eigen::AngleAxisd(rad(delta_rot * rotation_array_[0]), Eigen::Vector3d::UnitY());
        auto delta_phi = Eigen::AngleAxisd(rad(delta_rot * rotation_array_[1]), Eigen::Vector3d::UnitX());
        auto delta_psi = Eigen::AngleAxisd(rad(delta_rot * rotation_array_[2]), Eigen::Vector3d::UnitZ());

        Eigen::Quaterniond orientation(orientation_);
        orientation = orientation * delta_theta * delta_phi * delta_psi;
        orientation.normalize();
        orientation_ = orientation.toRotationMatrix();
    }

    /* -------------------------------------------------------------------------------------------------------------- */


#define SWITCH_ACTION_CASE_TRANSLATION(event_key, action_vector, index, value) \
case event_key:                                                                \
switch(action) {                                                               \
    case (ACamera::KEY_DOWN):                                                  \
    case (ACamera::REPEAT):                                                    \
          action_vector[index] = value;                                        \
          break;                                                               \
    case (ACamera::KEY_UP):                                                    \
          if (action_vector[index] == value)                                   \
            (action_vector)[index] = 0.0;                                        \
          break;                                                               \
};                                                                             \
break;

    void FPVCamera::ProcessEvent(ACamera::EVENT_KEY key, ACamera::EVENT_ACTION action) {
        if (key == SWITCH_MODE_0 && action == KEY_DOWN)
            return SwitchMode(FPV_MODE);
        if (state_ == NO_MODE)
            return;

        switch (key) {
            SWITCH_ACTION_CASE_TRANSLATION(FRONT, direction_array_, 2, 1.0)
            SWITCH_ACTION_CASE_TRANSLATION(BACK, direction_array_, 2, -1.0)
            SWITCH_ACTION_CASE_TRANSLATION(RIGHT, direction_array_, 0, -1.0)
            SWITCH_ACTION_CASE_TRANSLATION(LEFT, direction_array_, 0, 1.0)
            SWITCH_ACTION_CASE_TRANSLATION(R_RIGHT, rotation_array_, 2, -0.5)
            SWITCH_ACTION_CASE_TRANSLATION(R_LEFT, rotation_array_, 2, 0.5)
            SWITCH_ACTION_CASE_TRANSLATION(UP, direction_array_, 1, -1)
            SWITCH_ACTION_CASE_TRANSLATION(DOWN, direction_array_, 1, 1.0)

            case ACCELERATE:
                if (action == KEY_DOWN) {
                    angular_speed *= 2.0;
                    translation_speed_ *= 10.0;
                }
                if (action == KEY_UP) {
                    angular_speed /= 2.0;
                    translation_speed_ /= 10.0;
                }
                break;

            default:
                break;
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */

    void FPVCamera::CursorPosition(double x_pos, double y_pos) {
        // TODO
        double x_range = x_pos - width_ / 2;
        auto radius_ratio = radius_orientation_ratio;
        auto diff_x = radius_ratio * width_;

        double y_range = y_pos - height_ / 2;
        auto diff_y = radius_ratio * height_;

        if (state_ == FPV_MODE) {
            if (std::abs(x_range) > diff_x) {
                auto value =
                        (x_range < 0 ? 1.0 : -1.0) * (std::abs(x_range) - diff_x) / ((double) width_ / 2.0 - diff_x);
                rotation_array_[0] = value;
            } else
                rotation_array_[0] = 0.0;

            if (std::abs(y_range) > diff_y) {
                auto value =
                        (y_range < 0 ? 1.0 : -1.0) * (std::abs(y_range) - diff_y) / ((double) width_ / 2.0 - diff_y);
                rotation_array_[1] = value;
            } else
                rotation_array_[1] = 0.0;
        }

    }

    /* -------------------------------------------------------------------------------------------------------------- */

};
