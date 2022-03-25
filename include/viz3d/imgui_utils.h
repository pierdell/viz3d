#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
#ifndef VIZ3D_UTILS_H
#define VIZ3D_UTILS_H

#include <imgui.h>

namespace viz3d {


    // A Centered text box
    inline void ImGui_CenteredText(const char *text) {
        auto size = ImGui::CalcTextSize(text).x;
        auto width = ImGui::GetWindowSize().x;
        ImGui::SetCursorPosX((width - size) * 0.5f);
        ImGui::Text(text);
    }

    // A Button of width equal to the whole available width
    inline ImVec2 ImGui_HorizontalButtonSize(float window_ratio = 1.f) {
        return ImVec2((ImGui::GetContentRegionAvail().x) * window_ratio,
                      2 * ImGui::GetFontSize());
    };

    // A Button of width equal to the whole available width
    inline bool ImGui_HorizontalButton(const char *button_label,
                                       float window_ratio = 1.f) {
        auto button_size = ImGui_HorizontalButtonSize(window_ratio);
        return ImGui::Button(button_label, button_size);
    }


}

#endif //VIZ3D_UTILS_H

#pragma clang diagnostic pop