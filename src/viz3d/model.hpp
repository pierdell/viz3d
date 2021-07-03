#ifndef LIDAR_SFM_MODEL_H
#define LIDAR_SFM_MODEL_H

#include <memory>
#include <Eigen/Dense>
#include <Eigen/StdVector>
#include <glog/logging.h>

#include "types.hpp"

#include <glad/glad.h>
#include <GL/gl.h>

namespace viz {

    /// An AModel is an abstract class Containing Data which can be drawn in a rendering window
    class AModel {
    public:

        struct ModelData {

            // The Default color of the model data
            Eigen::Vector3f default_color = Eigen::Vector3f(0.0, 0.63, 1.0);

            // Instance Model To World
            ArrayM4f instance_model_to_world{glMatrix4f::Identity()};

            long SizeOfInstancePoses() const {
                return (long) (instance_model_to_world.size() * sizeof(glMatrix4f));
            };

            long NumInstances() const {
                return (long) instance_model_to_world.size();
            }

            virtual ~ModelData() = 0;
        };

        using ModelDataPtr = std::shared_ptr<ModelData>;

        virtual ~AModel() = 0;

        // Returns the Number of Instances of the Model On the GPU
        // (ie the last time UpdateModel was called)
        size_t NumInstances() const;

        unsigned int VAO() const;

        bool IsInitialized() const;

        /* ---------------------------------------------------------------------------------------------------------- */
        /// ABSTRACT METHODS

        // Draws the geometry
        virtual void DrawSelf() = 0;

        // Removes the model from the GPU
        virtual void Remove();

        // Initializes the buffers
        virtual void InitializesBuffers();

        // Sets the Model Data (and check its validity)
        virtual void SetModelData(ModelDataPtr model_data);

        // Initializes the Model
        virtual void InitModel() = 0;

        // Updates the model buffers when its state has changed
        virtual void UpdateModel();

        /* ---------------------------------------------------------------------------------------------------------- */
        /// SHADER PROGRAM LOCATIONS

        // Returns the shader program location of 3D coordinates (xyz) of vertex data
        static int PointsLocation() { return 0; }

        // Returns the shader program location of color (rgb) of vertex data
        static int ColorLocation() { return 1; }

        // Returns the shader program location of instance poses
        static int InstancePoseLocation() { return 3; }

    protected:
        bool is_initialized_ = false;
        std::shared_ptr<ModelData> model_data_ = nullptr;
        unsigned int GL_VAO_, GL_INSTANCE_POSE_BO_;
        size_t num_instances_ = -1;

        // Verifies the validity of the Model Data and
        virtual void VerifyModelData();

    };

    typedef std::shared_ptr<AModel> ModelPtr;


    /* -------------------------------------------------------------------------------------------------------------- */
    class PointCloudModel final : public AModel {
    public:
        ~PointCloudModel() final;

        PointCloudModel();

        struct PointCloudModelData : AModel::ModelData {

            ArrayV3f xyz; // The points of the PointCloud

            ArrayV3f rgb; // The RGB color of the PointCloud

            int point_size = 2; // The OpenGL Point Size

            int gl_storage_mode = GL_STATIC_DRAW;

            long SizeOfXYZ() const { return (long) (xyz.size() * sizeof(Eigen::Vector3f)); }

            long SizeOfRGB() const { return (long) (rgb.size() * sizeof(Eigen::Vector3f)); }

            long NumPoints() const { return xyz.size(); }
        };

        // Returns the Number of Points of the PointCloud allocated on the GPU
        size_t NumPoints() const { return last_num_points_; };

        // Returns a reference to the ModelData
        PointCloudModelData &ModelData();

        /* ---------------------------------------------------------------------------------------------------------- */
        /// OVERRIDES
        void InitializesBuffers() override;

        void InitModel() override;

        void VerifyModelData() override;

        void Remove() override;

        // Updates Model
        void UpdateModel() override;

        void DrawSelf() override;

    private:
        unsigned int GL_XYZ_BO_, GL_RGB_BO_;
        size_t last_num_points_ = -1;
    };


    /* -------------------------------------------------------------------------------------------------------------- */
    class PosesModel final : public AModel {
    public:
        ~PosesModel() final;

        PosesModel();

        struct PosesModelData : AModel::ModelData {

            double scaling = 1.0; // The scaling factor of the model

            int line_size = 2; // The OpenGL Line Size

            int gl_storage_mode = GL_STATIC_DRAW;

            bool with_default_color = false; // Whether to color the lines with the default color

        };

        // Returns the Number of Points of the PointCloud allocated on the GPU
        static size_t NumPoints() { return 6; };

        // Returns a reference to the ModelData
        PosesModelData &ModelData();

        /* ---------------------------------------------------------------------------------------------------------- */
        /// OVERRIDES
        void InitializesBuffers() override;

        void InitModel() override;

        void VerifyModelData() override;

        void Remove() override;

        // Updates Model
        void UpdateModel() override;

        void DrawSelf() override;

    private:
        unsigned int GL_XYZRGB_BO_;
    };


    /* -------------------------------------------------------------------------------------------------------------- */
    // A Simple Screen Model which allows to display a texture to a screen
    class ScreenModel : public AModel {
    public:

        static int NumPoints() { return 6; }

        /* ---------------------------------------------------------------------------------------------------------- */
        /// OVERRIDES

        void Remove() override;

        void InitializesBuffers() override;

        void InitModel() override;

        void VerifyModelData() override;

        void UpdateModel() override;

        void DrawSelf() override;

        /* ---------------------------------------------------------------------------------------------------------- */
        /// PROGRAM LOCATIONS
        static int TexCoordsLocation() { return 2; }

    private:
        unsigned int GL_XYUV_BO_;

    };

}


#endif //LIDAR_SFM_MODEL_H
