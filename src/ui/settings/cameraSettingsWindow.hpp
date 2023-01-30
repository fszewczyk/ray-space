#ifndef CAMERA_SETTINGS_WINDOW_H
#define CAMERA_SETTINGS_WINDOW_H

#include "world/camera.hpp"

namespace shkyera {

class cameraSettingsWindow {
  public:
    cameraSettingsWindow(std::shared_ptr<camera> im);

    void render(bool &updated);

  private:
    std::shared_ptr<camera> m_camera;
};

} // namespace shkyera

#endif
