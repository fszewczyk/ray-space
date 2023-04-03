#ifndef RENDERER_H
#define RENDERER_H

#include "core/image.hpp"
#include "core/utils.hpp"
#include "ui/settings/exportSettingsWindow.hpp"
#include "world/camera.hpp"
#include "world/visibleWorld.hpp"

#include <thread>

namespace shkyera {

class renderer {
  public:
    renderer(std::shared_ptr<visibleWorld> world, std::shared_ptr<camera> cam, std::shared_ptr<image> im,
             color background);
    renderer(std::shared_ptr<visibleWorld> world, std::shared_ptr<camera> cam, std::shared_ptr<image> im);

    void startRendering();
    void stopRendering();

    std::thread &renderingThread();

    bool renderedImage() const;

    std::shared_ptr<image> setupImageToExport(exportSettings settings);
    void setDenoiseCoordinates(std::vector<std::pair<size_t, size_t>> coords);
    bool isDenoising() const;
    bool isExporting() const;
    std::shared_ptr<image> stopExporting();

    unsigned int getTakenSamples() const;

    static constexpr float SCALING_FACTOR = 2.5;
    static constexpr int MAXIMUM_RAY_DEPTH = 5;

  private:
    void render();
    void renderRow(int y);
    void renderCoordinates(std::vector<std::pair<size_t, size_t>> coords);
    void updateScaledImage();

    void clearScene();
    color rayColor(const ray &r, int depth);

    color m_backgroundColor;

    std::shared_ptr<image> m_image;
    std::shared_ptr<image> m_imageToDraw;
    std::shared_ptr<camera> m_cam;
    std::shared_ptr<visibleWorld> m_world;

    bool m_stop;
    bool m_renderedImage;

    unsigned int m_samplesTaken;

    std::thread m_renderingThread;

    bool m_isExporting;
    std::vector<std::pair<size_t, size_t>> m_denoiseCoordinates;
};

} // namespace shkyera

#endif
