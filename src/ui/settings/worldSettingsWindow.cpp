#include "ui/settings/worldSettingsWindow.hpp"
#include "ui/ui.hpp"

#include "imgui.h"

namespace shkyera {

worldSettingsWindow::worldSettingsWindow(std::shared_ptr<visibleWorld> world) : m_world(world) {
    updatePlanetSettingsWindows();
}

worldSettings worldSettingsWindow::render(bool &updated) {
    updatePlanetSettingsWindows();

    worldSettings settings = m_world->getSettings();

    float ambientLightColorComponents[3] = {static_cast<float>(settings.ambientColor[0]),
                                            static_cast<float>(settings.ambientColor[1]),
                                            static_cast<float>(settings.ambientColor[2])};

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

    if (ImGui::BeginPopupModal("Add New Object", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::InputText("Name", planetName, IM_ARRAYSIZE(planetName));
        ImGui::Dummy(ImVec2(0.0f, 5.0f));
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Accept")) {
            auto planet = make_shared<sphere>(
                point3(randomDouble(-2, 2), randomDouble(-2, 2), randomDouble(-2, 2)), randomDouble(0.5, 3),
                make_shared<lambertian>(color(randomDouble(0, 1), randomDouble(0, 1), randomDouble(0, 1))), planetName);
            settings.planets.push_back(planet->getSettings());
            settings.updatedPlanets.push_back(true);

            updated = true;

            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::End();

    color newAmbientLightColor(ambientLightColorComponents[0], ambientLightColorComponents[1],
                               ambientLightColorComponents[2]);

    if (settings.ambientColor != newAmbientLightColor) {
        updated = true;
    }

    settings.ambientColor = newAmbientLightColor;

    return settings;
}

void worldSettingsWindow::updatePlanetSettingsWindows() {
    if (m_planetSettingsWindows.size() == m_world->getObjects().size() - 1)
        return;

    m_planetSettingsWindows.clear();
    for (auto planet : m_world->getObjects()) {
        if (planet == m_world->getUniverse())
            continue;
        m_planetSettingsWindows.push_back(planetSettingsWindow(planet));
    }
}

} // namespace shkyera
