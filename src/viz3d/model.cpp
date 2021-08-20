#include "model.hpp"
#include "opengl_utils.hpp"
#include <array>

namespace viz {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// AModel
    /* -------------------------------------------------------------------------------------------------------------- */

    AModel::ModelData::~ModelData() = default;

    /* -------------------------------------------------------------------------------------------------------------- */
    size_t AModel::NumInstances() const {
        return num_instances_;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    AModel::~AModel() {
        Remove();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void AModel::Remove() {
        if (is_initialized_ && IsGraphicThread()) {
            glDeleteVertexArrays(1, &GL_VAO_);
            glDeleteBuffers(1, &GL_INSTANCE_POSE_BO_);
            model_data_ = nullptr;
            is_initialized_ = false;
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void AModel::InitializesBuffers() {
        is_initialized_ = true;
        glGenVertexArrays(1, &GL_VAO_);
        glGenBuffers(1, &GL_INSTANCE_POSE_BO_);
    }


    /* -------------------------------------------------------------------------------------------------------------- */
    void AModel::SetModelData(AModel::ModelDataPtr model_data) {
        model_data_ = model_data;
        VerifyModelData();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void AModel::UpdateModel() {
        VerifyModelData();
        CHECK(is_initialized_) << "The buffers are not initialized" << std::endl;
        CHECK(model_data_ != nullptr) << "The Model Data is undefined" << std::endl;
        glBindVertexArray(GL_VAO_);

        // Send instance object poses to the GPU
        glBindBuffer(GL_ARRAY_BUFFER, GL_INSTANCE_POSE_BO_);
        glBufferData(GL_ARRAY_BUFFER, model_data_->SizeOfInstancePoses(),
                     &(model_data_->instance_model_to_world[0](0, 0)),
                     GL_STATIC_DRAW);

        // Link the buffer to the location in the Shader Program
        int size_vec4 = sizeof(float) * 4;
        for (int i(0); i < 4; ++i) {
            // For 4x4 matrices each of the 4 column vectors must be defined at a separate location of the program shader
            int location = InstancePoseLocation() + i;
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location,
                                  4,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  4 * size_vec4,
                                  (void *) (size_vec4 * i));
            glVertexAttribDivisor(location, 1);
        }
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        num_instances_ = model_data_->instance_model_to_world.size();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void AModel::VerifyModelData() {
        if (model_data_->instance_model_to_world.empty()) {
            model_data_->instance_model_to_world.push_back(glMatrix4f::Identity());
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    unsigned int AModel::VAO() const {
        CHECK(is_initialized_) << "The Model is not initialized. Cannot request its VAO" << std::endl;
        return GL_VAO_;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    bool AModel::IsInitialized() const {
        return is_initialized_;
    }

    /* -------------------------------------------------------------------------------------------------------------- */

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// PointCloudModel
    /* -------------------------------------------------------------------------------------------------------------- */
    void PointCloudModel::InitializesBuffers() {
        AModel::InitializesBuffers();
        glGenBuffers(1, &GL_XYZ_BO_);
        glGenBuffers(1, &GL_RGB_BO_);
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void PointCloudModel::InitModel() {
        InitializesBuffers();
        UpdateModel();
        is_initialized_ = true;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void PointCloudModel::VerifyModelData() {
        AModel::VerifyModelData();
        auto model_data = std::dynamic_pointer_cast<PointCloudModelData>(model_data_);
        CHECK(model_data != nullptr) << "The ModelData Cannot be converted to a PointCloudModelData" << std::endl;
        if (model_data->rgb.size() == 0) {
            model_data->rgb = ArrayV3f(model_data->xyz.size(), model_data->default_color);
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void PointCloudModel::UpdateModel() {
        AModel::UpdateModel();
        auto model_data = *std::dynamic_pointer_cast<PointCloudModelData>(model_data_);
        CHECK(model_data.xyz.size() == model_data.rgb.size())
        << "Error rgb and xyz have different sizes" << std::endl;
        glBindVertexArray(GL_VAO_);

        // Send XYZ Data to GPU And Bind the buffer to the VAO
        glBindBuffer(GL_ARRAY_BUFFER, GL_XYZ_BO_);
        glBufferData(GL_ARRAY_BUFFER, model_data.SizeOfXYZ(),
                     &model_data.xyz[0](0), model_data.gl_storage_mode);

        glVertexAttribPointer(PointsLocation(), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(PointsLocation());

        // Send RGB Data to GPU And Bind the buffer to the VAO
        glBindBuffer(GL_ARRAY_BUFFER, GL_RGB_BO_);
        glBufferData(GL_ARRAY_BUFFER, model_data.SizeOfRGB(),
                     &model_data.rgb[0](0), model_data.gl_storage_mode);

        glVertexAttribPointer(ColorLocation(), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(ColorLocation());
        last_num_points_ = model_data.xyz.size();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void PointCloudModel::DrawSelf() {
        glBindVertexArray(GL_VAO_);
        auto &model = ModelData();
        glPointSize(model.point_size);
        glDrawArraysInstanced(GL_POINTS, 0, model.NumPoints(), model.NumInstances());
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    PointCloudModel::PointCloudModelData &PointCloudModel::ModelData() {
        auto model_ptr = std::dynamic_pointer_cast<PointCloudModelData>(model_data_);
        CHECK(model_ptr != nullptr) << "Error Model could not be cast as a PointCloudModelData" << std::endl;
        return *model_ptr;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void PointCloudModel::Remove() {
        if (is_initialized_ && IsGraphicThread()) {
            glDeleteBuffers(1, &GL_XYZ_BO_);
            glDeleteBuffers(1, &GL_RGB_BO_);
            AModel::Remove();
            is_initialized_ = false;
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    PointCloudModel::~PointCloudModel() {
        PointCloudModel::Remove();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    PointCloudModel::PointCloudModel() {
        PointCloudModel::SetModelData(std::make_shared<PointCloudModelData>());
    }

    /* -------------------------------------------------------------------------------------------------------------- */

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// ScreenModel

    namespace {
        const float xyuv_data[] = {
                // First screen triangle
                -1.0, 1.0, 0.0, 1.0,
                -1.0, -1.0, 0.0, 0.0,
                1.0, -1.0, 1.0, 0.0,

                // Second screen triangle
                -1.0, 1.0, 0.0, 1.0,
                1.0, -1.0, 1.0, 0.0,
                1.0, 1.0, 1.0, 1.0
        };
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void ScreenModel::InitializesBuffers() {
        glGenVertexArrays(1, &GL_VAO_);
        glGenBuffers(1, &GL_XYUV_BO_);
        UpdateModel();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void ScreenModel::InitModel() {
        InitializesBuffers();
        is_initialized_ = true;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void ScreenModel::VerifyModelData() {
        CHECK(model_data_ == nullptr) << "No ModelData should be defined for a ScreenModel";
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void ScreenModel::UpdateModel() {

        glBindVertexArray(GL_VAO_);
        glBindBuffer(GL_ARRAY_BUFFER, GL_XYUV_BO_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(xyuv_data), xyuv_data, GL_STATIC_DRAW);
        glVertexAttribPointer(PointsLocation(), 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
        glVertexAttribPointer(TexCoordsLocation(), 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                              (void *) (2 * sizeof(float)));
        glEnableVertexAttribArray(PointsLocation());
        glEnableVertexAttribArray(TexCoordsLocation());
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void ScreenModel::Remove() {
        if (is_initialized_ && IsGraphicThread()) {
            glDeleteBuffers(1, &GL_XYUV_BO_);
            glDeleteVertexArrays(1, &GL_VAO_);
            model_data_ = nullptr;
            is_initialized_ = false;
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void ScreenModel::DrawSelf() {
        glBindVertexArray(GL_VAO_);
        glDisable(GL_DEPTH_TEST);
        glDrawArrays(GL_TRIANGLES, 0, NumPoints());
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    /// PosesModel

    namespace {
        const float xyzrgb_data[] = {
                // X Axis
                0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
                1.0, 0.0, 0.0, 1.0, 0.0, 0.0,

                // Y Axis
                0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
                0.0, 1.0, 0.0, 0.0, 1.0, 0.0,

                // Z Axis
                0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
                0.0, 0.0, 1.0, 0.0, 0.63, 1.0,
        };
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    PosesModel::PosesModelData &PosesModel::ModelData() {
        auto ptr = std::dynamic_pointer_cast<PosesModelData>(model_data_);
        CHECK(ptr != nullptr) << "Could not convert Model Data to PosesModel";
        return *ptr;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void PosesModel::InitializesBuffers() {
        AModel::InitializesBuffers();
        glGenBuffers(1, &GL_XYZRGB_BO_);
        UpdateModel();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void PosesModel::InitModel() {
        InitializesBuffers();
        is_initialized_ = true;

    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void PosesModel::VerifyModelData() {
        AModel::VerifyModelData();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void PosesModel::Remove() {
        AModel::Remove();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void PosesModel::UpdateModel() {
        AModel::UpdateModel();

        const int size_model = sizeof(xyzrgb_data) / sizeof(float);
        std::array<float, size_model> _scaled_xyz_rgb;
        std::copy(xyzrgb_data, xyzrgb_data + size_model, _scaled_xyz_rgb.begin());

        auto &model_data = ModelData();
        auto scaling = model_data.scaling;
        auto with_default_color = model_data.with_default_color;
        for (int i(0); i < 6; ++i) {
            for (int j(0); j < 3; ++j) {
                _scaled_xyz_rgb[6 * i + j] *= ModelData().scaling;
                if (with_default_color)
                    _scaled_xyz_rgb[6 * i + 3 + j] = model_data.default_color[j];
            }

        }

        glBindVertexArray(GL_VAO_);
        glBindBuffer(GL_ARRAY_BUFFER, GL_XYZRGB_BO_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(xyzrgb_data), &_scaled_xyz_rgb[0], GL_STATIC_DRAW);
        glVertexAttribPointer(PointsLocation(), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
        glVertexAttribPointer(ColorLocation(), 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                              (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(PointsLocation());
        glEnableVertexAttribArray(ColorLocation());
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void PosesModel::DrawSelf() {
        glBindVertexArray(GL_VAO_);
        auto &model = ModelData();
        glLineWidth(model.line_size);
        glDrawArraysInstanced(GL_LINES, 0, NumPoints(), model.NumInstances());
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    PosesModel::PosesModel() {
        PosesModel::SetModelData(std::make_shared<PosesModelData>());
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    PosesModel::~PosesModel() noexcept {
        Remove();
    }
    /* -------------------------------------------------------------------------------------------------------------- */


}
