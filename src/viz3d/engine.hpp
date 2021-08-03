#ifndef LIDAR_SFM_ENGINE_H
#define LIDAR_SFM_ENGINE_H

// STL
struct ImGuiIO;

#include <chrono>
#include <map>
#include <set>
#include <list>

// External
#include <glog/logging.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mutex>

// Project
#include "model.hpp"
#include "camera.hpp"
#include "shader.hpp"

struct ImGuiIO;

namespace viz {

    class ExplorationEngine;

    /// Engine Options which modify the behaviour of the engine
    /// TODO: - Add a mechanism of notification to the engine
    struct EngineOptions {

        int height = 600; // The initial height of the window

        int width = 800; // The Initial width of the window

        std::string title = "Debug Window"; // The title of the GLFW window

        int max_fps = 60; // The maximum number of frame per seconds rendered

        Eigen::Vector4f background_color = Eigen::Vector4f(1.f, 1.f, 1.f, 1.0);

        int num_updates_per_frame = 1;

        /// EDL SHADER PARAMETERS
        bool with_edl = true;

        float edl_strength = 100.0;

        float edl_distance = 1.0;
    };

    /// An Exploration Engine is a Rasterization based rendering engine built with a user controlled camera
    /// Which allows him to move through a scene.
    ///
    /// The Exploration Engine is implemented as a Singleton, (thus only one can exist by process);
    ///
    /// Rendering is performed in two passes:
    ///     - A First pass renders the scene into a texture (see CameraAlbedoShader)
    ///     - A Second pass renders the textures into the screen (with optionally some processing effects)
    ///
    /// TODO:
    ///     - Send commands to the Engine
    class ExplorationEngine {
    private:
        using steady_clock = std::chrono::steady_clock;
    public:

        class GUIWindow {
        public:

            virtual ~GUIWindow() = 0;

            GUIWindow(std::string &&winname, bool *display_window) :
                    window_name_(winname), display_window_(display_window) {}

            virtual void Draw();

            virtual void DrawContent() = 0;

            const std::string &WindowName() { return window_name_; }

        protected:
            std::string window_name_;
            bool *display_window_;

            friend class ExplorationEngine;
        };

        typedef std::shared_ptr<GUIWindow> GUIWindowPtr;

        static bool HasContext() {
            return Instance().window_ != nullptr;
        }

        static ExplorationEngine &Instance();

        EngineOptions &Options() {
            return options_;
        }

        ~ExplorationEngine();

        // Update All models which need updating
        bool UpdateModels();

        // Runs the Main Loop
        bool MainLoop();

        // Adds a Model
        void AddModel(int model_id_, ModelPtr model);

        // Adds a GUIWindow
        void AddWindow(GUIWindowPtr window_ptr);

        // Removes a Model
        void RemoveModel(int model_id_);

        // Returns whether the Engine is already Initialized
        bool IsInitialized();

        static bool LaunchMainLoop();

        // Signals the engine that it should close
        void SignalClose();

        ExplorationEngine();

    private:
        static bool statically_initialized_;
        static std::timed_mutex instance_mutex_;

        bool InitializeGLFW();

        bool StaticClean();

        // Renders the Scene
        bool RenderScene();

        // Initialize the Window
        bool Init();

        explicit ExplorationEngine(int index) : index_(index) {}

        int index_ = -1;
        bool do_close_;
        EngineOptions options_;
        GLFWwindow *window_ = nullptr;
        std::shared_ptr<ACamera> camera_ = nullptr;


        int height_viewport_, width_viewport_;
        int viewport_pos_x_, viewport_pos_y_;

        GLuint first_pass_frame_buffer_ = -1;
        GLuint second_pass_frame_buffer = -1;
        GLuint gl_color_texture_id_ = -1, gl_depth_texture_id_ = -1, gl_post_process_texture_id_;

        AlbedoCameraShader camera_shader_;
        EDLScreenShader screen_shader_;
        ScreenModel screen_model_;

        std::list<std::shared_ptr<GUIWindow>> gui_windows_;

        std::set<int> models_to_update_;
        // Models must deallocate buffers, thus should only be deleted from the GUI Thread
        std::set<ModelPtr> garbage_can_;
        std::map<int, ModelPtr> models_;

        std::timed_mutex mutex_add_model_;

        // The Framebuffer Resize Callback
        static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

        // Input Key Callbacks
        static void window_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

        // Input Mouse Callback
        static void window_mouse_callback(GLFWwindow *window, double xpos, double ypos);

        // Input Mouse Button Callback
        static void window_mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

        // Define the textures framebuffer
        void BindTextures();

        // Unsafe remove
        void UnsafeRemoveModel(int index);

        bool ImGUIDrawFrame(ImGuiIO &);
    };


} // namespace viz


#endif //LIDAR_SFM_ENGINE_H
