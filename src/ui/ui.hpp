#ifndef UI_H
#define UI_H

#define GLFW_INCLUDE_NONE

#include "core/image.hpp"
#include "ui/render/renderWindow.hpp"

#include "imgui.h"
#include <GLFW/glfw3.h>

namespace fg {

class ui {
  public:
    ui(image &im);

    void init();
    void run();
    void close();

  private:
    renderWindow m_renderWindow;

    GLFWwindow *m_window;
    ImVec4 m_clearColor;
};

} // namespace fg

#endif
