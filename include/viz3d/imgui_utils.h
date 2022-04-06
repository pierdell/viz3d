#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
#ifndef VIZ3D_UTILS_H
#define VIZ3D_UTILS_H

#include <string>
#include <vtkLookupTable.h>

#include <imgui.h>
#include "config.h"

namespace viz3d {


    // A Centered text box
    inline void ImGui_CenteredText(const char *text) {
        auto size = ImGui::CalcTextSize(text).x;
        auto width = ImGui::GetWindowSize().x;
        ImGui::SetCursorPosX((width - size) * 0.5f);
        ImGui::Text("%s", text);
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

    enum VTKColorMapType {
        JET,
        MAGMA,
        PLASMA,
        INFERNO,
        VIRIDIS
    };

    /*!
     * @brief   Returns a reference to a vector of color map names
     */
    const std::vector<std::pair<std::string, VTKColorMapType>> &ColorMapPairs();

    /*!
     * @brief   Returns a vector to the ColorMap Labels
     */
    std::vector<std::string> ColorMapLabels();

    /*!
     * @brief   Returns a vtkLookupTable defining the specified colormap
     */
    vtkSmartPointer<vtkLookupTable> ColorMap(VTKColorMapType type = VIRIDIS);

    // A Simple utility function to select a vtk color map via an ImGui Combo
    struct ImGui_ColorMapCombo : ComboParam {

        inline VTKColorMapType GetSelectedColorMapType() const;

        inline vtkSmartPointer<vtkLookupTable> GetSelectedColorMap() const;

        explicit ImGui_ColorMapCombo(std::string &&window_id);
    };


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// IMPLEMENTATIONS
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    VTKColorMapType ImGui_ColorMapCombo::GetSelectedColorMapType() const {
        return ColorMapPairs()[selected_idx].second;
    }

    vtkSmartPointer<vtkLookupTable> ImGui_ColorMapCombo::GetSelectedColorMap() const {
        return ColorMap(GetSelectedColorMapType());
    }

}

#endif //VIZ3D_UTILS_H

#pragma clang diagnostic pop