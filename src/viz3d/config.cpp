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
    void Param::DrawHover() const {
        ImGui::TextDisabled("(?)");
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(description.c_str());
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
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

    /* -------------------------------------------------------------------------------------------------------------- */
    ComboParam::ComboParam(std::string &&key, std::string &&label, std::string &&description,
                           std::vector<std::string> &&labels)
            : Param(std::move(key), std::move(label), std::move(description)),
              labels(std::move(labels)) {}

    /* -------------------------------------------------------------------------------------------------------------- */
    ComboParam::ComboParam(std::string &&key, std::string &&label, std::string &&description,
                           std::initializer_list<std::string> &&labels)
            : Param(std::move(key), std::move(label), std::move(description)),
              labels(labels) {}

    /* -------------------------------------------------------------------------------------------------------------- */
    void ComboParam::Draw() {
        DrawHover();
        ImGui::SameLine();
        if (labels.empty()) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6, 0., 0., 1.));
            ImGui::Text("/!\\ Empty Combo !");
            ImGui::PopStyleColor();
            return;
        }
        if (ImGui::BeginCombo(label.c_str(), labels[selected_idx % int(labels.size())].c_str())) {
            for (int i(0); i < labels.size(); i++) {
                const bool is_selected = (selected_idx == i);
                if (ImGui::Selectable(labels[i].c_str(), is_selected)) {
                    selected_idx = i;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void ComboParam::Load(YAML::Node &node) {
        if (node.IsMap() && node[key] && node[key].IsScalar()) {
            auto value = node[key].as<std::string>();
            auto it = std::find(labels.begin(), labels.end(), value);
            if (it != labels.end())
                selected_idx = int(std::distance(labels.begin(), it));
        }
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void ComboParam::Save(YAML::Node &node) const {
        if (selected_idx >= 0 && selected_idx < labels.size())
            node[key] = labels[selected_idx];
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    void ComboParam::PrintSelf(std::ostream &os) const {
        if (selected_idx >= 0 && selected_idx < labels.size())
            os << "[key: " << key << ", label: " << labels[selected_idx] << "]";
    }
} // namespace viz3d
