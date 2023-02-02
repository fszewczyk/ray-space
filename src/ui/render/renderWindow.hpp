#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include "core/image.hpp"
#include "world/camera.hpp"

namespace shkyera {

class renderWindow {
  public:
    renderWindow(std::shared_ptr<image> im, std::shared_ptr<camera> cam);

    point3 render(bool sampleTexture, bool &updated,
                  std::pair<int, int> &mouseMovement);

    std::shared_ptr<image> getImage() const;

  private:
    void updateImageTexture();

    std::shared_ptr<image> m_image;
    std::shared_ptr<camera> m_camera;

    int m_loadWidth;
    int m_loadHeight;
    unsigned m_loadTexId;
    std::vector<uint8_t> m_renderTexture;

    bool m_loadedImage;

    bool m_leftMouseHold;
    std::pair<int, int> m_lastMousePosition;
};

} // namespace shkyera

#endif
