#ifndef UI_H
#define UI_H

#include "core/image.hpp"
#include "ui/plot/plotWindow.hpp"
#include "ui/render/renderWindow.hpp"
#include "ui/renderer.hpp"
#include "ui/settings/cameraSettingsWindow.hpp"
#include "ui/settings/worldSettingsWindow.hpp"

#include "imgui.h"
#include <GLFW/glfw3.h>

namespace shkyera {

class ui {
  public:
    ui(std::shared_ptr<image> im, std::shared_ptr<renderer> renderer, std::shared_ptr<visibleWorld> world,
       std::shared_ptr<camera> cam);

    void init();
    void run();
    void close();

    bool isOpen() const;

    inline static float MOUSE_SENSITIVITY = 0.025;
    inline static ImFont *NORMAL_FONT;
    inline static ImFont *BOLD_FONT;

  private:
    void style();

    std::shared_ptr<renderer> m_renderer;
    std::shared_ptr<camera> m_camera;
    std::shared_ptr<visibleWorld> m_world;

    renderWindow m_renderWindow;
    cameraSettingsWindow m_cameraSettingsWindow;
    worldSettingsWindow m_worldSettingsWindow;
    plotWindow m_plotWindow;

    GLFWwindow *m_window;
    ImVec4 m_clearColor;

    bool m_open;

    float m_mouseSensitivity;
};

} // namespace shkyera

#endif
