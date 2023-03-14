#include "ui/settings/planetSettingsWindow.hpp"
#include "ui/ui.hpp"

#include "imgui.h"

namespace shkyera {

planetSettingsWindow::planetSettingsWindow(std::shared_ptr<sphere> planet) : m_planet(planet) {}

planetSettings planetSettingsWindow::render(bool &updated) {
    planetSettings settings = m_planet->getSettings();

    float positionX = settings.origin[0];
    float positionY = settings.origin[1];
    float positionZ = settings.origin[2];

    float radius = settings.radius;
    std::string name = settings.name;

    if (ImGui::CollapsingHeader(name.c_str())) {
        ImGui::PushFont(ui::BOLD_FONT);
        ImGui::Text("Position");
        ImGui::PopFont();
        ImGui::SliderFloat(("X##" + name).c_str(), &positionX, -30.0f, 30.0f, "%.2f");
        ImGui::SliderFloat(("Y##" + name).c_str(), &positionY, -30.0f, 30.0f, "%.2f");
        ImGui::SliderFloat(("Z##" + name).c_str(), &positionZ, -30.0f, 30.0f, "%.2f");

        ImGui::PushFont(ui::BOLD_FONT);
        ImGui::Text("Visual");
        ImGui::PopFont();
        ImGui::SliderFloat(("Radius##" + name).c_str(), &radius, 0.1f, 20.0f, "%.2f");

        settings.mat = renderMaterialSettings();

        ImGui::Dummy(ImVec2(0.0f, 5.0f));
    }

    settings.origin[0] = positionX;
    settings.origin[1] = positionY;
    settings.origin[2] = positionZ;
    settings.radius = radius;
    settings.name = name;

    if (!(settings == m_planet->getSettings())) {
        updated = true;
    }

    return settings;
}

std::shared_ptr<material> planetSettingsWindow::renderMaterialSettings() {
    std::shared_ptr<material> mat = m_planet->getMaterial();

    const char *textureTypes[] = {"Solid Color", "Image Texture"};
    static const char *currentTextureType = textureTypes[1];

    if (ImGui::BeginCombo(("Texture##" + m_planet->getName()).c_str(), currentTextureType)) {
        for (int n = 0; n < IM_ARRAYSIZE(textureTypes); n++) {
            bool is_selected =
                (currentTextureType ==
                 textureTypes[n]); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(textureTypes[n], is_selected))
                currentTextureType = textureTypes[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus(); // You may set the initial focus when opening the combo (scrolling + for
                                              // keyboard navigation support)
        }
        ImGui::EndCombo();
    }

    return mat;
}

} // namespace shkyera
