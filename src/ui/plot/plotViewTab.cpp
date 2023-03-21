#include <algorithm>
#include <functional>
#include <map>

#include "imgui.h"
#include "implot.h"
#include "ui/plot/plotViewTab.hpp"

namespace shkyera {

plotViewTab::plotViewTab(std::shared_ptr<visibleWorld> world, std::shared_ptr<camera> cam, PLANE plane)
    : m_world(world), m_camera(cam), m_plane(plane) {}

systemSettings plotViewTab::render(bool &updated) {
    worldSettings settingsWorld = m_world->getSettings();
    cameraSettings settingsCamera = m_camera->getSettings();
    systemSettings settingsPlot = {settingsWorld, settingsCamera};

    ImGui::Unindent();
    if (ImPlot::BeginPlot(("##" + std::to_string(m_plane)).c_str(),
                          ImVec2(ImGui::GetWindowWidth() - 10, ImGui::GetWindowHeight() - 40),
                          ImPlotFlags_Equal | ImPlotFlags_NoTitle | ImPlotFlags_NoMouseText)) {

        labelAxes();
        plotDummyPoints();
        settingsCamera = plotCamera();
        settingsWorld = plotPlanets();
        settingsCamera = plotCamera();

        ImPlot::EndPlot();
    }

    if (!(settingsCamera == m_camera->getSettings()))
        updated = true;

    for (auto wasUpdated : settingsWorld.updatedPlanets)
        updated |= wasUpdated;

    settingsPlot.cam = settingsCamera;
    settingsPlot.world = settingsWorld;

    return settingsPlot;
}

void plotViewTab::labelAxes() {
    switch (m_plane) {
    case XZ:
        ImPlot::SetupAxes("X", "Z", ImPlotAxisFlags_Invert | ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks,
                          ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks);
        break;
    case XY:
        ImPlot::SetupAxes("X", "Y", ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks,
                          ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks);
        break;
    case YZ:
        ImPlot::SetupAxes("Z", "Y", ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks,
                          ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_NoTickMarks);
        break;
    }
}

void plotViewTab::plotDummyPoints() {
    auto objects = m_world->getObjects();

    std::sort(objects.begin(), objects.end(),
              [](const std::shared_ptr<sphere> &a, const std::shared_ptr<sphere> &b) -> bool {
                  return a->getName() < b->getName();
              });

    // Ensures legend to be sorted in lexicographical order by putting dummy points.
    // My sincere apologies to anybody that reads this, I did not have a better idea.
    if (objects.size() > 1) {
        planetSettings settingsPlanet = objects[1]->getSettings();
        auto [x, y] = getPositionOnPlane(settingsPlanet.origin);
        for (auto object : objects) {
            if (object == m_world->getUniverse())
                continue;

            color objectColor = object->getMaterial()->getVisibleColor();

            ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 0,
                                       ImVec4(objectColor[0], objectColor[1], objectColor[2], 1), -1.0,
                                       ImVec4(objectColor[0], objectColor[1], objectColor[2], 1));
            ImPlot::PlotScatter(object->getName().c_str(), &x, &y, 1);
        }
    }
}

cameraSettings plotViewTab::plotCamera() {
    cameraSettings settingsCamera = m_camera->getSettings();

    auto [x, y] = getPositionOnPlane(settingsCamera.origin);

    ImPlot::SetNextMarkerStyle(ImPlotMarker_Diamond, 8, ImVec4(0.3, 0.3, 0.3, 1), -1.0f, ImVec4(0.2, 0.2, 0.2, 1));
    ImPlot::DragPoint(std::hash<std::string>{}("Camera"), &x, &y, ImVec4(0.3, 0.3, 0.3, 1), 7);

    settingsCamera.origin = getPointFromPlane(settingsCamera.origin, x, y);

    return settingsCamera;
}

