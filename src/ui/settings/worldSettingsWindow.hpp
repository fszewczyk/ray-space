#ifndef WORLD_SETTINGS_WINDOW_H
#define WORLD_SETTINGS_WINDOW_H

#include "world/visibleWorld.hpp"

namespace shkyera {

struct worldSettings {
    color ambientColor;
};

class worldSettingsWindow {
  public:
    worldSettingsWindow(std::shared_ptr<visibleWorld> world);

    worldSettings render(bool &updated);

  private:
    void objectSettings();

    std::shared_ptr<visibleWorld> m_world;
};

} // namespace shkyera

#endif
