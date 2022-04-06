#include <gtest/gtest.h>

#include <list>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <glog/logging.h>

#include <viz3d/config.h>

struct Form : viz3d::ParamGroup {
    using viz3d::ParamGroup::ParamGroup;
    VIZ3D_PARAM_WITH_DEFAULT_VALUE(IntParam, x, "x", "The value x", 1)
};

// Config
TEST(Config, config) {
    Form form("First Form", "Param Group");
    form.x = 42;
    auto& instance = viz3d::GlobalConfig::Instance();
    instance.Persist();
}