worldSettings plotViewTab::plotPlanets() {
    worldSettings settingsWorld = m_world->getSettings();

    auto objectsByPosition = m_world->getObjects();
    auto objectsBySize = objectsByPosition;

    std::map<shared_ptr<sphere>, size_t> originalPlanetIndices;
    for (size_t i = 1; i < objectsBySize.size(); ++i) {
        originalPlanetIndices.insert(std::make_pair(objectsBySize[i], i - 1));
    }

    std::sort(objectsBySize.begin(), objectsBySize.end(),
              [](const std::shared_ptr<sphere> &a, const std::shared_ptr<sphere> &b) -> bool {
                  return a->getRadius() < b->getRadius();
              });

    switch (m_plane) {
    case XZ:
        std::sort(objectsByPosition.begin(), objectsByPosition.end(),
                  [](const std::shared_ptr<sphere> &a, const std::shared_ptr<sphere> &b) -> bool {
                      return a->getOrigin()[1] < b->getOrigin()[1];
                  });
        break;
    case XY:
        std::sort(objectsByPosition.begin(), objectsByPosition.end(),
                  [](const std::shared_ptr<sphere> &a, const std::shared_ptr<sphere> &b) -> bool {
                      return a->getOrigin()[2] < b->getOrigin()[2];
                  });
        break;
    case YZ:
    default:
        std::sort(objectsByPosition.begin(), objectsByPosition.end(),
                  [](const std::shared_ptr<sphere> &a, const std::shared_ptr<sphere> &b) -> bool {
                      return a->getOrigin()[0] < b->getOrigin()[0];
                  });
        break;
    }

    for (auto object : objectsBySize) {
        if (object == m_world->getUniverse())
            continue;

        planetSettings settingsPlanet = object->getSettings();
        auto [x, y] = getPositionOnPlane(settingsPlanet.origin);

        color objectColor = object->getMaterial()->getVisibleColor();

        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle,
                                   settingsPlanet.radius / ImPlot::GetPlotLimits().X.Size() *
                                       (0.95 * ImGui::GetWindowWidth()),
                                   ImVec4(objectColor[0], objectColor[1], objectColor[2], 1), -1.0,
                                   ImVec4(objectColor[0], objectColor[1], objectColor[2], 1));

        ImPlot::DragPoint(std::hash<std::string>{}(settingsPlanet.name), &x, &y,
                          ImVec4(objectColor[0], objectColor[1], objectColor[2], 1),
                          settingsPlanet.radius / ImPlot::GetPlotLimits().X.Size() * (0.95 * ImGui::GetWindowWidth()));
    }

    for (auto object : objectsByPosition) {
        if (object == m_world->getUniverse())
            continue;

        planetSettings settingsPlanet = object->getSettings();

        auto [x, y] = getPositionOnPlane(settingsPlanet.origin);

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

        ImPlot::DragPoint(std::hash<std::string>{}(settingsPlanet.name), &x, &y,
                          ImVec4(objectColor[0], objectColor[1], objectColor[2], 1),
                          settingsPlanet.radius / ImPlot::GetPlotLimits().X.Size() * (0.95 * ImGui::GetWindowWidth()));

        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 0);
        ImPlot::PlotScatter(settingsPlanet.name.c_str(), xBorders, yBorders, 4);

        settingsPlanet.origin = getPointFromPlane(settingsPlanet.origin, x, y);

        if (settingsPlanet.origin != object->getSettings().origin) {
            settingsWorld.planets[originalPlanetIndices[object]] = settingsPlanet;
            settingsWorld.updatedPlanets[originalPlanetIndices[object]] = true;
        }
    }

    return settingsWorld;
}

std::pair<double, double> plotViewTab::getPositionOnPlane(point3 point) {
    double x;
    double y;
    switch (m_plane) {
    case XZ:
        x = point[0];
        y = point[2];
        break;
    case XY:
        x = point[0];
        y = point[1];
        break;
    case YZ:
    default:
        x = point[2];
        y = point[1];
        break;
    }

    return {x, y};
}

point3 plotViewTab::getPointFromPlane(point3 point, float x, float y) {
    switch (m_plane) {
    case XZ:
        point[0] = x;
        point[2] = y;
        break;
    case XY:
        point[0] = x;
        point[1] = y;
        break;
    case YZ:
    default:
        point[2] = x;
        point[1] = y;
        break;
    }

    return point;
}

} // namespace shkyera