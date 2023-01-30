#ifndef RENDERER_H
#define RENDERER_H

#include "core/image.hpp"
#include "core/utils.hpp"
#include "world/camera.hpp"
#include "world/hittableWorld.hpp"

#include <thread>

namespace shkyera {

class renderer {
  public:
    renderer(hittableWorld &world, std::shared_ptr<camera> cam,
             std::shared_ptr<image> im);

    void startRendering();
    void stopRendering();

    std::thread &renderingThread();

    bool renderedImage() const;

  private:
    void render();
    void clearScene();
    color rayColor(const ray &r, int depth);

    std::shared_ptr<image> m_image;
    std::shared_ptr<camera> m_cam;
    hittableWorld &m_world;

    bool m_stop;
    bool m_renderedImage;

    std::thread m_renderingThread;
};

} // namespace shkyera

#endif
