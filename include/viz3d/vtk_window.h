#ifndef VIZ3D_VTK_WINDOW_H
#define VIZ3D_VTK_WINDOW_H

#include <viz3d/ui.h>

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCallbackCommand.h>
#include <vtkInteractorStyle.h>
#include <vtkGenericRenderWindowInteractor.h>

#include <glog/logging.h>

namespace viz3d {

    class VTKWindow : public ImGuiWindow {
    public:
        using ImGuiWindow::ImGuiWindow;

        // Draws the content of the window
        void DrawImGUIContent() override;

        // Adds a VTK Actor to the window
        void AddActor(vtkSmartPointer<vtkActor> actor);

        // Removes a VTK Actor from the window
        void RemoveActor(vtkSmartPointer<vtkActor> actor);

        // Initializes the VTK Context for the window, must be called before DrawImGUIContent is called
        void InitializeVTKContext();

        ~VTKWindow();

    private:
        struct VTKWindowContext {
            vtkSmartPointer<vtkRenderWindow> render_window = nullptr;
            vtkSmartPointer<vtkRenderWindowInteractor> interactor = nullptr;
            vtkSmartPointer<vtkInteractorStyle> interactor_style = nullptr;
            vtkSmartPointer<vtkRenderer> renderer = nullptr;
            bool show_window = true;
            int vport_size[2] = {640, 480};
            bool is_initialized = false;
            bool with_edl_shader = false;

            // OpenGL Ids
            GLuint FBOId = 0; //< FrameBuffer Id
            GLuint RBOId = 0; //< RenderBuffer Id
            GLuint textureId = 0; //< Texture Id

        } _vtk_context;

        static void VTKIsCurrentCallback(vtkObject *caller, long unsigned int eventId, void *clientData,
                                         void *callData);

        void SetVPortSize(int w, int h);

        void ProcessEvents();

    };

} // namespace viz3d

#endif //VIZ3D_VTK_WINDOW_H
