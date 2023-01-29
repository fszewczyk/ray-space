#ifndef UI_H
#define UI_H

#define GLFW_INCLUDE_NONE

#include "GLFW/glfw3.h"
#include "imgui.h"

namespace fg {

class ui {
  public:
    ui() = default;

    void init();
    void run();

  private:
};

} // namespace fg

#endif
