#ifndef VIZ3D_CONFIG_H
#define VIZ3D_CONFIG_H

#include <mutex>

#include <yaml-cpp/yaml.h>
#include <misc/cpp/imgui_stdlib.h>

namespace viz3d {

    /*!
     * @brief An Abstract Parameter of the config, which defines an editable ImGui component
     */
    struct Param {
        std::string key; //< Key - Id of the param in the param group (must be unique in a Param Group)
        std::string label; //< The label of the ImGui component
        std::string description; //< A Longer Description of the parameter (optional)

        // Draws the editable parameter with ImGui
        virtual void Draw() = 0;

        // Loads the parameter value from the YAML Node
        virtual void Load(YAML::Node &node) = 0;

        // Saves the parameter key-value in the YAML Node
        virtual void Save(YAML::Node &node) const = 0;

        // Prints the parameter in a human readable form
        virtual void PrintSelf(std::ostream &os) const = 0;

        // Prints the parameter to a stream
        friend std::ostream &operator<<(std::ostream &os, const Param &param);

        Param(std::string &&_key, std::string &&label, std::string &&_description) :
                key(std::move(_key)), label(std::move(label)), description(std::move(_description)) {}

    protected:

        void DrawHover() const;
    };

#define VIZ3D_REGISTER_PARAM(param_name) \
    private:                                                             \
        viz3d::ParamGroup::ParamHelper helper_ ## param_name ## _  = ParamGroup::ParamHelper(*this, param_name); \
    public:

