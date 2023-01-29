#ifndef UI_H
#define UI_H

#define GLFW_INCLUDE_NONE

#include "core/image.hpp"

#include "GLFW/glfw3.h"
#include "imgui.h"

namespace fg {

class ui {
  public:
    ui(image &im);

    void init();
    void run();
    void close();

  private:
    void updateImageTexture();

    image &m_image;

    GLFWwindow *m_window;
    ImGuiIO m_io;
    ImVec4 m_clearColor;

    int m_loadWidth;
    int m_loadHeight;
    unsigned m_loadTexId;
    std::vector<uint8_t> m_renderTexture;

    bool m_loadedImage;
};

} // namespace fg

#endif
