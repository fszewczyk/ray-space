#include "ui/plot/plotWindow.hpp"
#include "imgui.h"

namespace shkyera {

plotWindow::plotWindow(std::shared_ptr<visibleWorld> world, std::shared_ptr<camera> cam)
    : m_world(world), m_camera(cam) {}

systemSettings plotWindow::render(bool &updated) {
    worldSettings settingsWorld;
    cameraSettings settingsCamera = m_camera->getSettings();
    systemSettings settingsPlot = {settingsWorld, settingsCamera};

    ImGui::Begin("Planetary System");

    ImGui::End();

    return settingsPlot;
}

} // namespace shkyera