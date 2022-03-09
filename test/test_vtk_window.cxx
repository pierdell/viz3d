#include <gtest/gtest.h>

#include <thread>
#include <chrono>

#include <viz3d/ui.h>
#include <viz3d/vtk_window.h>

#include "test_utils.h"


// A Test Which removes a window at runtime
TEST(VTKWindow, RemoveActor) {

    auto &gui = viz3d::GUI::Instance();
    size_t window_id;
    auto vtk_window2 = std::make_shared<viz3d::VTKWindow>("VTK Window 2");
    vtk_window2->AddActor(GetPointCloudActor());
    auto actor_to_remove = GetLineActor();
    vtk_window2->AddActor(actor_to_remove);
    window_id = gui.AddWindow(vtk_window2);

    std::thread gui_thread{viz3d::GUI::LaunchMainLoop, "GUI"};
    std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<1, 1>>(3.));

    vtk_window2->RemoveActor(actor_to_remove);

    std::cout << "Line Actor Removed" << std::endl;

    gui_thread.join();
}