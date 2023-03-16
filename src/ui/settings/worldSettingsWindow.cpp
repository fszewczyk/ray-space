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

    ImGui::Dummy(ImVec2(0.0f, 5.0f));

    static const char *sampledPlanetName;
    static char planetName[128];

    if (ImGui::Button("New Object", ImVec2(0.96f * ImGui::GetWindowWidth(), 0.0f))) {
        sampledPlanetName = PLANET_NAMES[randomInt(0, N_PLANET_NAMES - 1)].c_str();
        strcpy(planetName, sampledPlanetName);

        ImGui::OpenPopup("Add New Object");
    }

    if (ImGui::BeginPopupModal("Add New Object", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::InputText("Name", planetName, IM_ARRAYSIZE(planetName));
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Accept")) {
            auto planet = make_shared<sphere>(
                point3(0, 0, 0), 1,
                make_shared<lambertian>(color(randomDouble(0, 1), randomDouble(0, 1), randomDouble(0, 1))), planetName);

            m_world->add(planet);
            m_planetSettingsWindows.clear();
            for (auto planet : m_world->getObjects()) {
                if (planet == m_world->getUniverse())
                    continue;
                m_planetSettingsWindows.push_back(planetSettingsWindow(planet));
            }
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
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
