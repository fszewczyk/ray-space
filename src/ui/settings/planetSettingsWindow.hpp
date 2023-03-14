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

    std::shared_ptr<sphere> m_planet;
};

} // namespace shkyera

#endif
