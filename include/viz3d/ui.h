#ifndef VIZ3D_UI_H
#define VIZ3D_UI_H

#include <string>
#include <memory>
#include <map>

#include <GLES3/gl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <implot.h>

namespace viz3d {

    /**!
     * @brief An abstract GUIWindow, which groups a set of widgets in a named window
     */
    class ImGuiWindow {
    public:
        virtual ~ImGuiWindow() = 0;

        ImGuiWindow(std::string &&winname) : window_name_(std::move(winname)) {}

        virtual void Draw();

        virtual void DrawImGUIContent() = 0;

        virtual void EndContext() {};

        virtual void BeginContext() {};

    protected:
        bool display_window = true;
        std::string window_name_;

        friend class GUI;
    };

    typedef std::shared_ptr<ImGuiWindow> GUIWindowPtr;


    /**!
     * @brief   A GUI composed of a set of windows, built on top of ImGUI, ImPlot and VTK
     */
    class GUI {
    public:

        // Singleton GUI instance
        static GUI &Instance(std::string &&window_name = "GUI Window");

        // Launches the GUI's main loop
        void MainLoop();

        // Launches the main loop, for the singleton GUI
        static void LaunchMainLoop(std::string &&window_name = "GUI Window");;

        // Adds a window to the GUI, returns the id of the window
        size_t AddWindow(GUIWindowPtr window);

        // Removes a window from the GUI
        void RemoveWindow(size_t window_id);

        GUI(const GUI &) = delete;

        GUI &operator=(const GUI &) = delete;

        // Remove all windows
        void ClearWindows();

        // Signal the engine to close
        void SignalClose();

    private:
        explicit GUI(std::string &&winname = "GUI Window");

        struct GLFWContext {
            GLFWwindow *window;
            int width = 720;
            int height = 720;
            std::string window_name = "GUI Window";
        } glfwContext;

        bool remain_open = true;
        bool is_initialized_ = false;
        bool clear_windows = false;
        std::map<size_t, GUIWindowPtr> windows_;
        static size_t window_id_;

        bool InitializeGUI();

        bool RenderImGUIFrame(ImGuiIO &io);

        static void GLFWResizeCallback(GLFWwindow *, int, int);
    };

} // namespace viz3d

#endif //VIZ3D_UI_H
