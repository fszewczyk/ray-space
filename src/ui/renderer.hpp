#ifndef RENDERER_H
#define RENDERER_H

#include "core/image.hpp"
#include "core/utils.hpp"
#include "world/camera.hpp"
#include "world/hittableWorld.hpp"

#include <thread>

namespace fg {

class renderer {
  public:
    renderer(hittableWorld &world, camera &cam, image &im);

    std::thread startRendering();

  private:
    void render();
    void clearScene();
    color rayColor(const ray &r, int depth);

    image &m_image;
    hittableWorld &m_world;
    camera &m_cam;
};

} // namespace fg

#endif
