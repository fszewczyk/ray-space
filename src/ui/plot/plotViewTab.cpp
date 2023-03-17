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

        cameraSettings settings = m_camera->getSettings();

        float x;
        float y;
        switch (m_plane) {
        case XZ:
            x = -settings.origin[0];
            y = settings.origin[2];
            break;
        case XY:
            x = settings.origin[0];
            y = settings.origin[1];
            break;
        case YZ:
        default:
            x = settings.origin[2];
            y = settings.origin[1];
            break;
        }

        ImPlot::SetNextMarkerStyle(ImPlotMarker_Diamond, 8, ImVec4(0.3, 0.3, 0.3, 1), -1.0f, ImVec4(0.2, 0.2, 0.2, 1));
        ImPlot::PlotScatter("Camera", &x, &y, 1);

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

        auto legendObjects = objects;

        std::sort(legendObjects.begin(), legendObjects.end(),
                  [](const std::shared_ptr<sphere> &a, const std::shared_ptr<sphere> &b) -> bool {
                      return a->getName() < b->getName();
                  });

        // Ensures legend to be sorted by order. I apologize to anybody that sees this, I did not have a better idea.
        if (objects.size() > 1) {
            planetSettings settingsPlanet = objects[1]->getSettings();
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
            for (auto object : legendObjects) {
                if (object == m_world->getUniverse())
                    continue;

                ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 0);
                ImPlot::PlotScatter(object->getName().c_str(), &x, &y, 1);
            }
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