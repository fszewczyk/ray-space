#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include "core/image.hpp"

namespace fg {

class renderWindow {
  public:
    renderWindow(image &im);

    void render();

    image &getImage() const;

  private:
    void updateImageTexture();

    image &m_image;

    int m_loadWidth;
    int m_loadHeight;
    unsigned m_loadTexId;
    std::vector<uint8_t> m_renderTexture;

    bool m_loadedImage;
};

} // namespace fg

#endif
