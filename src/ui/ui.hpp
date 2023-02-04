#ifndef UI_H
#define UI_H

#define GLFW_INCLUDE_NONE

#include "core/image.hpp"
#include "ui/render/renderWindow.hpp"
#include "ui/renderer.hpp"
#include "ui/settings/cameraSettingsWindow.hpp"

#include "imgui.h"
#include <GLFW/glfw3.h>

namespace shkyera {

class ui {
  public:
    ui(std::shared_ptr<image> im, std::shared_ptr<renderer> renderer,
       std::shared_ptr<camera> cam);

    void init();
    void run();
    void close();

  private:
    std::shared_ptr<renderer> m_renderer;
    std::shared_ptr<camera> m_camera;

    renderWindow m_renderWindow;
    cameraSettingsWindow m_cameraSettingsWindow;

    GLFWwindow *m_window;
    ImVec4 m_clearColor;
};

} // namespace shkyera

#endif
