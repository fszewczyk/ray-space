#include <algorithm>

#include "imgui.h"
#include "implot.h"
#include "ui/plot/plotViewTab.hpp"

namespace shkyera {

plotViewTab::plotViewTab(std::shared_ptr<visibleWorld> world, std::shared_ptr<camera> cam, PLANE plane)
    : m_world(world), m_camera(cam), m_plane(plane) {}

systemSettings plotViewTab::render(bool &updated) {
    worldSettings settingsWorld;
    cameraSettings settingsCamera = m_camera->getSettings();
    systemSettings settingsPlot = {settingsWorld, settingsCamera};

    ImGui::Unindent();
    if (ImPlot::BeginPlot(("##" + std::to_string(m_plane)).c_str(),
                          ImVec2(ImGui::GetWindowWidth() - 10, ImGui::GetWindowHeight() - 40),
                          ImPlotFlags_Equal | ImPlotFlags_NoTitle | ImPlotFlags_NoMouseText)) {
        ImPlot::SetupAxes(0, 0, ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks,
                          ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks);

        auto objects = m_world->getObjects();

        switch (m_plane) {
        case XZ:
            std::sort(objects.begin(), objects.end(),
                      [](const std::shared_ptr<sphere> &a, const std::shared_ptr<sphere> &b) -> bool {
                          return a->getOrigin()[1] < b->getOrigin()[1];
                      });
            break;
        case XY:
            std::sort(objects.begin(), objects.end(),
                      [](const std::shared_ptr<sphere> &a, const std::shared_ptr<sphere> &b) -> bool {
                          return a->getOrigin()[2] < b->getOrigin()[2];
                      });
            break;
        case YZ:
        default:
            std::sort(objects.begin(), objects.end(),
                      [](const std::shared_ptr<sphere> &a, const std::shared_ptr<sphere> &b) -> bool {
                          return a->getOrigin()[0] < b->getOrigin()[0];
                      });
            break;
        }
        for (auto object : objects) {
            if (object == m_world->getUniverse())
                continue;

            planetSettings settingsPlanet = object->getSettings();

            float x;
            float y;
            switch (m_plane) {
            case XZ:
                x = -settingsPlanet.origin[0];
                y = settingsPlanet.origin[2];
                break;
            case XY:
                x = settingsPlanet.origin[0];
                y = settingsPlanet.origin[1];
                break;
            case YZ:
            default:
                x = settingsPlanet.origin[2];
                y = settingsPlanet.origin[1];
                break;
            }

            float lx = x - settingsPlanet.radius - 5;
            float ly = y - settingsPlanet.radius - 5;
            float tx = x + settingsPlanet.radius + 5;
            float ty = y + settingsPlanet.radius + 5;

            float xBorders[4] = {lx, lx, tx, tx};
            float yBorders[4] = {ly, ty, ly, ty};

            color objectColor = object->getMaterial()->getVisibleColor();

            ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle,
                                       settingsPlanet.radius / ImPlot::GetPlotLimits().X.Size() *
                                           (0.95 * ImGui::GetWindowWidth()),
                                       ImVec4(objectColor[0], objectColor[1], objectColor[2], 1), -1.0,
                                       ImVec4(objectColor[0], objectColor[1], objectColor[2], 1));
            ImPlot::PlotScatter(settingsPlanet.name.c_str(), &x, &y, 1);

            ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 0);
            ImPlot::PlotScatter(settingsPlanet.name.c_str(), xBorders, yBorders, 4);
        }

        ImPlot::EndPlot();
    }

    return settingsPlot;
}

} // namespace shkyera