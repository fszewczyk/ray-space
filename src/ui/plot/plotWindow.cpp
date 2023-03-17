#include "ui/plot/plotWindow.hpp"
#include "imgui.h"
#include "implot.h"

namespace shkyera {

plotWindow::plotWindow(std::shared_ptr<visibleWorld> world, std::shared_ptr<camera> cam)
    : m_topViewTab(world, cam, XZ), m_sideViewTab(world, cam, XY), m_frontViewTab(world, cam, YZ) {}

systemSettings plotWindow::render(bool &updated) {
    systemSettings settings;

    ImGui::Begin("Front view");
    settings = m_frontViewTab.render(updated);
    ImGui::End();

    ImGui::Begin("Side view");
    settings = m_sideViewTab.render(updated);
    ImGui::End();

    ImGui::Begin("Top view");
    settings = m_topViewTab.render(updated);
    ImGui::End();

    return settings;
}

} // namespace shkyera