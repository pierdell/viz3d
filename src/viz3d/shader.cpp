#include "shader.hpp"

namespace viz {


    /* -------------------------------------------------------------------------------------------------------------- */
    bool ShaderProgram::Compile() {
        CHECK(!compiled_) << "The program was already compiled" << std::endl;
        LOG(INFO) << "Compiling Shader " << shader_name_ << std::endl;
        if (!CompileShader(vertex_shader_, GL_VERTEX_SHADER, gl_vertex_shader_id_))
            return false;
        if (!CompileShader(fragment_shader_, GL_FRAGMENT_SHADER, gl_fragment_shader_id_))
            return false;
        if (!CompileProgram())
            return false;
        glDeleteShader(gl_vertex_shader_id_);
        glDeleteShader(gl_fragment_shader_id_);
        compiled_ = true;
        return true;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    unsigned int ShaderProgram::GLShaderProgramId() {
        CHECK(compiled_) << "The Shader Program Was not Compiled";
        return gl_shader_program_id;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    bool ShaderProgram::CompileShader(const char *shader, int gl_shader_type, unsigned int &gl_shader_id) {
        gl_shader_id = glCreateShader(gl_shader_type);

        int size = strlen(shader);
        glShaderSource(gl_shader_id, 1, &shader, &size);
        glCompileShader(gl_shader_id);
        GLint compile_status;
        glGetShaderiv(gl_shader_id, GL_COMPILE_STATUS, &compile_status);
        if (compile_status == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(gl_shader_id, GL_INFO_LOG_LENGTH, &maxLength);
            std::string error_message(maxLength, ' ');
            glGetShaderInfoLog(gl_shader_id, maxLength, &maxLength, &error_message[0]);
            LOG(ERROR) << "Shader Failed to compile, with the following message \n " << error_message;
            return false;
        }
        return true;
    }


    /* -------------------------------------------------------------------------------------------------------------- */
    bool ShaderProgram::CompileProgram() {
        gl_shader_program_id = glCreateProgram();
        glAttachShader(gl_shader_program_id, gl_vertex_shader_id_);
        glAttachShader(gl_shader_program_id, gl_fragment_shader_id_);

        glUseProgram(gl_shader_program_id);
        // Validate Program
        glLinkProgram(gl_shader_program_id);
        // Check Link Program
        GLint link_status;
        glGetProgramiv(gl_shader_program_id, GL_LINK_STATUS, &link_status);
        if (link_status == GL_FALSE) {
            LOG(ERROR) << "The program could not be linked to Shaders" << std::endl;
            return false;
        }

        glUseProgram(0);
        return true;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    /// AShader
    AShader::~AShader() = default;


    /// AlbedoCameraShader
    /* -------------------------------------------------------------------------------------------------------------- */

    bool AlbedoCameraShader::RunOnModel(AModel &model) {
        CHECK(IsValid()) << "Invalid Shader, cannot draw model" << std::endl;
        glUseProgram(shader_program_->GLShaderProgramId());
        glBindVertexArray(model.VAO());
        model.DrawSelf();
        return true;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    namespace {
        const char albedo_vertex_shader_[] =
                "#version 330 core\n"
                "layout (location = 0) in vec3 in_position;\n"
                "layout (location = 1) in vec3 in_color;\n"
                "layout (location = 3) in mat4 in_model_to_world;\n"
                "uniform mat4 world_to_cam;\n"
                "uniform mat4 projection;\n"
                "out vec3 _color;\n"
                "void main() {\n"
                "vec4 homogeneous = vec4(in_position, 1.0);\n"
                "gl_Position = projection * world_to_cam * in_model_to_world * homogeneous;\n"
                "_color = in_color;\n"
                "}\0";

        const char albedo_fragment_shader_[] =
                "#version 330 core\n"
                "out vec3 fragment_color;\n"
                "in vec3 _color;\n"
                "void main() {\n"
                "   fragment_color = _color;\n"
                "}\0";
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    const char *AlbedoCameraShader::FragmentShader() const {
        return albedo_fragment_shader_;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    const char *AlbedoCameraShader::VertexShader() const {
        return albedo_vertex_shader_;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    /// EDLScreenShader
    namespace {

        const char edl_vertex_shader_[] =
                "#version 330 core\n"
                "layout (location = 0) in vec2 in_position;\n"
                "layout (location = 2) in vec2 in_tex_coord;\n"
                "out vec2 TexCoord;\n"
                "void main() {\n"
                "gl_Position = vec4(in_position.x, in_position.y, 0.0, 1.0);\n"
                "TexCoord = in_tex_coord;\n"
                "}\0";

        // Code strongly inspired by
        // https://github.com/CesiumGS/cesium/blob/master/Source/Shaders/PostProcessStages/PointCloudEyeDomeLighting.glsl
        const char edl_fragment_shader_[] =
                "#version 330 core\n"
                "out vec4 fragment_color;\n"
                "in vec2 TexCoord;\n"
                "uniform sampler2D colorTexture;\n"
                "uniform sampler2D depthTexture;\n"
                "uniform float A;\n"
                "uniform float B;\n"
                "uniform float EDL_STRENGTH;\n"
                "uniform float EDL_DISTANCE;\n"
                "uniform bool WITH_EDL;\n"
                "vec4 color;\n"
                "float buffer_depth;\n"
                "float real_depth;\n"
                "float buffer_depth_to_real_depth(float depth)\n"
                "{\n"
                "   return 0.5*(-A * depth + B) / depth + 0.5;\n"
                "}\n"
                ""
                "vec2 neighborContribution(float log2Depth, vec2 offset) {\n"
                "   float dist = EDL_DISTANCE;\n"
                "   vec2 texCoordOrig = TexCoord + offset * dist;\n"
                "   vec2 texCoord0 = TexCoord + offset * floor(dist);\n"
                "   vec2 texCoord1 = TexCoord + offset * ceil(dist);\n"
                "   float depthOrLogDepth0 = texture2D(depthTexture, texCoord0).r;\n"
                "   float depthOrLogDepth1 = texture2D(depthTexture, texCoord1).r;\n"
                "   if (depthOrLogDepth0 == 0.0 || depthOrLogDepth1 == 0.0) {\n"
                "   return vec2(0.0);\n"
                "}\n"
                "// interpolate the two adjacent depth values\n"
                "float depthMix = mix(depthOrLogDepth0, depthOrLogDepth1, fract(dist));\n"
                "float new_depth = buffer_depth_to_real_depth(depthMix);\n"
                "return vec2(max(0.0, log2Depth - log2(new_depth)), 1.0);\n"
                "}\n"
                "void main() {\n"
                "color = texture(colorTexture, TexCoord);\n"
                "if (WITH_EDL) {\n"
                "   // Build the Depth\n"
                "   buffer_depth = texture(depthTexture, TexCoord).r;\n"
                "   real_depth = buffer_depth_to_real_depth(buffer_depth);\n"
                "   float log2Depth = log2(real_depth);\n"
                "   if (log2Depth == 0)\n"
                "      discard;\n"
                "   vec2 texelSize = 1.0 / vec2(720, 1080);\n"
                "   vec2 responseAndCount = vec2(0.0);\n"
                "   responseAndCount += neighborContribution(log2Depth, vec2(-texelSize.x, 0.0));\n"
                "   responseAndCount += neighborContribution(log2Depth, vec2(+texelSize.x, 0.0));\n"
                "   responseAndCount += neighborContribution(log2Depth, vec2(0.0, -texelSize.y));\n"
                "   responseAndCount += neighborContribution(log2Depth, vec2(0.0, +texelSize.y));\n"
                "   // Build the Eye Dome Lighting effect PostProcessing\n"
                "   float response = responseAndCount.x / responseAndCount.y;\n"
                "   float shade = exp(-response * 300.0 * EDL_STRENGTH);\n"
                "   color.rgb *= shade;\n"
                "}\n"
                "fragment_color = vec4(color);\n"
                "}\0";
    }


    /* -------------------------------------------------------------------------------------------------------------- */
    const char *EDLScreenShader::FragmentShader() const {
        return edl_fragment_shader_;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    const char *EDLScreenShader::VertexShader() const {
        return edl_vertex_shader_;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    bool EDLScreenShader::RunOnModel(AModel &model) {
        auto pid = shader_program_->GLShaderProgramId();
        glUseProgram(pid);
        glBindVertexArray(model.VAO());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gl_color_texture_);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, gl_depth_texture_);

        auto &screen_model_ptr = dynamic_cast<ScreenModel &>(model);
        model.DrawSelf();
        glBindVertexArray(0);
        glUseProgram(0);

        return true;
    }

} // namespace viz