#ifndef VIZ3D_VTK_WINDOW_H
#define VIZ3D_VTK_WINDOW_H

#include <set>
#include <mutex>

#include <glog/logging.h>

#include <viz3d/ui.h>
#include "viz3d/imgui_utils.h"
#include "config.h"

#include <imgui_internal.h>

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCallbackCommand.h>
#include <vtkInteractorStyle.h>
#include <vtkGenericRenderWindowInteractor.h>
#include <vtkAutoInit.h>


VTK_MODULE_INIT(vtkRenderingOpenGL2);

VTK_MODULE_INIT(vtkInteractionStyle);

VTK_MODULE_INIT(vtkRenderingFreeType);

namespace viz3d {

    class VTKWindow : public ImGuiWindow {
    public:
        VTKWindow(std::string &&winname);

        // Draws the ImGui content of the window, including the VTKWindow, and the
        void DrawImGUIContent() override;

        // Adds a VTK Actor to the window, returns the Id of the Actor
        void AddActor(vtkSmartPointer<vtkActor> actor);

        void Draw() override;

        // Removes a VTK Actor from the window
        void RemoveActor(vtkSmartPointer<vtkActor> actor);

        // Initializes the VTK Context for the window, must be called before DrawImGUIContent is called
        virtual void InitializeVTKContext();

        // Draws the ImGui components in a subordinated window
        // the subordinated window elements are drawn by calling DrawSubordinatedImGuiContent
        virtual void DrawImGuiWindowConfigurations();

        // Draws the VTK Window in a canvas of the ImGui Window
        virtual void DrawVTKWindow();

        void EndContext() override;

        void BeginContext() override;

        ~VTKWindow();

    protected:

        // Draws the elements of the subordinated window
        virtual void DrawSubordinatedImGuiContent();

        struct ImGuiVars_ : ParamGroup {

            explicit ImGuiVars_(VTKWindow &window) : viz3d::ParamGroup(window.window_name_ + "_VTK_Window_Vars",
                                                                       "Default Window Form"),
                                                     color_combo(std::string(window.window_name_)) {
                color_scale_range.value[0] = 0.;
                color_scale_range.value[1] = 1.;
            };

            VIZ3D_PARAM_WITH_DEFAULT_VALUE(FloatParam, point_size, "Point Size", "The OpenGL point size", 2.f)
            VIZ3D_PARAM_WITH_DEFAULT_VALUE(FloatParam, line_size, "Line Size", "The OpenGL line size", 2.f)
            VIZ3D_PARAM_WITH_DEFAULT_VALUE(BoolParam, with_edl_shader, "With EDL Shader",
                                           "Whether to activate the EDL Shader", 2.f)
            VIZ3D_PARAM_WITH_DEFAULT_VALUE(BoolParam, open_window_options, "Open Window options",
                                           "Whether to open the Window Options window", 2.f)
            VIZ3D_PARAM_WITH_DEFAULT_VALUE(FloatArray3, background_color,
                                           "Background Color", "The Background Color for VTK's viewport", 0.f)

            VIZ3D_PARAM_WITH_DEFAULT_VALUE(FloatArray2, color_scale_range,
                                           "Color Scale Range", "The range of the color scale for the actors", 0.f)
            ImGui_ColorMapCombo color_combo;
        VIZ3D_REGISTER_PARAM(color_combo)

        } imgui_vars_;
        const std::string options_winname_;

        void ColorRangePopup(bool open = true);

        void RenderingPopup(bool open = true);

        void BackgroundPopup(bool open = true);

        struct VTKWindowContext {
            vtkSmartPointer<vtkRenderWindow> render_window = nullptr;
            vtkSmartPointer<vtkRenderWindowInteractor> interactor = nullptr;
            vtkSmartPointer<vtkInteractorStyle> interactor_style = nullptr;
            vtkSmartPointer<vtkRenderer> renderer = nullptr;
            bool show_window = true;
            int vport_size[2] = {640, 480};
            bool is_initialized = false;

            // OpenGL Ids
            GLuint FBOId = 0; //< FrameBuffer Id
            GLuint RBOId = 0; //< RenderBuffer Id
            GLuint textureId = 0; //< Texture Id

        } _vtk_context;

        std::set<vtkSmartPointer<vtkActor>> actors_, actors_to_remove_, actors_to_add_;
        std::mutex actors_management_mutex_;

        static void VTKIsCurrentCallback(vtkObject *caller, long unsigned int eventId, void *clientData,
                                         void *callData);

        void SetVPortSize(int w, int h);

        void ProcessEvents();

    };

} // namespace viz3d

#endif //VIZ3D_VTK_WINDOW_H
