#include "ui/settings/cameraSettingsWindow.hpp"
#include "ui/ui.hpp"

#include "imgui.h"

namespace shkyera {

cameraSettingsWindow::cameraSettingsWindow(std::shared_ptr<camera> cam) : m_camera(cam) {}

cameraSettings cameraSettingsWindow::render(bool &updated) {
    static float f = 0.0f;
    static int counter = 0;

    cameraSettings settings = m_camera->getSettings();

    float positionX = -settings.origin[0];
    float positionY = settings.origin[1];
    float positionZ = settings.origin[2];

    float depthOfField = settings.depthOfField;
    float fieldOfView = settings.fieldOfView;
    float focusDistance = settings.focusDistance;

    ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_NoScrollWithMouse);

    ImGui::PushFont(ui::BOLD_FONT);
    ImGui::Text("Position");
    ImGui::PopFont();
    ImGui::SliderFloat("X", &positionX, -30.0f, 30.0f, "%.2f");
    ImGui::SliderFloat("Y", &positionY, -30.0f, 30.0f, "%.2f");
    ImGui::SliderFloat("Z", &positionZ, -30.0f, 30.0f, "%.2f");

    ImGui::PushFont(ui::BOLD_FONT);
    ImGui::Text("Visual");
    ImGui::PopFont();
    ImGui::SliderFloat("Field of View", &fieldOfView, 10.0f, 140.0f, "%.2f");
    if (ImGui::IsItemHovered() && !ImGui::IsItemActive()) {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted("The bigger, the more the camera sees.");
        ImGui::EndTooltip();
    }

    ImGui::SliderFloat("Depth of Field", &depthOfField, 0.0f, 1.0f, "%.2f");
    if (ImGui::IsItemHovered() && !ImGui::IsItemActive()) {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted("Size of the camera aperture.\nUse in combination with Focus Distance.");
        ImGui::EndTooltip();
    }

    ImGui::SliderFloat("Focus Distance", &focusDistance, 0.5f, 30.0f, "%.2f");
    if (ImGui::IsItemHovered() && !ImGui::IsItemActive()) {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted("Camera will be focused at the specified distance.\nEffect only visible if Depth of "
                               "Field is larger than 0.");
        ImGui::EndTooltip();
    }

    ImGui::End();

    settings.origin[0] = -positionX;
    settings.origin[1] = positionY;
    settings.origin[2] = positionZ;

    settings.depthOfField = depthOfField;
    settings.fieldOfView = fieldOfView;
    settings.focusDistance = focusDistance;

    if (!(settings == m_camera->getSettings()))
        updated = true;

    return settings;
}

} // namespace shkyera
