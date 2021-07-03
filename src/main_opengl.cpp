#include "viz3d/engine.hpp"
#include "viz3d/model.hpp"

#include <thread>


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

int main(int argc, char **argv) {
    auto &engine = viz::ExplorationEngine::Instance();
    engine.Options().with_edl = false;
    bool display = true;
    auto window_ptr = std::make_shared<ImGUIDemo>("Dear ImGui Demo", &display);
    engine.AddWindow(window_ptr);

    // Add Models
    {
        auto model_ptr = std::make_shared<viz::PosesModel>();
        auto &model_data = model_ptr->ModelData();
        model_data.line_size = 5;
        engine.AddModel(0, model_ptr);
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

