#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#include "core/image.hpp"

namespace shkyera {

class renderWindow {
  public:
    renderWindow(std::shared_ptr<image> im);

    point3 render(bool sampleTexture, bool &updated);

    std::shared_ptr<image> getImage() const;

  private:
    void updateImageTexture();

    std::shared_ptr<image> m_image;

    int m_loadWidth;
    int m_loadHeight;
    unsigned m_loadTexId;
    std::vector<uint8_t> m_renderTexture;

    bool m_loadedImage;
};

} // namespace shkyera

#endif
