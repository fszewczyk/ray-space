#include "ui/renderer.hpp"
#include "shapes/material.hpp"
#include "shapes/sphere.hpp"

#include <execution>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

namespace shkyera {

renderer::renderer(std::shared_ptr<visibleWorld> world,
                   std::shared_ptr<camera> cam, std::shared_ptr<image> im,
                   color backgroundColor)
    : m_world(world), m_cam(cam), m_image(im), m_stop(false),
      m_isExporting(false) {
  m_imageToDraw = std::make_shared<image>(m_image->width() / SCALING_FACTOR,
                                          m_image->height() / SCALING_FACTOR);
  m_backgroundColor = backgroundColor;
}

renderer::renderer(std::shared_ptr<visibleWorld> world,
                   std::shared_ptr<camera> cam, std::shared_ptr<image> im)
    : m_world(world), m_cam(cam), m_image(im), m_isExporting(false) {
  m_imageToDraw = std::make_shared<image>(m_image->width() / SCALING_FACTOR,
                                          m_image->height() / SCALING_FACTOR);
}

void renderer::startRendering() {
  m_renderedImage = false;
  m_samplesTaken = 0;
  m_renderingThread = std::thread([this] { render(); });
}

bool renderer::renderedImage() const { return m_renderedImage; }

void renderer::stopRendering() { m_stop = true; }

std::shared_ptr<image> renderer::setupImageToExport(exportSettings settings) {
  m_imageToDraw = std::make_shared<image>(settings.width, settings.height);
  m_isExporting = true;

  return m_imageToDraw;
}

void renderer::setDenoiseCoordinates(
    std::vector<std::pair<size_t, size_t>> coords) {
  m_denoiseCoordinates = coords;
}

bool renderer::isDenoising() const { return !m_denoiseCoordinates.empty(); }

bool renderer::isExporting() const { return m_isExporting; }

std::shared_ptr<image> renderer::stopExporting() {
  m_imageToDraw = std::make_shared<image>(m_image->width() / SCALING_FACTOR,
                                          m_image->height() / SCALING_FACTOR);
  m_isExporting = false;
  m_cam->setAspectRatio(static_cast<float>(m_imageToDraw->width()) /
                        m_imageToDraw->height());
  m_denoiseCoordinates.clear();

  return m_image;
}

unsigned int renderer::getTakenSamples() const { return m_samplesTaken; }

std::thread &renderer::renderingThread() { return m_renderingThread; }

void renderer::renderRow(int y) {
  for (int x = 0; x < m_imageToDraw->width(); ++x) {
    auto u = (x + randomDouble()) / (m_imageToDraw->width() - 1);
    auto v = ((m_imageToDraw->height() - y) + randomDouble()) /
             (m_imageToDraw->height() - 1);

    ray r = m_cam->getRay(u, v);
    color c = rayColor(r, MAXIMUM_RAY_DEPTH);

    color result = m_imageToDraw->at(x, y) *
                       (double(m_samplesTaken) / (m_samplesTaken + 1)) +
                   (1.0 / (m_samplesTaken + 1)) * c;

    m_imageToDraw->at(x, y) = clamp(result, 0, 1);
  }
}

void renderer::renderCoordinates(
    std::vector<std::pair<size_t, size_t>> coords) {
  for (auto [x, y] : coords) {
    auto u = (x + randomDouble()) / (m_imageToDraw->width() - 1);
    auto v = ((m_imageToDraw->height() - y) + randomDouble()) /
             (m_imageToDraw->height() - 1);

    ray r = m_cam->getRay(u, v);
    color c = rayColor(r, MAXIMUM_RAY_DEPTH);

    color result = m_imageToDraw->at(x, y) *
                       (double(m_samplesTaken) / (m_samplesTaken + 1)) +
                   (1.0 / (m_samplesTaken + 1)) * c;

    m_imageToDraw->at(x, y) = clamp(result, 0, 1);
  }
}

void renderer::render() {
  const int maxDepth = 5;

  int imageHeight = m_imageToDraw->height();
  int imageWidth = m_imageToDraw->width();

  m_samplesTaken = 0;

  while (true) {
    if (m_stop) {
      clearScene();
      m_stop = false;
      break;
    }

    // Whoever reads this, I'm sorry it's done this way. Nothing else
    // worked.
    std::vector<std::thread> renderingThreads;
    if (m_denoiseCoordinates.empty()) {
      for (int y = 0; y < m_imageToDraw->height(); ++y) {
        renderingThreads.push_back(std::thread([this, y] { renderRow(y); }));
      }
    } else {
      for (size_t i = 0; i < m_denoiseCoordinates.size(); i += 1000) {
        size_t last = std::min(i + 1000, m_denoiseCoordinates.size());
        std::vector<std::pair<size_t, size_t>> cords(
            m_denoiseCoordinates.begin() + i,
            m_denoiseCoordinates.begin() + last);
        renderingThreads.push_back(
            std::thread([this, cords] { renderCoordinates(cords); }));
      }
    }

    for (auto &t : renderingThreads) {
      t.join();
    }

    m_imageToDraw->scaleImage(m_image);

    m_renderedImage = true;
    m_samplesTaken++;
  }
}

void renderer::clearScene() { m_image->clear(); }

color renderer::rayColor(const ray &r, int depth) {
  if (depth <= 0)
    return color(0, 0, 0);

  hitData data;

  if (!m_world->hit(r, 0.001, INFINITY, data)) {
    return m_backgroundColor;
  }

  bool initialHit = depth == MAXIMUM_RAY_DEPTH;

  ray scattered;
  color attenuation;
  color emittedLight =
      data.hitMaterial->emit(data.u, data.v, data.p, initialHit);

  if (!data.hitMaterial->scatter(r, data, attenuation, scattered)) {
    return emittedLight;
  }

  color result = emittedLight + attenuation * rayColor(scattered, depth - 1);

  return result;
}

} // namespace shkyera
