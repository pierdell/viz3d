#ifndef VIZ3D_CONFIG_H
#define VIZ3D_CONFIG_H

#include <yaml-cpp/yaml.h>
#include <mutex>

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

    // A Macro to declare a parameter inside a `ParamGroup`
#define PARAM_WITH_DEFAULT_VALUE(Type, name, label, description, default_value) \
    viz3d::Type name  = viz3d::Type (std::string(#name), label, std::string(description), std::move(default_value)); \
    private:                                                             \
    viz3d::ParamGroup::ParamHelper helper_ ## name ## _  = ParamGroup::ParamHelper(*this, name); \
    public:


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

    protected:
        // Registers the parameter to the registry
        virtual void RegisterParam(Param *param);;

        // Removes the parameter from the registry
        virtual void RemoveParam(Param *param);;

        std::map<std::string, struct Param *> param_registry;

        // Helper which registers a param to the ParamGroup at construction
        struct ParamHelper {
            ParamHelper(ParamGroup &group, Param &param);
        };
    };



    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// PARAMETERS DECLARATIONS
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define VIZ3D_DECLARE_SIMPLE_PARAM(TypeName, value_type) \
    struct TypeName : ValueParam< value_type > {         \
        using ValueParam< value_type >::ValueParam;      \
        using ValueParam<value_type>::operator=;         \
        using ValueParam::value_t;                       \
        void Draw() override;                            \
        void Save(YAML::Node &) const override;          \
        void Load(YAML::Node &) override;                \
    };

    VIZ3D_DECLARE_SIMPLE_PARAM(TextParam, std::string)

    /* -------------------------------------------------------------------------------------------------------------- */
    // Boolean Param
    VIZ3D_DECLARE_SIMPLE_PARAM(BoolParam, bool)


    template<>
    inline std::ostream &ValueParam<bool>::PrintValue(std::ostream &os) const {
        os << (value ? "true" : "false");
        return os;
    }

    /* -------------------------------------------------------------------------------------------------------------- */
    // Numeric Parameter
    VIZ3D_DECLARE_SIMPLE_PARAM(FloatParam, float)
    VIZ3D_DECLARE_SIMPLE_PARAM(IntParam, int)

#define SPECIALIZATION_PRINT_NUMERIC_VALUE(numeric_type)                                \
    template<>                                                                          \
    inline std::ostream &ValueParam<numeric_type>::PrintValue(std::ostream &os) const { \
        static_assert(std::is_arithmetic_v<numeric_type>, "Not an arithmetic type");    \
        os << std::to_string(value);                                                    \
        return os;                                                                      \
    };


    SPECIALIZATION_PRINT_NUMERIC_VALUE(int)

    SPECIALIZATION_PRINT_NUMERIC_VALUE(double)

    SPECIALIZATION_PRINT_NUMERIC_VALUE(float)

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

            ConfigForm() : ParamGroup("global_config", "Global Config") {};

            PARAM_WITH_DEFAULT_VALUE(TextParam, config_file_path, "Config File Path",
                                     "Path to the config for save/load operations",
                                     "viz3d.conf");
            PARAM_WITH_DEFAULT_VALUE(BoolParam, save_on_exit, "Save On Exit",
                                     "Whether to save the Config to disk on exit", true);

        } config_form;

        YAML::Node cached_node; //< A Cache for persisting operations before saving to disk
    };

}


#endif //VIZ3D_CONFIG_H
