#include <viz3d/ui.h>

#include <iostream>
#include <glog/logging.h>

#include <GLES3/gl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace viz3d {


    namespace {

        // Sets the color elements of ImGUI components
        void DetDefaultTheme() {

            auto &style = ImGui::GetStyle();

            style.FrameBorderSize = 1.0;
            style.FrameRounding = 0.;

            // Setup default padding
            style.WindowPadding = ImVec2(2, 2);
            style.FramePadding = ImVec2(10, 5);

            auto &colors = style.Colors;

            colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};
            colors[ImGuiCol_Header] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
            colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
            colors[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
            colors[ImGuiCol_Button] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
            colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
            colors[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
            colors[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
            colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
            colors[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
            colors[ImGuiCol_Tab] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
            colors[ImGuiCol_TabHovered] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
            colors[ImGuiCol_TabActive] = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
            colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
            colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
            colors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
            colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
            colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

            // Setup Default Sizes
            auto selection_color = ImVec4(0.63, 0.11f, 0, 1.0f);
            colors[ImGuiCol_SliderGrab] = selection_color;
            colors[ImGuiCol_SliderGrabActive] = selection_color;
            colors[ImGuiCol_CheckMark] = selection_color;
            colors[ImGuiCol_SeparatorActive] = selection_color;
            colors[ImGuiCol_SeparatorHovered] = selection_color;
            colors[ImGuiCol_ResizeGrip] = selection_color;
            colors[ImGuiCol_ResizeGripHovered] = selection_color;
            colors[ImGuiCol_ResizeGripActive] = selection_color;
            colors[ImGuiCol_DockingPreview] = selection_color;
            colors[ImGuiCol_TextSelectedBg] = selection_color;
            colors[ImGuiCol_NavHighlight] = selection_color;
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    ImGuiWindow::~ImGuiWindow() = default;

    /* -------------------------------------------------------------------------------------------------------------- */
    void ImGuiWindow::Draw() {
        if (!ImGui::Begin(window_name_.c_str())) {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }
        DrawImGUIContent();
        ImGui::End();
    }


    /* -------------------------------------------------------------------------------------------------------------- */
    void GUI::ClearWindows() {
        if (!is_initialized_)
            windows_.clear();
        else
            clear_windows = true;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void GUI::MainLoop() {
        // Initialize the GLFW Window and OpenGL context
        CHECK(InitializeGUI()) << "Could not initialize OpenGL Window Context.";

        // Initialize ImGui & ImPlot
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
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
        DetDefaultTheme();

        ImGui_ImplGlfw_InitForOpenGL(glfwContext.window, true);
        ImGui_ImplOpenGL3_Init();

        // Initialize the context of each window
        for (auto &window: windows_)
            window.second->BeginContext();

        while (!glfwWindowShouldClose(glfwContext.window) && remain_open) {

            if (clear_windows) {
                windows_.clear();
                clear_windows = false;
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, glfwContext.width, glfwContext.height);
            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            RenderImGUIFrame(io);
            glfwSwapBuffers(glfwContext.window);
            glfwPollEvents();
        }

        // End the context of each window
        for (auto &window: windows_)
            window.second->EndContext();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();
        glfwTerminate();
        remain_open = true;
        is_initialized_ = false;
        glfwContext.window = nullptr;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    size_t GUI::AddWindow(GUIWindowPtr window) {
        size_t window_id = window_id_++;
        windows_[window_id] = window;
        return window_id;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void GUI::RemoveWindow(size_t window_id) {
        if (windows_.find(window_id) != windows_.end())
            windows_.erase(window_id);
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void GUI::GLFWResizeCallback(GLFWwindow *, int height, int width) {
        auto &instance = GUI::Instance();
        instance.glfwContext.height = height;
        instance.glfwContext.width = width;
    }


    /* -------------------------------------------------------------------------------------------------------------- */
    bool GUI::InitializeGUI() {
        if (is_initialized_)
            return true;

        // Initialize glfw
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwContext.window = glfwCreateWindow(glfwContext.width, glfwContext.height,
                                              glfwContext.window_name.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(glfwContext.window);
        if (!glfwContext.window) {
            LOG(FATAL) << "Failed to create a GLFW Window" << std::endl;
            glfwTerminate();
            return false;
        }

        // Setup handlers for the redimension of the window
        glfwSetFramebufferSizeCallback(glfwContext.window, GLFWResizeCallback);

        // Load GL functions with GLAD
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            LOG(FATAL) << "Failed to initialize GLAD" << std::endl;
            return false;
        }

        is_initialized_ = true;
        remain_open = true;
        return true;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    GUI::GUI(std::string &&winname) { glfwContext.window_name = std::move(winname); }

    /* -------------------------------------------------------------------------------------------------------------- */
    GUI &GUI::Instance(std::string &&window_name) {
        static GUI window(std::move(window_name));
        return window;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    size_t GUI::window_id_ = 0;


    /* -------------------------------------------------------------------------------------------------------------- */
    bool GUI::RenderImGUIFrame(ImGuiIO &io) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Setup the Root Docking Space covering the whole GLFW Window
        static bool is_initial_layout = true;
        static bool open_root_dockspace = true;
        static bool show_imgui_demo_window = false;
        static bool show_implot_demo_window = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiStyle &style = ImGui::GetStyle();
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");

        {
            // Main Dock Space
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            ImGuiViewport *viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            ImGui::Begin("Root DockSpace", &open_root_dockspace, window_flags);
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

            // Setup a default layout
            ImGui::PopStyleVar(2);

            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("Help")) {
                    ImGui::MenuItem("Show ImGui Demo Window", NULL, &show_imgui_demo_window);
                    ImGui::MenuItem("Show Implot Demo Window", NULL, &show_implot_demo_window);
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            ImGui::End();
        }

        if (show_imgui_demo_window) {
            if (is_initial_layout)
                ImGui::SetNextWindowDockID(dockspace_id);
            ImGui::ShowDemoWindow(&show_imgui_demo_window);
        }

        if (show_implot_demo_window) {
            if (is_initial_layout)
                ImGui::SetNextWindowDockID(dockspace_id);
            ImPlot::ShowDemoWindow(&show_implot_demo_window);
        }

        for (auto &window: windows_) {
            if (is_initial_layout)
                ImGui::SetNextWindowDockID(dockspace_id);
            if (window.second)

                window.second->Draw();
        }
        if (is_initial_layout)
            is_initial_layout = false;

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
        return true;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void GUI::LaunchMainLoop(std::string &&window_name) {
        Instance(std::move(window_name)).MainLoop();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void GUI::SignalClose() {
        remain_open = false;
    }


} // namespace viz3d


