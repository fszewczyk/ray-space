#include "ui/settings/cameraSettingsWindow.hpp"

#include "imgui.h"

namespace shkyera {

cameraSettingsWindow::cameraSettingsWindow(std::shared_ptr<camera> cam)
    : m_camera(cam) {}

void cameraSettingsWindow::render(bool &updated) {
    static float f = 0.0f;
    static int counter = 0;
    ImGui::Begin("Camera"); // Create a window called
    if (ImGui::Button("Reset"))
        updated = true;

    static vec3 cameraPosition = m_camera->getPosition();

    static float cameraPositionX = cameraPosition[0];
    static float cameraPositionY = cameraPosition[1];
    static float cameraPositionZ = cameraPosition[2];

    if (ImGui::SliderFloat("Position X", &cameraPositionX, -10.0f, 10.0f,
                           "ratio = %.3f"))
        updated = true;

    if (ImGui::SliderFloat("Position Y", &cameraPositionY, -10.0f, 10.0f,
                           "ratio = %.3f"))
        updated = true;

    if (ImGui::SliderFloat("Position Z", &cameraPositionZ, -10.0f, 10.0f,
                           "ratio = %.3f"))
        updated = true;

    m_camera->setPosition(
        point3(cameraPositionX, cameraPositionY, cameraPositionZ));

    ImGui::End();
}

} // namespace shkyera
