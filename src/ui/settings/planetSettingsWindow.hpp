#ifndef PLANET_SETTINGS_WINDOW_H
#define PLANET_SETTINGS_WINDOW_H

#include "shapes/material.hpp"
#include "world/visibleWorld.hpp"

namespace shkyera {

class planetSettingsWindow {
  public:
    planetSettingsWindow(std::shared_ptr<sphere> planet);

    planetSettings render(bool &updated);

  private:
    std::shared_ptr<material> renderMaterialSettings();
    IMAGE_TEXTURE_TYPE imageTextureButton(std::shared_ptr<image> im, IMAGE_TEXTURE_TYPE buttonTextureType,
                                          IMAGE_TEXTURE_TYPE currentChosenTextureType);

    std::shared_ptr<sphere> m_planet;
};

} // namespace shkyera

#endif
