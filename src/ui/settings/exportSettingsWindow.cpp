#include "ui/settings/exportSettingsWindow.hpp"
#include "ui/ui.hpp"

#include "imgui.h"
#include "imgui_internal.h"

namespace shkyera {

exportSettingsWindow::exportSettingsWindow(std::shared_ptr<camera> cam) : m_camera(cam) {}

exportSettings exportSettingsWindow::render(RENDER_MODE &mode) {
    ImGui::Begin("Export");

    static exportSettings settings = getDefaultExportSettings();
    int setWidth = settings.width;
    int setHeight = settings.height;

    float aspectRatio = static_cast<float>(settings.width) / settings.height;

    if (ImGui::BeginCombo("Format", (" " + FILE_EXTENSIONS[settings.extension]).c_str())) {
        if (ImGui::Selectable("PNG", settings.extension == PNG))
            settings.extension = PNG;
        if (ImGui::Selectable("JPG", settings.extension == JPG))
            settings.extension = JPG;
        ImGui::EndCombo();
    }

    ImGui::Checkbox("Lock Aspect Ratio", &settings.lockAspectRatio);

    ImGui::SliderInt("Width", &setWidth, 128, 4096);
    ImGui::SliderInt("Height", &setHeight, 128, 4096);

    if (settings.lockAspectRatio) {
        if (setWidth != settings.width)
            setHeight = setWidth / aspectRatio;
        else if (setHeight != settings.height)
            setWidth = setHeight * aspectRatio;
    }

    settings.width = std::max(128, setWidth);
    settings.height = std::max(128, setHeight);

    ImGui::SliderInt("Ray Depth", &settings.maximumRayDepth, 2, 10);
    if (ImGui::IsItemHovered() && !ImGui::IsItemActive()) {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted(
            "Maximum number of bounces of every ray.\nUse large values only if your objects are very "
            "close to each other.");
        ImGui::EndTooltip();
    }

    ImGui::SliderInt("Rays Per Pixel", &settings.raysPerPixel, 10, 100);
    if (ImGui::IsItemHovered() && !ImGui::IsItemActive()) {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted("Rays shot out for every pixel.\nThe higher, the less noisy the image.\nUse high values "
                               "if you have relatively small light sources.");
        ImGui::EndTooltip();
    }

    ImGui::Dummy(ImVec2(0, 5.0f));

    if (mode == EDIT) {
        if (ImGui::Button("Start Preview", ImVec2(160, 0))) {
            mode = PREVIEW;
        }
    } else {
        if (ImGui::Button("Stop Preview", ImVec2(160, 0))) {
            mode = EDIT;
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Export", ImVec2(160, 0))) {
    }

    ImGui::End();

    return settings;
} // namespace shkyera

exportSettings exportSettingsWindow::getDefaultExportSettings() {
    exportSettings settings;
    settings.width = 1920;
    settings.height = 1080;
    settings.maximumRayDepth = 5;
    settings.raysPerPixel = 20;
    settings.extension = PNG;
    settings.lockAspectRatio = true;

    return settings;
}

} // namespace shkyera
