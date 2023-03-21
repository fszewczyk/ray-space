#include "ui/plot/plotWindow.hpp"
#include "imgui.h"
#include "implot.h"

namespace shkyera {

plotWindow::plotWindow(std::shared_ptr<visibleWorld> world, std::shared_ptr<camera> cam)
    : m_topViewTab(world, cam, XZ), m_sideViewTab(world, cam, XY), m_frontViewTab(world, cam, YZ) {}

systemSettings plotWindow::render(bool &updated) {
    systemSettings settings;

    ImGui::Begin("Front view");
    systemSettings frontSettings = m_frontViewTab.render(updated);
    settings = frontSettings;
    ImGui::End();

    ImGui::Begin("Side view");
    bool sideUpdated = false;
    systemSettings sideSettings = m_sideViewTab.render(sideUpdated);
    if (sideUpdated)
        settings = sideSettings;
    ImGui::End();

    ImGui::Begin("Top view");
    bool topUpdated = false;
    systemSettings topSettings = m_topViewTab.render(topUpdated);
    if (topUpdated)
        settings = topSettings;
    ImGui::End();

    updated |= sideUpdated | topUpdated;

    return settings;
}

} // namespace shkyera