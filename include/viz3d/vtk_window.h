#ifndef VIZ3D_VTK_WINDOW_H
#define VIZ3D_VTK_WINDOW_H

#include <set>
#include <mutex>

#include <glog/logging.h>

#include <viz3d/ui.h>

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
        using ImGuiWindow::ImGuiWindow;

        // Draws the content of the window
        void DrawImGUIContent() override;

        // Adds a VTK Actor to the window, returns the Id of the Actor
        void AddActor(vtkSmartPointer<vtkActor> actor);

        // Removes a VTK Actor from the window
        void RemoveActor(vtkSmartPointer<vtkActor> actor);

        // Initializes the VTK Context for the window, must be called before DrawImGUIContent is called
        virtual void InitializeVTKContext();

        // Draws the ImGui components specifying the configuration of the window
        virtual void DrawImGuiWindowConfigurations();

        // Draws the VTK Window in a canvas of the ImGui Window
        virtual void DrawVTKWindow();

        void EndContext() override;

        void BeginContext() override;

        ~VTKWindow();

    protected:

        struct ImGuiVars_ {
            float point_size = 2.;
            float line_width = 2.;
            bool with_edl_shader = false;
        } imgui_vars_;

        void ColorRangePopup();

        void RenderingPopup();

        void BackgroundPopup();

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
