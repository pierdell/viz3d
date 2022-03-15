#include <viz3d/vtk_window.h>

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleMultiTouchCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderStepsPass.h>
#include <vtkEDLShading.h>
#include <vtkProperty.h>
#include <vtkMapper.h>
#include <vtkDataSet.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkCollectionIterator.h>

namespace viz3d {

    namespace {

        void VTKCreateTimerCallback(vtkObject *caller, unsigned long eventId, void *clientData, void *callData) {
            // What I am supposed to do ?
            std::cout << "Called " << std::endl;
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void VTKWindow::InitializeVTKContext() {
        assert(!_vtk_context.interactor);
        assert(!_vtk_context.interactor_style);
        assert(!_vtk_context.interactor);
        assert(!_vtk_context.render_window);

        auto &renderer = _vtk_context.renderer;
        renderer = vtkSmartPointer<vtkRenderer>::New();
        renderer->ResetCamera();
        renderer->SetBackground(0.2, 0.2, 0.2);

        auto &interactor_style = _vtk_context.interactor_style;
        interactor_style = vtkSmartPointer<vtkInteractorStyleMultiTouchCamera>::New();
        interactor_style->SetDefaultRenderer(renderer);

        auto &interactor = _vtk_context.interactor;
        interactor = vtkSmartPointer<vtkGenericRenderWindowInteractor>::New();
        vtkSmartPointer<vtkCallbackCommand> createTimerCommand = vtkSmartPointer<vtkCallbackCommand>::New();
        createTimerCommand->SetCallback(VTKCreateTimerCallback);
        interactor->AddObserver(vtkCommand::CreateTimerEvent, createTimerCommand);
        interactor->SetInteractorStyle(interactor_style);
        interactor->EnableRenderOff();

        auto render_window = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
        render_window->SetSize(_vtk_context.vport_size);

        vtkSmartPointer<vtkCallbackCommand> isCurrentCallback =
                vtkSmartPointer<vtkCallbackCommand>::New();
        isCurrentCallback->SetCallback(VTKIsCurrentCallback);
        render_window->AddObserver(vtkCommand::WindowIsCurrentEvent, isCurrentCallback);

        render_window->SwapBuffersOn();
        render_window->UseOffScreenBuffersOff();
        render_window->SetFrameBlitModeToBlitToCurrent();
        render_window->AddRenderer(renderer);
        render_window->SetInteractor(interactor);

        for (auto &actor: actors_) {
            actor->GetProperty()->SetPointSize(imgui_vars_.point_size);
            actor->GetProperty()->SetLineWidth(imgui_vars_.line_width);
            renderer->AddActor(actor);
        }

        _vtk_context.render_window = render_window;
        _vtk_context.FBOId = 0;
        _vtk_context.RBOId = 0;
        _vtk_context.textureId = 0;
        _vtk_context.vport_size[0] = 640;
        _vtk_context.vport_size[1] = 480;
        _vtk_context.show_window = true;
        _vtk_context.is_initialized = true;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void VTKWindow::ProcessEvents() {
        if (!ImGui::IsWindowFocused())
            return;

        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        io.ConfigWindowsMoveFromTitleBarOnly = true;
        ImVec2 wPos = ImGui::GetWindowPos();
        // /!\ There is probably an error : The vtkInteractorStyleMultiTouchCamera moves twice too fast
        //     This is compensated here by dividing the Mouse Position by 2, But this is really ugly
        double xpos = (static_cast<double>(io.MousePos[0]) - static_cast<double>(wPos.x)) / 2;
        double ypos = (static_cast<double>(io.MousePos[1]) - static_cast<double>(wPos.y)) / 2;

        int ctrl = static_cast<int>(io.KeyCtrl);
        int shift = static_cast<int>(io.KeyShift);
        bool dclick = io.MouseDoubleClicked[0] || io.MouseDoubleClicked[1] || io.MouseDoubleClicked[2];
        _vtk_context.interactor->SetEventInformationFlipY(xpos, ypos, ctrl, shift, dclick);

        if (io.MouseClicked[ImGuiMouseButton_Left]) {
            _vtk_context.interactor->InvokeEvent(vtkCommand::LeftButtonPressEvent, nullptr);
        } else if (io.MouseReleased[ImGuiMouseButton_Left]) {
            _vtk_context.interactor->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, nullptr);
        } else if (io.MouseClicked[ImGuiMouseButton_Right])
            _vtk_context.interactor->InvokeEvent(vtkCommand::RightButtonPressEvent, nullptr);
        else if (io.MouseReleased[ImGuiMouseButton_Right])
            _vtk_context.interactor->InvokeEvent(vtkCommand::RightButtonReleaseEvent, nullptr);
        else if (io.MouseWheel > 0)
            _vtk_context.interactor->InvokeEvent(vtkCommand::MouseWheelForwardEvent, nullptr);
        else if (io.MouseWheel < 0)
            _vtk_context.interactor->InvokeEvent(vtkCommand::MouseWheelBackwardEvent, nullptr);

        _vtk_context.interactor->InvokeEvent(vtkCommand::MouseMoveEvent, nullptr);
        _vtk_context.interactor_style->OnTimer();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    VTKWindow::~VTKWindow() {
        _vtk_context.render_window = nullptr;
        _vtk_context.interactor = nullptr;
        _vtk_context.interactor_style = nullptr;
        _vtk_context.renderer = nullptr;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void VTKWindow::SetVPortSize(int w, int h) {
        CHECK(_vtk_context.render_window != nullptr) << "The VTK Window has not been initialized";
#if VTK_MAJOR_VERSION >= 9
        _vtk_context.render_window->SetShowWindow(_vtk_context.show_window);
#endif
        if (_vtk_context.vport_size[0] == w && _vtk_context.vport_size[1] == h)
            return;
        if (w == 0 || h == 0)
            return;

        _vtk_context.vport_size[0] = w;
        _vtk_context.vport_size[1] = h;

        // these cause a crash in glfwSwapBuffers()
        glGenTextures(1, &_vtk_context.textureId);
        glBindTexture(GL_TEXTURE_2D, _vtk_context.textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _vtk_context.vport_size[0], _vtk_context.vport_size[1],
                     0, GL_RGB, GL_UNSIGNED_BYTE, 0);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindTexture(GL_TEXTURE_2D, 0);

        glGenRenderbuffers(1, &_vtk_context.RBOId);
        glBindRenderbuffer(GL_RENDERBUFFER, _vtk_context.RBOId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                              _vtk_context.vport_size[0], _vtk_context.vport_size[1]);
        glBindRenderbuffer(GL_RENDERBUFFER, _vtk_context.RBOId);

        glGenFramebuffers(1, &_vtk_context.FBOId);
        glBindFramebuffer(GL_FRAMEBUFFER, _vtk_context.FBOId);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _vtk_context.textureId, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _vtk_context.RBOId);
        _vtk_context.render_window->InitializeFromCurrentContext();
        _vtk_context.render_window->SetSize(_vtk_context.vport_size);
        _vtk_context.interactor->UpdateSize(_vtk_context.vport_size[0], _vtk_context.vport_size[1]);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void VTKWindow::DrawImGUIContent() {
        if (!_vtk_context.is_initialized) {
            LOG(WARNING) << "";
            return;
        }

        {
            std::lock_guard<std::mutex> lockGuard(actors_management_mutex_);
            for (auto &actor: actors_to_remove_) {
                _vtk_context.renderer->RemoveActor(actor);
                actors_.erase(actor);
                actors_to_add_.erase(actor);
            }
            actors_to_remove_.clear();
        }

        {
            std::lock_guard<std::mutex> lockGuard(actors_management_mutex_);
            for (auto &actor: actors_to_add_) {
                _vtk_context.renderer->AddActor(actor);
                actors_.insert(actor);
            }
            actors_to_add_.clear();
        }

        DrawImGuiWindowConfigurations();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0., 0.));
        ImGui::BeginChild("##Viewport", ImVec2(0.f, 0.f), true,
                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        DrawVTKWindow();

        ProcessEvents();
        ImGui::Image((void *) _vtk_context.textureId,
                     ImGui::GetContentRegionAvail(),
                     ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();
        ImGui::PopStyleVar();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void VTKWindow::AddActor(vtkSmartPointer<vtkActor> actor) {
        std::lock_guard<std::mutex> lock(actors_management_mutex_);
        actor->GetProperty()->SetPointSize(imgui_vars_.point_size);
        actor->GetProperty()->SetLineWidth(imgui_vars_.line_width);
        actors_to_add_.insert(actor);
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void VTKWindow::RemoveActor(vtkSmartPointer<vtkActor> actor) {
        std::lock_guard<std::mutex> lock(actors_management_mutex_);
        if (_vtk_context.render_window != nullptr)
            actors_to_remove_.emplace(actor); //< We need to remove the actor from the window

    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void VTKWindow::VTKIsCurrentCallback(vtkObject *caller, unsigned long eventId, void *clientData, void *callData) {
        bool *isCurrent = static_cast<bool *>(callData);
        *isCurrent = true;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void VTKWindow::BackgroundPopup() {
        if (ImGui::Button("Background Color"))
            ImGui::OpenPopup("background_color");
        // Popup to select the Background color
        if (ImGui::BeginPopup("background_color")) {
            ImGui::Text("Background Color:");
            ImGui::Separator();
            ImVec2 button_size = ImVec2(
                    (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x) * 0.5f,
                    2 * ImGui::GetFontSize());

            static float color[3] = {0.f, 0.f, 0.f};
            ImGui::ColorPicker3("Background Color", color);
            ImGui::Separator();
            if (ImGui::Button("Apply", button_size)) {
                _vtk_context.renderer->SetBackground(color[0], color[1], color[2]);
            }
            ImGui::SameLine();
            if (ImGui::Button("Close", button_size))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void VTKWindow::ColorRangePopup() {
        if (ImGui::Button("Color Options"))
            ImGui::OpenPopup("color_options");

        if (ImGui::BeginPopup("color_options")) {
            static float scalar_range[2] = {0.f, 1.f};
            ImGui::InputFloat2("Scalar Range", scalar_range);
            ImVec2 button_size = ImVec2(
                    (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x) * 0.5f,
                    2 * ImGui::GetFontSize());

            auto collection = _vtk_context.renderer->GetActors();

            if (ImGui::Button("Set to Min-Max", ImVec2(button_size.x * 2, button_size.y))) {
                collection->InitTraversal();
                auto actor = collection->GetNextActor();
                double min_max[2] = {std::numeric_limits<float>::max(), std::numeric_limits<float>::min()};
                double actor_min_max[2];
                while (actor) {
                    auto scalars = actor->GetMapper()->GetInput()->GetPointData()->GetScalars();
                    if (scalars) {
                        scalars->GetRange(actor_min_max);
                        if (actor_min_max[0] < min_max[0])
                            min_max[0] = float(actor_min_max[0]);
                        if (actor_min_max[1] > min_max[0])
                            min_max[1] = float(actor_min_max[1]);
                    }
                    actor = collection->GetNextActor();
                }
                if (min_max[0] != std::numeric_limits<float>::max() &&
                    min_max[1] != std::numeric_limits<float>::min()) {
                    collection->InitTraversal();
                    actor = collection->GetNextActor();
                    while (actor) {
                        actor->GetMapper()->SetScalarRange(min_max);
                        actor = collection->GetNextActor();
                    }
                }
            }

            if (ImGui::Button("Apply", button_size)) {
                collection->InitTraversal();
                auto actor = collection->GetNextActor();
                while (actor) {
                    actor->GetMapper()->SetScalarRange(scalar_range[0], scalar_range[1]);
                    actor = collection->GetNextActor();
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Close", button_size))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void VTKWindow::RenderingPopup() {
        // Popup to select
        if (ImGui::Button("Rendering Options"))
            ImGui::OpenPopup("rendering_options");

        if (ImGui::BeginPopup("rendering_options")) {
            ImGui::Text("Rendering Options:");
            ImGui::Separator();

            ImGui::Checkbox("With EDL Shader", &imgui_vars_.with_edl_shader);
            ImGui::DragFloat("Point Size", &imgui_vars_.point_size, 0.2f, 1.0f, 20.0f);
            ImGui::DragFloat("Line Width", &imgui_vars_.line_width, 0.2f, 1.0f, 20.0f);

            ImVec2 button_size = ImVec2(
                    (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x) * 0.5f,
                    2 * ImGui::GetFontSize());
            if (ImGui::Button("Apply", button_size)) {

                _vtk_context.renderer->ReleaseGraphicsResources(_vtk_context.render_window);
                _vtk_context.renderer->SetRenderWindow(nullptr);

                if (imgui_vars_.with_edl_shader) {
                    auto basicPasses = vtkSmartPointer<vtkRenderStepsPass>::New();
                    auto edl = vtkSmartPointer<vtkEDLShading>::New();
                    edl->SetDelegatePass(basicPasses);
                    _vtk_context.renderer->SetPass(edl);
                } else {
                    auto basicPasses = vtkSmartPointer<vtkRenderStepsPass>::New();
                    _vtk_context.renderer->SetPass(basicPasses);
                }

                auto collection = _vtk_context.renderer->GetActors();
                collection->InitTraversal();
                auto actor = collection->GetNextActor();
                while (actor) {
                    actor->GetProperty()->SetPointSize(imgui_vars_.point_size);
                    actor->GetProperty()->SetLineWidth(imgui_vars_.line_width);
                    actor = collection->GetNextActor();
                }
                _vtk_context.renderer->SetRenderWindow(_vtk_context.render_window);
            }
            ImGui::SameLine();
            if (ImGui::Button("Close", button_size))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void VTKWindow::DrawImGuiWindowConfigurations() {
        // Setup the GUI Options
        if (ImGui::CollapsingHeader("Window Configuration")) {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0, 4.0));
            BackgroundPopup();
            ImGui::SameLine();
            RenderingPopup();
            ImGui::SameLine();
            ColorRangePopup();
            ImGui::PopStyleVar();
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void VTKWindow::DrawVTKWindow() {
        SetVPortSize(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _vtk_context.FBOId); // required since we set BlitToCurrent = On.
        _vtk_context.render_window->Render();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void VTKWindow::BeginContext() {
        if (_vtk_context.is_initialized)
            return;
        InitializeVTKContext();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void VTKWindow::EndContext() {
        glDeleteBuffers(1, &_vtk_context.FBOId);
        glDeleteBuffers(1, &_vtk_context.RBOId);
        glDeleteBuffers(1, &_vtk_context.textureId);

        _vtk_context.render_window = nullptr;
        _vtk_context.interactor = nullptr;
        _vtk_context.interactor_style = nullptr;
        _vtk_context.renderer = nullptr;
        _vtk_context.is_initialized = false;
        {
            std::lock_guard lock(actors_management_mutex_);
            actors_to_remove_.clear();
        }
    }


}