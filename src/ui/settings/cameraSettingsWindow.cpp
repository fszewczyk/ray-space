#include "ui/settings/cameraSettingsWindow.hpp"

#include "imgui.h"

namespace shkyera {

cameraSettingsWindow::cameraSettingsWindow(std::shared_ptr<camera> cam)
    : m_camera(cam) {}

point3 cameraSettingsWindow::render(bool &updated) {
    static float f = 0.0f;
    static int counter = 0;
    ImGui::Begin("Camera"); // Create a window called
    if (ImGui::Button("Reset"))
        updated = true;

    static vec3 cameraPosition = m_camera->getPosition();

    static float cameraPositionX = cameraPosition[0];
    static float cameraPositionY = cameraPosition[1];
    static float cameraPositionZ = cameraPosition[2];

    if (ImGui::SliderFloat("Position X", &cameraPositionX, -30.0f, 30.0f,
                           "ratio = %.3f"))
        updated = true;

    if (ImGui::SliderFloat("Position Y", &cameraPositionY, -30.0f, 30.0f,
                           "ratio = %.3f"))
        updated = true;

    if (ImGui::SliderFloat("Position Z", &cameraPositionZ, -30.0f, 30.0f,
                           "ratio = %.3f"))
        updated = true;

    ImGui::End();

    return point3(cameraPositionX, cameraPositionY, cameraPositionZ);
}

} // namespace shkyera
