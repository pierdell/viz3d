#include "engine.hpp"
#include "image.hpp"

#include <iostream>
#include <thread>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"


namespace viz {

    /* -------------------------------------------------------------------------------------------------------------- */
    /// Options GUIWindow
    class OptionsGUIWindow : public ExplorationEngine::GUIWindow {
    public:
        OptionsGUIWindow(std::string &&winname, bool *display_window, EngineOptions *options)
                : ExplorationEngine::GUIWindow(std::move(winname), display_window), options_(options) {}

        void DrawContent() override {
            // Background Color
            ImGui::SliderFloat3("Background Color", &options_->background_color[0], 0.0f, 1.0f);
            // EDL Shader
            ImGui::Checkbox("EDL shader", &options_->with_edl);
            // EDL Strength
            ImGui::SliderFloat("EDL strength", &options_->edl_strength, 1.0, 10000.0);
        }

    private:
        EngineOptions *options_;
    };

    /* -------------------------------------------------------------------------------------------------------------- */

    static bool _is_init = false;

    /* -------------------------------------------------------------------------------------------------------------- */
    bool ExplorationEngine::Init() {
        InitializeGLFW();
        _is_init = true;

        // TODO - Support multiple windows
        window_ = glfwCreateWindow(options_.width, options_.height, options_.title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(window_);
        if (window_ == nullptr) {
            LOG(FATAL) << "Failed to create a GLFW Window" << std::endl;
            glfwTerminate();
            return false;
        }

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            LOG(FATAL) << "Failed to initialize GLAD" << std::endl;
            return false;
        }


        // Set the dimensions of the rendering window
        glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
        glfwSetKeyCallback(window_, window_key_callback);
        glfwSetCursorPosCallback(window_, window_mouse_callback);
        glfwSetMouseButtonCallback(window_, window_mouse_button_callback);

        // Initialize the Camera
        camera_ = std::make_shared<FPVCamera>();
        camera_->ChangeDimensions(options_.height, options_.width);

        // Initializes the Framebuffer to render the scene in a texture
        glGenFramebuffers(1, &first_pass_frame_buffer_);
        glGenTextures(1, &gl_color_texture_id_);
        glGenTextures(1, &gl_depth_texture_id_);
        BindTextures();

        // Initializes the Shaders

        camera_shader_.Init();
        screen_shader_.Init();

        auto projection_matrix = camera_->ProjectionMatrix();
        camera_shader_.SetProjectionMatrix(projection_matrix);
        screen_shader_.SetUpTextures(gl_color_texture_id_, gl_depth_texture_id_);
        screen_shader_.SetUpEDL(options_.with_edl, options_.edl_strength, options_.edl_distance);

        // Initialize the Models
        UpdateModels();
        screen_model_.InitModel();

        return true;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    bool ExplorationEngine::UpdateModels() {
        CHECK(mutex_add_model_.try_lock_for(std::chrono::duration<double>(1.0)))
        << "RACE CONDITION at " << __FILE__ << " line : " << __LINE__ << std::endl;

        for (auto &model_id : models_to_update_) {
            auto &model = models_[model_id];
            if (!model->IsInitialized())
                model->InitModel();
            else
                model->UpdateModel();
        }

        models_to_update_.clear();
        garbage_can_.clear();
        mutex_add_model_.unlock();
        return true;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    bool ExplorationEngine::MainLoop() {
        bool result = Init();
        CHECK(result) << "Could not Initialize the GLFW window.";

        // Launches the main loop
        double s_per_frame = 1.0 / (double) options_.max_fps;

        auto previous_tick = steady_clock::now();
        std::chrono::duration<double> elapsed, lag, delta_sleep;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        ImGui::StyleColorsDark();
        ImGuiStyle &style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplGlfw_InitForOpenGL(window_, true);
        ImGui_ImplOpenGL3_Init();
        static bool options_window = true;
        gui_windows_.push_back(std::make_shared<OptionsGUIWindow>("EngineOptions",
                                                                  &options_window,
                                                                  &options_));

        while (!glfwWindowShouldClose(window_)) {
            screen_shader_.SetUpEDL(options_.with_edl, options_.edl_strength, options_.edl_distance);

            auto new_tick = steady_clock::now();
            // Update Models
            CHECK(UpdateModels()) << "Could not Update models" << std::endl;

            // Update Camera Physics
            lag = new_tick - previous_tick;
            camera_->UpdatePhysics(lag.count());
            previous_tick = new_tick;

            // Renders the Scene
            CHECK(RenderScene()) << "Could not Render The scene" << std::endl;

            elapsed = steady_clock::now() - previous_tick;

            if (elapsed.count() < s_per_frame) {
                delta_sleep = std::chrono::duration<double>(s_per_frame - elapsed.count());
                std::this_thread::sleep_for(delta_sleep);
            }
            previous_tick = new_tick;

            CHECK(ImGUIDrawFrame(io)) << "Failed to Draw ImGui windows";

            glfwSwapBuffers(window_);
            glfwPollEvents();
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        std::cout << 6 << std::endl;


        return true;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void ExplorationEngine::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
        auto &instance = Instance();
        instance.options_.height = height;
        instance.options_.width = width;
        instance.camera_->ChangeDimensions(height, width);
        instance.BindTextures();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void ExplorationEngine::window_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto &engine = Instance();
        auto &camera = *engine.camera_;
        ACamera::EVENT_ACTION event_action = (action == GLFW_PRESS ? ACamera::EVENT_ACTION::KEY_DOWN :
                                              (action == GLFW_RELEASE ? ACamera::EVENT_ACTION::KEY_UP :
                                               ACamera::EVENT_ACTION::REPEAT));
        switch (key) {
            case GLFW_KEY_W:
                camera.ProcessEvent(ACamera::FRONT, event_action);
                break;
            case GLFW_KEY_S:
                camera.ProcessEvent(ACamera::BACK, event_action);
                break;
            case GLFW_KEY_D:
                camera.ProcessEvent(ACamera::RIGHT, event_action);
                break;
            case GLFW_KEY_A:
                camera.ProcessEvent(ACamera::LEFT, event_action);
                break;
            case GLFW_KEY_SPACE:
                camera.ProcessEvent(ACamera::SWITCH_MODE_0, event_action);
                break;
            case GLFW_KEY_R:
                camera.ProcessEvent(ACamera::UP, event_action);
                break;
            case GLFW_KEY_F:
                camera.ProcessEvent(ACamera::DOWN, event_action);
                break;
            case GLFW_KEY_Q:
                camera.ProcessEvent(ACamera::R_LEFT, event_action);
                break;
            case GLFW_KEY_E:
                camera.ProcessEvent(ACamera::R_RIGHT, event_action);
                break;
            case GLFW_KEY_LEFT_SHIFT:
                camera.ProcessEvent(ACamera::ACCELERATE, event_action);
                break;
            default:
                break;
        }


    }

    /* -------------------------------------------------------------------------------------------------------------- */
    bool ExplorationEngine::RenderScene() {
        // Draw in Texture
        glBindFramebuffer(GL_FRAMEBUFFER, first_pass_frame_buffer_);
        glEnable(GL_DEPTH_TEST);
        glClearColor(options_.background_color[0], options_.background_color[1],
                     options_.background_color[2], options_.background_color[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera_shader_.SetWorldToCam(camera_->WorldToCam());
        camera_shader_.SetProjectionMatrix(camera_->ProjectionMatrix());
        mutex_add_model_.lock();
        for (auto &pair : models_) {
            if (pair.second->IsInitialized())
                camera_shader_.RunOnModel(*pair.second);
        }

        // Draw on Screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, options_.width, options_.height);
        glDisable(GL_DEPTH_TEST);

        screen_shader_.RunOnModel(screen_model_);
        mutex_add_model_.unlock();

        return true;
    }

    /* -------------------------------------------------------------------------------------------------------------- */

    void ExplorationEngine::BindTextures() {

        int height = options_.height;
        int width = options_.width;

        glBindFramebuffer(GL_FRAMEBUFFER, first_pass_frame_buffer_);
        // Generate color texture for color attachment
        glBindTexture(GL_TEXTURE_2D, gl_color_texture_id_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gl_color_texture_id_, 0);

        glBindTexture(GL_TEXTURE_2D, gl_depth_texture_id_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Attach the texture as the framebuffer's depth buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gl_depth_texture_id_, 0);
        CHECK(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
        << "[OpenGL]Invalid Framebuffer" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


    /* -------------------------------------------------------------------------------------------------------------- */
    void ExplorationEngine::AddModel(int model_id_, ModelPtr model) {
        CHECK(mutex_add_model_.try_lock_for(std::chrono::duration<double>(1.0)))
        << "RACE CONDITION at " << __FILE__ << " line : " << __LINE__ << std::endl;
        if (models_.find(model_id_) != models_.end())
            UnsafeRemoveModel(model_id_);
        models_[model_id_] = model;
        models_to_update_.emplace(model_id_);
        mutex_add_model_.unlock();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    bool ExplorationEngine::IsInitialized() {
        bool _is_initialized = window_ != nullptr;
        return _is_initialized;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void ExplorationEngine::window_mouse_callback(GLFWwindow *window, double xpos, double ypos) {
        auto &engine = Instance();
        auto &camera = *engine.camera_;
        engine.camera_->CursorPosition(xpos, ypos);
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void ExplorationEngine::window_mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
        auto &engine = Instance();
        auto &camera = *engine.camera_;

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
            camera.ProcessEvent(ACamera::SWITCH_MODE_0, ACamera::KEY_DOWN);
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    ExplorationEngine::~ExplorationEngine() {
        CHECK(mutex_add_model_.try_lock_for(std::chrono::duration<double>(1.0)))
        << "RACE CONDITION at " << __FILE__ << " line : " << __LINE__ << std::endl;
        for (auto &model : models_)
            model.second->Remove();
        screen_model_.Remove();

        if (HasContext()) {
            glfwWindowShouldClose(window_);
        }

        StaticClean();
        window_ = nullptr;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    bool ExplorationEngine::LaunchMainLoop() {
        LOG(INFO) << "Launching Engine in Main Loop" << std::endl;
        try {
            auto &instance = Instance();

            bool result = instance.MainLoop();

            CHECK(result);

            return instance.StaticClean();
        } catch (...) {
            LOG(ERROR) << " An error was found. Exiting." << std::endl;
            return false;
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    bool ExplorationEngine::InitializeGLFW() {
        if (!statically_initialized_) {
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            statically_initialized_ = true;
        }

        return true;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    bool ExplorationEngine::StaticClean() {
        glfwTerminate();
        statically_initialized_ = false;
        return true;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    bool ExplorationEngine::statically_initialized_ = false;

    /* -------------------------------------------------------------------------------------------------------------- */
    void ExplorationEngine::RemoveModel(int model_id_) {
        CHECK(mutex_add_model_.try_lock_for(std::chrono::duration<double>(1.0)))
        << "RACE CONDITION at " << __FILE__ << " line : " << __LINE__ << std::endl;
        UnsafeRemoveModel(model_id_);
        mutex_add_model_.unlock();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    std::timed_mutex ExplorationEngine::instance_mutex_;

    /* -------------------------------------------------------------------------------------------------------------- */
    ExplorationEngine &ExplorationEngine::Instance() {
        static std::unique_ptr<ExplorationEngine> engine_ = nullptr;
        {
            std::lock_guard<std::timed_mutex> lock(instance_mutex_);
            if (engine_ == nullptr) {
                engine_ = std::make_unique<ExplorationEngine>();
            }
        }

        return *engine_;
    }

    ExplorationEngine::ExplorationEngine() = default;

    /* -------------------------------------------------------------------------------------------------------------- */
    // Lock-free Remove Model
    void ExplorationEngine::UnsafeRemoveModel(int model_id_) {
        bool has_model = models_.find(model_id_) != models_.end();
        if (!has_model) {
            return;
        }

        garbage_can_.emplace(std::move(models_[model_id_]));
        models_.erase(model_id_);
        if (models_to_update_.find(model_id_) != models_to_update_.end())
            models_to_update_.erase(model_id_);

    }

    /* -------------------------------------------------------------------------------------------------------------- */
    bool ExplorationEngine::ImGUIDrawFrame(ImGuiIO &io) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for (auto &window : gui_windows_) {
            if (window != nullptr)
                window->Draw();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        return true;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void ExplorationEngine::AddWindow(ExplorationEngine::GUIWindowPtr ptr) {
        gui_windows_.push_back(ptr);
    }


    /* -------------------------------------------------------------------------------------------------------------- */
    /// GUIWindow
    ExplorationEngine::GUIWindow::~GUIWindow() = default;

    /* -------------------------------------------------------------------------------------------------------------- */
    void ExplorationEngine::GUIWindow::Draw() {
        if (!ImGui::Begin(window_name_.c_str(), display_window_)) {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }
        DrawContent();
        ImGui::End();
    }


} // namespace viz
