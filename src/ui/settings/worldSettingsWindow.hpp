#ifndef WORLD_SETTINGS_WINDOW_H
#define WORLD_SETTINGS_WINDOW_H

#include "world/hittableWorld.hpp"

namespace shkyera {

struct worldSettings {
    color ambientColor;
};

class worldSettingsWindow {
  public:
    worldSettingsWindow(std::shared_ptr<hittableWorld> world);

    worldSettings render(bool &updated);

  private:
    std::shared_ptr<hittableWorld> m_world;
};

} // namespace shkyera

#endif
