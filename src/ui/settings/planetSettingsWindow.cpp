#include "ui/settings/planetSettingsWindow.hpp"
#include "ui/ui.hpp"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace shkyera {

planetSettingsWindow::planetSettingsWindow(std::shared_ptr<sphere> planet) : m_planet(planet) {}

planetSettings planetSettingsWindow::render(bool &updated) {
    planetSettings settings = m_planet->getSettings();

    float positionX = settings.origin[0];
    float positionY = settings.origin[1];
    float positionZ = settings.origin[2];

    float radius = settings.radius;

    if (ImGui::CollapsingHeader(settings.name.c_str())) {
        ImGui::PushFont(ui::BOLD_FONT);
        ImGui::Text("Position");
        ImGui::PopFont();
        ImGui::SliderFloat(("X##" + settings.name).c_str(), &positionX, -30.0f, 30.0f, "%.2f");
        ImGui::SliderFloat(("Y##" + settings.name).c_str(), &positionY, -30.0f, 30.0f, "%.2f");
        ImGui::SliderFloat(("Z##" + settings.name).c_str(), &positionZ, -30.0f, 30.0f, "%.2f");

        ImGui::PushFont(ui::BOLD_FONT);
        ImGui::Text("Visual");
        ImGui::PopFont();
        ImGui::SliderFloat(("Radius##" + settings.name).c_str(), &radius, 0.1f, 20.0f, "%.2f");

        settings.mat = renderMaterialSettings();

        ImGui::Dummy(ImVec2(0.0f, 5.0f));
    }

    settings.origin[0] = positionX;
    settings.origin[1] = positionY;
    settings.origin[2] = positionZ;
    settings.radius = radius;

    if (!(settings == m_planet->getSettings())) {
        updated = true;
    }

    return settings;
}

std::shared_ptr<material> planetSettingsWindow::renderMaterialSettings() {
    std::shared_ptr<material> mat = m_planet->getMaterial();

    const char *textureTypes[] = {};
    static const char *currentTextureType = textureTypes[1];

    static float solidColorComponents[3] = {0.5, 0.1, 0.8};
    static bool useSolidColor;
    IMAGE_TEXTURE_TYPE chosenTexture = NONE;

    if (ImGui::Button(("Choose Texture##" + m_planet->getName()).c_str())) {
        ImGui::OpenPopup((m_planet->getName() + " Texture").c_str());
        useSolidColor = false;
    }
    if (ImGui::BeginPopupModal((m_planet->getName() + " Texture").c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

        ImGui::ColorEdit3(("##MaterialSolidColor" + m_planet->getName()).c_str(), solidColorComponents);
        ImGui::SameLine();
        if (ImGui::Button("Apply")) {
            useSolidColor = true;
            ImGui::CloseCurrentPopup();
        }

        ImGui::Dummy(ImVec2(0.0f, 5.0f));

        /* clang-format off */
        chosenTexture = imageTextureButton(image::ICON_EARTH_DAY_TEXTURE, EARTH_DAY, chosenTexture); ImGui::SameLine();
        chosenTexture = imageTextureButton(image::ICON_EARTH_NIGHT_TEXTURE, EARTH_NIGHT, chosenTexture); ImGui::SameLine();
        chosenTexture = imageTextureButton(image::ICON_MARS_TEXTURE, MARS, chosenTexture); ImGui::SameLine();
        chosenTexture = imageTextureButton(image::ICON_SUN_TEXTURE, SUN, chosenTexture); ImGui::SameLine();
        chosenTexture = imageTextureButton(image::ICON_MOON_TEXTURE, MOON, chosenTexture);
        chosenTexture = imageTextureButton(image::ICON_CERES_TEXTURE, CERES, chosenTexture); ImGui::SameLine();
        chosenTexture = imageTextureButton(image::ICON_CLOUDY_VENUS_TEXTURE, CLOUDY_VENUS, chosenTexture); ImGui::SameLine();
        chosenTexture = imageTextureButton(image::ICON_ERIS_TEXTURE, ERIS, chosenTexture); ImGui::SameLine();
        chosenTexture = imageTextureButton(image::ICON_HAUMEA_TEXTURE, HAUMEA, chosenTexture); ImGui::SameLine();
        chosenTexture = imageTextureButton(image::ICON_JUPITER_TEXTURE, JUPITER, chosenTexture);
        chosenTexture = imageTextureButton(image::ICON_MAKE_TEXTURE, MAKE, chosenTexture); ImGui::SameLine();
        chosenTexture = imageTextureButton(image::ICON_MERCURY_TEXTURE, MERCURY, chosenTexture); ImGui::SameLine();
        chosenTexture = imageTextureButton(image::ICON_NEPTUNE_TEXTURE, NEPTUNE, chosenTexture); ImGui::SameLine();
        chosenTexture = imageTextureButton(image::ICON_SATURN_TEXTURE, SATURN, chosenTexture); ImGui::SameLine();
        chosenTexture = imageTextureButton(image::ICON_URANUS_TEXTURE, URANUS, chosenTexture);
        chosenTexture = imageTextureButton(image::ICON_VENUS_TEXTURE, VENUS, chosenTexture);
        /* clang-format on */

        ImGui::EndPopup();
    }

    if (useSolidColor) {
        mat = std::make_shared<lambertian>(
            color(solidColorComponents[0], solidColorComponents[1], solidColorComponents[2]));
        useSolidColor = false;
    } else if (chosenTexture != NONE)
        mat = lambertian::generateFromImageTextureType(chosenTexture);

    return mat;
}

IMAGE_TEXTURE_TYPE planetSettingsWindow::imageTextureButton(std::shared_ptr<image> im,
                                                            IMAGE_TEXTURE_TYPE buttonTextureType,
                                                            IMAGE_TEXTURE_TYPE currentChosenTextureType) {
    if (ImGui::ImageButton((ImTextureID)im->getTextureId(), ImVec2(image::ICON_SIZE, image::ICON_SIZE))) {
        currentChosenTextureType = buttonTextureType;
        ImGui::CloseCurrentPopup();
    }

    return currentChosenTextureType;
}

} // namespace shkyera
