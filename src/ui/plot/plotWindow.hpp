#ifndef PLOT_WINDOW_H
#define PLOT_WINDOW_H

#include "world/camera.hpp"
#include "world/visibleWorld.hpp"

namespace shkyera {

struct systemSettings {
    worldSettings world;
    cameraSettings cam;
};

class plotWindow {
  public:
    plotWindow(std::shared_ptr<visibleWorld> world, std::shared_ptr<camera> cam);

    systemSettings render(bool &updated);

  private:
    std::shared_ptr<visibleWorld> m_world;
    std::shared_ptr<camera> m_camera;
};

} // namespace shkyera

#endif
