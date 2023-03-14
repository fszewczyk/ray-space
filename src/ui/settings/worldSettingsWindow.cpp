#include "ui/settings/worldSettingsWindow.hpp"
#include "ui/ui.hpp"

#include "imgui.h"

namespace shkyera {

worldSettingsWindow::worldSettingsWindow(std::shared_ptr<visibleWorld> world) : m_world(world) {
    for (auto planet : world->getObjects()) {
        if (planet == world->getUniverse())
            continue;
        m_planetSettingsWindows.push_back(planetSettingsWindow(planet));
    }
}

worldSettings worldSettingsWindow::render(bool &updated) {
    worldSettings settings;
    settings.planets.resize(m_world->getObjects().size());
    settings.updatedPlanets.resize(m_world->getObjects().size());

    color ambientLightColor = m_world->getAmbientLightColor();
    float ambientLightColorComponents[3] = {static_cast<float>(ambientLightColor[0]),
                                            static_cast<float>(ambientLightColor[1]),
                                            static_cast<float>(ambientLightColor[2])};

    ImGui::Begin("World");
    ImGui::ColorEdit3("Ambient Light", ambientLightColorComponents);

    ImGui::Dummy(ImVec2(0.0f, 5.0f));

    for (size_t i = 0; i < m_planetSettingsWindows.size(); ++i) {
        bool updatedPlanet = false;
        settings.planets[i] = m_planetSettingsWindows[i].render(updatedPlanet);
        settings.updatedPlanets[i] = updatedPlanet;
        updated |= updatedPlanet;
    }

    ImGui::End();

    color newAmbientLightColor(ambientLightColorComponents[0], ambientLightColorComponents[1],
                               ambientLightColorComponents[2]);

    if (ambientLightColor != newAmbientLightColor) {
        updated = true;
    }

    settings.ambientColor = newAmbientLightColor;

    return settings;
}

} // namespace shkyera
