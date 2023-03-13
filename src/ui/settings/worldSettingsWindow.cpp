#include "ui/settings/worldSettingsWindow.hpp"
#include "ui/ui.hpp"

#include "imgui.h"

namespace shkyera {

worldSettingsWindow::worldSettingsWindow(std::shared_ptr<hittableWorld> cam) : m_world(cam) {}

worldSettings worldSettingsWindow::render(bool &updated) {
    worldSettings settings;

    color ambientLightColor = m_world->getAmbientLightColor();
    float ambientLightColorComponents[3] = {static_cast<float>(ambientLightColor[0]),
                                            static_cast<float>(ambientLightColor[1]),
                                            static_cast<float>(ambientLightColor[2])};

    ImGui::Begin("World");
    ImGui::ColorEdit3("Ambient Light", ambientLightColorComponents);
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
