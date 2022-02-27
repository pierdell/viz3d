#include "../../include/viz3d/engine.h"
#include "../../include/viz3d/model.h"

#include <thread>
#include <imgui.h>
#include <implot.h>


namespace ImGui {
    void ShowDemoWindow(bool *p_open);
}

class ImGUIDemo : public viz::ExplorationEngine::GUIWindow {
public:
    ~ImGUIDemo() = default;

    ImGUIDemo(std::string &&winname, bool *display) : viz::ExplorationEngine::GUIWindow(std::move(winname), display) {}

    void DrawContent() override {
        ImGui::ShowDemoWindow(display_window_);
    };

};

class ImplotDemo : public viz::ExplorationEngine::GUIWindow {
public:
    ~ImplotDemo() = default;

    ImplotDemo(std::string &&winname, bool *display) : viz::ExplorationEngine::GUIWindow(std::move(winname), display) {}

    void DrawContent() override {
        ImPlot::ShowDemoWindow(display_window_);
    };
};


int main(int argc, char **argv) {
    auto &engine = viz::ExplorationEngine::Instance();
    engine.Options().with_edl = false;
    bool display = true;
    auto window_ptr = std::make_shared<ImGUIDemo>("Dear ImGui Demo", &display);
    auto implot_window_ptr = std::make_shared<ImplotDemo>("Implot Demo", &display);
    engine.AddWindow(implot_window_ptr);
    engine.AddWindow(window_ptr);

    // Add Models
    {
        auto model_ptr = std::make_shared<viz::PosesModel>();
        auto &model_data = model_ptr->ModelData();
        model_data.line_size = 5;
        engine.AddModel(0, model_ptr);
    }

    {
        auto model_ptr = std::make_shared<viz::PointCloudModel>();
        auto &model_data = model_ptr->ModelData();
        model_data.default_color = Eigen::Vector3f(1.0, 0.0, 0.0);
        model_data.xyz.resize(10000);
        model_data.point_size = 3;
        for (auto &point: model_data.xyz) {
            point = Eigen::Vector3f::Random() + Eigen::Vector3f(0, 0.0, -2.0);
        }
        engine.AddModel(-1, model_ptr);
    }

    {
        auto model_ptr = std::make_shared<viz::PosesModel>();
        auto &model_data = model_ptr->ModelData();
        model_data.instance_model_to_world[0](0, 3) += 3.0;
        model_data.scaling = 0.5;
        model_data.line_size = 3;
        model_data.with_default_color = true;
        engine.AddModel(1, model_ptr);
    }

    // Launches MainLoop
    std::thread t1{viz::ExplorationEngine::LaunchMainLoop};
    t1.join();

    return 0;
}

