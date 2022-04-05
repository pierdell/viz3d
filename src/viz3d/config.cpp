#include <filesystem>
#include <fstream>

#include <glog/logging.h>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "viz3d/config.h"

namespace fs = std::filesystem;

namespace viz3d {

    /* -------------------------------------------------------------------------------------------------------------- */
    std::ostream &operator<<(std::ostream &os, const Param &param) {
        param.PrintSelf(os);
        return os;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void ParamGroup::Draw() {
        ImGui::Separator();
        ImGui::Text("Param Group: %s", group_name.c_str());
        for (auto &param_pair: param_registry)
            param_pair.second->Draw();
        ImGui::Separator();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void ParamGroup::Save(YAML::Node &node) {
        YAML::Node param_node;
        for (auto &param_pair: param_registry) {
            if (param_pair.second)
                param_pair.second->Save(param_node);
        }
        node[param_group_id] = param_node;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void ParamGroup::Load(YAML::Node &node) {
        if (node[param_group_id]) {
            auto param_node = node[param_group_id];
            if (node.IsMap()) {
                for (auto &param_pair: param_registry) {
                    if (param_pair.second)
                        param_pair.second->Load(param_node);
                }
            }
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    ParamGroup::~ParamGroup() {
        auto &instance = GlobalConfig::Instance();
        instance.UnregisterGroup(*this);
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void ParamGroup::RemoveParam(Param *param) {
        if (param && param_registry.find(param->key) != param_registry.end())
            param_registry.erase(param->key);
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void ParamGroup::RegisterParam(Param *param) { if (param) param_registry.emplace(param->key, param); }

    /* -------------------------------------------------------------------------------------------------------------- */
    ParamGroup::ParamGroup(std::string &&group_id, std::string &&_group_name)
            : param_group_id(std::move(group_id)), group_name(std::move(_group_name)) {
        auto &instance = GlobalConfig::Instance();
        instance.RegisterParamGroup(*this);
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    ParamGroup::ParamHelper::ParamHelper(ParamGroup &group, Param &param) {
        group.RegisterParam(&param);
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    GlobalConfig &GlobalConfig::Instance() {
        static GlobalConfig config;
        static std::atomic<bool> is_initialized = false;
        if (!is_initialized) {
            is_initialized = true;
            // Initialize the config
            config.RegisterParamGroup(config.config_form);
        }
        return config;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void GlobalConfig::LoadFromDisk(const std::string &file_path) {
        if (!fs::exists(file_path)) {
            LOG(WARNING) << "The file `" << file_path << "` does not exist on disk" << std::endl;
            return;
        }

        YAML::Node node = YAML::LoadFile(file_path);
        LoadConfig(node);

    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void GlobalConfig::LoadConfig(YAML::Node &node) {
        std::lock_guard<std::mutex> lock{mutex};

        for (auto &group: group_param_registry)
            group.second->Load(node);
        cached_node = YAML::Node(node);
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void GlobalConfig::SaveConfig(YAML::Node &node) {
        std::lock_guard<std::mutex> lock{mutex};

        for (auto &group: group_param_registry)
            group.second->Save(node);
        cached_node = YAML::Node(node);
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void GlobalConfig::SaveToDisk(const std::string &file_path) {
        if (file_path.empty()) {
            LOG(ERROR) << "The file path specified is empty" << file_path;
            return;
        }

        YAML::Node node;
        SaveConfig(node);
        try {
            std::ofstream os(file_path);
            os << node;
        } catch (...) {
            LOG(ERROR) << "Could not save config to path " << file_path;
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void GlobalConfig::RegisterParamGroup(ParamGroup &group) {
        std::lock_guard<std::mutex> lock{mutex};
        CHECK(group_param_registry.find(group.param_group_id) ==
              group_param_registry.end())
                        << "The group `" << group.param_group_id << "` is already registered !";
        group_param_registry.emplace(group.param_group_id, &group);
        group.Load(cached_node); // Ensure that new window registered load the cached config
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void GlobalConfig::UnregisterGroup(ParamGroup &group) {
        std::lock_guard<std::mutex> lock{mutex};
        if (group_param_registry.find(group.param_group_id) != group_param_registry.end())
            group_param_registry.erase(group.param_group_id);
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void GlobalConfig::DrawConfig() {
        config_form.Draw();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    GlobalConfig::~GlobalConfig() {}

    /* -------------------------------------------------------------------------------------------------------------- */
    void GlobalConfig::Persist() {
        if (!config_form.config_file_path.value.empty())
            SaveToDisk(config_form.config_file_path.value);
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    GlobalConfig::ConfigForm::ConfigForm() {
        param_group_id = "global_config";
        group_name = "Global Config";
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// PARAMETERS IMPLEMENTATIONS
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define VIZ3D_VALUE_PARAM_SAVE_AND_LOAD(Type)         \
    void Type::Save(YAML::Node& node) const {   \
        node[key] = value;                      \
    }                                           \
    void Type::Load(YAML::Node& node) {         \
         if (node[key]) {                       \
            value = node[key].as<value_t>();    \
        }                                       \
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    // TextParam
    VIZ3D_VALUE_PARAM_SAVE_AND_LOAD(TextParam)

    void TextParam::Draw() {
        ImGui::InputText(label.c_str(), &value);
        DrawHover();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    // IntParam
    VIZ3D_VALUE_PARAM_SAVE_AND_LOAD(IntParam)

    void IntParam::Draw() {
        ImGui::InputInt(label.c_str(), &value);
        DrawHover();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    // FloatParam
    VIZ3D_VALUE_PARAM_SAVE_AND_LOAD(FloatParam)

    void FloatParam::Draw() {
        ImGui::InputFloat(label.c_str(), &value);
        DrawHover();
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    // Bool Param
    VIZ3D_VALUE_PARAM_SAVE_AND_LOAD(BoolParam)

    void BoolParam::Draw() {
        ImGui::Checkbox(label.c_str(), &value);
        DrawHover();
    }


} // namespace viz3d
