#ifndef LIDAR_SFM_SHADER_HPP
#define LIDAR_SFM_SHADER_HPP

#include <memory>
#include <glog/logging.h>
#include <glad/glad.h>
#include <GL/gl.h>
#include "model.hpp"


namespace viz {

    /// Shader Program: A Utility Class to compile Shader Programs
    class ShaderProgram {
    public:

        ShaderProgram(const char *vertex_shader,
                      const char *fragment_shader,
                      std::string &&shader_name) : fragment_shader_(fragment_shader),
                                                   vertex_shader_(vertex_shader),
                                                   shader_name_(std::move(shader_name)) {}

        // Compiles the ShaderProgram
        bool Compile();

        // Returns the GLShaderProgramId
        // Note: Only valid after Compile() has been called
        unsigned int GLShaderProgramId();


    private:
        std::string shader_name_;
        const char *fragment_shader_ = nullptr;
        const char *vertex_shader_ = nullptr;
        unsigned int gl_shader_program_id = -1, gl_vertex_shader_id_ = -1, gl_fragment_shader_id_ = -1;
        bool compiled_ = false;

        static bool CompileShader(const char *shader, int gl_shader_type, unsigned int &gl_shader_id);

        bool CompileProgram();
    };

    /* -------------------------------------------------------------------------------------------------------------- */
    /// AShader: An abstract class which gives access to the GLSL code which is executed on the GPU to render the scene
    class AShader {
    public:
        virtual ~AShader() = 0;

        // Initializes the Shader
        // Note: The shader might not be valid yet, as child classes might require
        //       Additional definitions (such as uniform variables)
        virtual bool Init() {
            CHECK(!initialized_) << "The Shader is already initialized" << std::endl;
            shader_program_ = std::make_unique<ShaderProgram>(VertexShader(), FragmentShader(), ShaderName());
            shader_program_->Compile();
            return true;
        };


        // Returns the uniform location of a program given a uniform variable name
        int GetULocation(const std::string &loc_name) {
            int uloc = glGetUniformLocation(shader_program_->GLShaderProgramId(), loc_name.c_str());
            return uloc;
        }

        /* ---------------------------------------------------------------------------------------------------------- */
        /// ABSTRACT METHODS
        // Returns whether the shader is ready to run on models
        // Typically, shaders need to set variables at uniform locations before running
        // (e.g. The camera pose or the projection matrix).
        virtual bool IsValid() { return false; }

        // Runs the Shader on the model
        virtual bool RunOnModel(AModel &model) = 0;

        // Returns the Shader Name
        virtual std::string ShaderName() const = 0;

        // Returns the Fragment Shader
        virtual const char *FragmentShader() const = 0;

        // Returns the Vertex Shader
        virtual const char *VertexShader() const = 0;

    protected:
        bool initialized_ = false;
        std::unique_ptr<ShaderProgram> shader_program_ = nullptr;
    };

    /* -------------------------------------------------------------------------------------------------------------- */
    /// AlbedoCameraShader: A Basic Shader to render models geometry primitives and colors
    class AlbedoCameraShader : public AShader {
    public:
        ~AlbedoCameraShader() override = default;

        // Sets the Matrix `world_to_cam` as uniform location
        bool SetWorldToCam(const glMatrix4f &world_to_cam) {
            glUseProgram(shader_program_->GLShaderProgramId());
            glUniformMatrix4fv(GetULocation("world_to_cam"), 1, GL_FALSE, &world_to_cam(0, 0));
            world_to_cam_set_ = true;
            return true;
        }

        // Sets the `projection` as uniform location
        bool SetProjectionMatrix(const glMatrix4f &projection) {
            glUseProgram(shader_program_->GLShaderProgramId());
            glUniformMatrix4fv(GetULocation("projection"), 1, GL_FALSE, &projection(0, 0));
            projection_set_ = true;
            return true;
        }

        /* ---------------------------------------------------------------------------------------------------------- */
        /// OVERRIDES
        // The shader is only valid once the "world_to_cam" and "projection" matrices have been set
        bool IsValid() override { return world_to_cam_set_ && projection_set_; }

        // Runs the Shader on the model
        bool RunOnModel(AModel &model) override;

        // Returns the Shader Name
        std::string ShaderName() const override { return "AlbedoCamera"; };

        // Returns the Fragment Shader
        const char *FragmentShader() const override;

        // Returns the Vertex Shader
        const char *VertexShader() const override;

    private:
        bool world_to_cam_set_ = false, projection_set_ = false;
    };


    /* -------------------------------------------------------------------------------------------------------------- */
    // A Screen Shader which implements Eye-Dome-Lighting shading for non realistic rendering of point cloud
    class EDLScreenShader : public AShader {
    public:
        ~EDLScreenShader() override = default;

        // Setup the textures to draw in
        void SetUpTextures(GLint color_texture, GLint depth_texture) {
            gl_color_texture_ = color_texture;
            gl_depth_texture_ = depth_texture;
            textures_set_ = true;
        }

        void SetUpEDL(bool with_edl = true, double edl_strength = 100.0,
                      double edl_distance = 1.0, double z_far = 10000.0, double z_near = 0.1) {
            auto pid = shader_program_->GLShaderProgramId();
            glUseProgram(pid);
            auto a_loc = glGetUniformLocation(pid, "A");
            auto b_loc = glGetUniformLocation(pid, "B");
            auto a = -(z_far + z_near) / (z_far - z_near);
            auto b = -2 * (z_far * z_near) / (z_far - z_near);
            glUniform1f(a_loc, (float) a);
            glUniform1f(b_loc, (float) b);

            auto edl_strength_loc = glGetUniformLocation(pid, "EDL_STRENGTH");
            auto edl_dist_loc = glGetUniformLocation(pid, "EDL_DISTANCE");
            auto with_edl_loc = glGetUniformLocation(pid, "WITH_EDL");

            glUniform1f(edl_strength_loc, (float) edl_strength);
            glUniform1f(edl_dist_loc, (float) edl_distance);
            auto with_edli = (int) (with_edl);
            glUniform1i(with_edl_loc, with_edli);
        }

        /* ---------------------------------------------------------------------------------------------------------- */
        /// OVERRIDES
        bool Init() override {
            AShader::Init();
            SetUpEDL();
            auto pid = shader_program_->GLShaderProgramId();
            glUseProgram(pid);

            // Initialize the texture locations
            auto color_texture_loc = glGetUniformLocation(pid, "colorTexture");
            auto depth_texture_loc = glGetUniformLocation(pid, "depthTexture");
            glUniform1i(color_texture_loc, 0);
            glUniform1i(depth_texture_loc, 1);
            return true;
        }


        // The shader is only valid once the "world_to_cam" and "projection" matrices have been set
        bool IsValid() override { return textures_set_; }

        // Runs the Shader on the model
        bool RunOnModel(AModel &model) override;

        // Returns the Shader Name
        std::string ShaderName() const override { return "EDLScreenShader"; };

        // Returns the Fragment Shader
        const char *FragmentShader() const override;

        // Returns the Vertex Shader
        const char *VertexShader() const override;

    private:
        bool textures_set_ = false;
        GLint gl_color_texture_ = -1, gl_depth_texture_ = -1;
    };

}


#endif //LIDAR_SFM_SHADER_HPP