    // A Macro to declare a parameter inside a `ParamGroup`
#define VIZ3D_PARAM_WITH_DEFAULT_VALUE(Type, name, label, description, default_value) \
    viz3d::Type name  = viz3d::Type (std::string(#name), label, std::string(description), std::move(default_value)); \
    VIZ3D_REGISTER_PARAM(name)


    /*!
     * @brief   A ParamGroup manages parameters as a group, drawing, saving and loading them together
     */
    struct ParamGroup {
        std::string param_group_id;
        std::string group_name;

        // Draws the Parameter Group
        virtual void Draw();

        // Saves the group to a YAML Node
        void Save(YAML::Node &node);

        // Loads the group from a YAML Node
        void Load(YAML::Node &node);

        ~ParamGroup();

        ParamGroup(std::string &&group_id, std::string &&_group_name);

        ParamGroup() = default;

    protected:
        friend class GlobalConfig;

        // Registers the parameter to the registry
        virtual void RegisterParam(Param *param);

        // Removes the parameter from the registry
        virtual void RemoveParam(Param *param);

        std::map<std::string, struct Param *> param_registry;

        // Helper which registers a param to the ParamGroup at construction
        struct ParamHelper {
            ParamHelper(ParamGroup &group, Param &param);
        };
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// PARAMETERS DEFINITIONS
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*!
     * @brief   A ComboParam which is a utility to define a multiple choice selection param
     */
    struct ComboParam : Param {

        explicit ComboParam(std::string &&key, std::string &&label,
                            std::string &&description, std::vector<std::string> &&labels);

        explicit ComboParam(std::string &&key, std::string &&label,
                            std::string &&description, std::initializer_list<std::string> &&labels);

        // Draws the editable parameter with ImGui
        void Draw() override;;

        // Loads the parameter value from the YAML Node
        void Load(YAML::Node &node) override;

        // Saves the parameter key-value in the YAML Node
        void Save(YAML::Node &node) const override;

        // Prints the parameter in a human readable form
        void PrintSelf(std::ostream &os) const override;

    protected:
        const std::vector<std::string> labels;
        int selected_idx = 0;
    };


    /*!
     * @brief   A Value parameter, is a simple extension of Param for simple Value types,
     *          Overloading convenient operators to easily manipulate the parameters
     */
    template<typename ValT>
    struct ValueParam : Param {
        typedef ValT value_t;
        ValT value;

        ValueParam(std::string &&name, std::string &&label, std::string &&description, const ValT &_value)
                : Param(std::move(name), std::move(label), std::move(description)) {
            value = _value;
        }

        ValueParam<ValT> &operator=(const ValT &new_value) {
            value = new_value;
            return *this;
        }

        std::ostream &PrintValue(std::ostream &os) const {
            os << value;
            return os;
        }

        void PrintSelf(std::ostream &os) const override {
            os << "[" << key << ": ";
            PrintValue(os) << value << "]";
        }
    };

    /*!
     * @brief   An array parameter handling multiple inputs simultaneously for some specific numeric types
     *          (see ImGui::InputFloat2 for e.g.)
     */
    template<typename ScalarT, int N>
    struct ArrayParam : Param {
        typedef std::array<ScalarT, N> value_t;
        typedef ScalarT scalar_t;
        static constexpr int dimension = N;
        value_t value;

        ArrayParam(std::string &&name, std::string &&label, std::string &&description, scalar_t _value)
                : Param(std::move(name), std::move(label), std::move(description)) {
            value.fill(_value);
            static_assert(std::is_arithmetic_v<ScalarT>,
                          "The type parameter is not compatible with ArrayParam (not supported by ImGui)");
        }

        inline void PrintSelf(std::ostream &os) const override {
            os << "[" << key << ": ";
            for (int i(0); i < N; ++i)
                os << value[i] << (i == N - 1 ? "" : ",");
            os << "]";
        }

    };


    /* -------------------------------------------------------------------------------------------------------------- */
    // MACRO TO DEFINE SIMPLE CLASSICAL INPUT VALUE PARAM
#define VIZ3D_DEFINE_VALUE_PARAM(TypeName, value_type, imgui_fun) \
    struct TypeName : ValueParam< value_type > {            \
        using ValueParam< value_type >::ValueParam;         \
        using ValueParam<value_type>::operator=;            \
        using ValueParam::value_t;                          \
        void Draw() override {                              \
            DrawHover(); ImGui::SameLine();                 \
            imgui_fun(label.c_str(), &value);               \
        };                                                  \
        void Save(YAML::Node &node) const override {node[key] = value;};    \
        void Load(YAML::Node &node) override{               \
             if (node.IsMap() && node[key]) {               \
                value = node[key].as<value_t>();            \
            }                                               \
        };                                                  \
    };

    VIZ3D_DEFINE_VALUE_PARAM(TextParam, std::string, ImGui::InputText)

    /* -------------------------------------------------------------------------------------------------------------- */
    // Boolean Param
    VIZ3D_DEFINE_VALUE_PARAM(BoolParam, bool, ImGui::Checkbox)

    template<>
    inline std::ostream &ValueParam<bool>::PrintValue(std::ostream &os) const {
        os << (value ? "true" : "false");
        return os;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    // Numeric Parameter
    VIZ3D_DEFINE_VALUE_PARAM(FloatParam, float, ImGui::InputFloat)
    VIZ3D_DEFINE_VALUE_PARAM(IntParam, int, ImGui::InputInt)

    /* -------------------------------------------------------------------------------------------------------------- */
    // MACRO TO DEFINE SCALAR ARRAY PARAMS
#define VIZ3D_DECLARE_ARRAY_PARAM(TypeName, scalar_type, dim, imgui_function) \
    struct TypeName : ArrayParam< scalar_type, dim > {              \
        using ArrayParam< scalar_type, dim >::ArrayParam;           \
        using ArrayParam< scalar_type, dim >::value_t;              \
        inline void Draw() override {                               \
            DrawHover(); ImGui::SameLine();                         \
            imgui_function(label.c_str(), value.data());            \
        };                                                          \
        inline void Save(YAML::Node & node) const override {        \
            YAML::Node array_node;                                  \
            for(int i(0);i<TypeName::dimension;++i)                 \
                array_node[i] = value[i];                           \
            node[key] = array_node;                                 \
        };                                                          \
        inline void Load(YAML::Node &node) override {               \
             if (node.IsMap() && node[key] &&                                       \
                 node[key].size() == dimension &&                   \
                 node[key].IsSequence()) {                          \
                auto array_node = node[key];                        \
                for(int i(0);i<TypeName::dimension;++i)                 \
                    value[i] = array_node[i].as<TypeName::scalar_t>();  \
            }                                                       \
        };                                                          \
    };


    VIZ3D_DECLARE_ARRAY_PARAM(IntArray2, int, 2, ImGui::InputInt2)
    VIZ3D_DECLARE_ARRAY_PARAM(IntArray3, int, 3, ImGui::InputInt3)
    VIZ3D_DECLARE_ARRAY_PARAM(IntArray4, int, 4, ImGui::InputInt4)

    VIZ3D_DECLARE_ARRAY_PARAM(FloatArray2, float, 2, ImGui::InputFloat2)
    VIZ3D_DECLARE_ARRAY_PARAM(FloatArray3, float, 3, ImGui::InputFloat3)
    VIZ3D_DECLARE_ARRAY_PARAM(FloatArray4, float, 4, ImGui::InputFloat4)


    /* -------------------------------------------------------------------------------------------------------------- */
    // MACRO TO DEFINE SLIDER SCALAR PARAMS

#define VIZ3D_DECLARE_VALUE_SLIDER_PARAM(Type, value_type, min_val, max_val, slider_type) \
    struct Type : ValueParam<value_type> {                                                          \
        using ValueParam<value_type>::ValueParam;                                                  \
        typedef value_type scalar_t;                                \
        static constexpr scalar_t min = min_val, max = max_val;     \
        inline void Draw() override {                               \
               DrawHover(); ImGui::SameLine();                      \
               slider_type(label.c_str(), &value, min, max);        \
        };                                                          \
        inline void Save(YAML::Node& node) const override{          \
            node[key] = value;                                      \
        }                                                           \
        inline void Load(YAML::Node& node) override {               \
             if (node.IsMap() && node[key] && node[key].IsScalar()) {   \
                    value = node[key].as<Type::scalar_t>();             \
            }                                                           \
        }                                                               \
    };

    VIZ3D_DECLARE_VALUE_SLIDER_PARAM(UnitIntSliderParam, int, 0, 1, ImGui::SliderInt);

    // MACRO TO DEFINE SLIDER SCALAR ARRAY PARAMS
#define VIZ3D_DEFINE_ARRAY_SLIDER_PARAM(Type, value_type, dim, min_val, max_val, slider_type) \
    struct Type : ArrayParam<value_type, dim> {                                                          \
        using  ArrayParam<value_type, dim>::ArrayParam;                                                  \
        typedef value_type scalar_t;                                \
        static constexpr scalar_t min = min_val, max = max_val;     \
        inline void Draw() override {                               \
               DrawHover(); ImGui::SameLine();                      \
               slider_type(label.c_str(), value.data(), min, max);        \
        };                                                                                               \
        inline void Save(YAML::Node& node) const override{     \
            YAML::Node array_node;                    \
            for(int i(0);i<Type::dimension;++i)       \
                array_node[i] = value[i];             \
            node[key] = array_node;                   \
        }                                             \
        inline void Load(YAML::Node& node) override { \
             if (node.IsMap() && node[key] &&         \
                 node[key].size() == dimension &&     \
                 node[key].IsSequence()) {            \
                auto array_node = node[key];          \
                for(int i(0);i<Type::dimension;++i)   \
                    value[i] = array_node[i].as<Type::scalar_t>(); \
            }                                         \
        }                                             \
    };


    VIZ3D_DEFINE_ARRAY_SLIDER_PARAM(UnitFloatConstrained3, float, 3, 0.f, 1.f, ImGui::SliderFloat3)


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// GLOBAL CONFIG
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*!
     * @brief   A Config is a singleton tracking all the ParamGroups created by the program
     *          To allow saving / loading configuration of ImGui parameter
     */
    class GlobalConfig {
    public:
        static GlobalConfig &Instance();

        // Loads the Config from a file saved to disk
        void LoadFromDisk(const std::string &file_path);

        // Loads the Config from a YAML::Node
        void LoadConfig(YAML::Node &node);

        // Saves the config to a YAML::Node
        void SaveConfig(YAML::Node &node);

        // Saves the config as a YAML file to disk
        void SaveToDisk(const std::string &file_path);

        // Saves the config as a YAML file to the determined location
        void Persist();

        // Registers a ParamGroup to the config
        void RegisterParamGroup(ParamGroup &group);

        // Removes a ParamGroup from the config
        void UnregisterGroup(ParamGroup &group);

        GlobalConfig(const GlobalConfig &) = delete;

        GlobalConfig(GlobalConfig &&) = delete;

    private:
        friend class GUI;
        friend class ParamGroup;

        GlobalConfig() = default;

        // Draws the config's own params group
        void DrawConfig();

        ~GlobalConfig();

        mutable std::mutex mutex;
        friend class ParamGroup;
        std::map<std::string, ParamGroup *> group_param_registry;

        struct ConfigForm : ParamGroup {

            ConfigForm();

            VIZ3D_PARAM_WITH_DEFAULT_VALUE(TextParam, config_file_path, "Config File Path",
                                           "Path to the config for save/load operations",
                                           "viz3d.conf");
            VIZ3D_PARAM_WITH_DEFAULT_VALUE(BoolParam, save_on_exit, "Save On Exit",
                                           "Whether to save the Config to disk on exit", true);

        } config_form;

        YAML::Node cached_node; //< A Cache for persisting operations before saving to disk
    };

}


#endif //VIZ3D_CONFIG_H
