#ifndef WORLD_SETTINGS_WINDOW_H
#define WORLD_SETTINGS_WINDOW_H

#include "ui/settings/planetSettingsWindow.hpp"
#include "world/visibleWorld.hpp"

namespace shkyera {

class worldSettingsWindow {
  public:
    worldSettingsWindow(std::shared_ptr<visibleWorld> world);

    worldSettings render(bool &updated);

  private:
    void objectSettings();

    std::shared_ptr<visibleWorld> m_world;
    std::vector<planetSettingsWindow> m_planetSettingsWindows;
};

} // namespace shkyera

#endif
