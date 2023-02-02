#include "ui/renderer.hpp"
#include "shapes/hittable.hpp"
#include "shapes/material.hpp"

#include <execution>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

namespace shkyera {

renderer::renderer(hittableWorld &world, std::shared_ptr<camera> cam,
                   std::shared_ptr<image> im)
    : m_world(world), m_cam(cam), m_image(im) {}

void renderer::startRendering() {
    m_renderedImage = false;
    m_renderingThread = std::thread([this] { render(); });
}

bool renderer::renderedImage() const { return m_renderedImage; }

void renderer::stopRendering() { m_stop = true; }

std::thread &renderer::renderingThread() { return m_renderingThread; }

void renderer::renderRow(int y) {
    for (int x = 0; x < m_image->width(); ++x) {
        auto u = (x + randomDouble()) / (m_image->width() - 1);
        auto v = ((m_image->height() - y) + randomDouble()) /
                 (m_image->height() - 1);

        ray r = m_cam->getRay(u, v);
        color c = rayColor(r, 5);

        m_image->at(x, y) = m_image->at(x, y) * (double(m_samplesTaken) /
                                                 (m_samplesTaken + 1)) +
                            (1.0 / (m_samplesTaken + 1)) * c;
    }
}

void renderer::render() {
    const int maxDepth = 5;

    int imageHeight = m_image->height();
    int imageWidth = m_image->width();

    m_samplesTaken = 0;

    while (true) {
        if (m_stop) {
            clearScene();
            m_stop = false;
            return;
        }

        std::vector<std::thread> renderingThreads;

        for (int y = 0; y < m_image->height(); ++y) {
            renderingThreads.push_back(
                std::thread([this, y] { renderRow(y); }));
        }

        for (auto &t : renderingThreads) {
            t.join();
        }

        m_renderedImage = true;
        m_samplesTaken++;
    }
}

void renderer::clearScene() { m_image->clear(); }

color renderer::rayColor(const ray &r, int depth) {
    if (depth <= 0)
        return color(0, 0, 0);

    hitData data;
    if (m_world.hit(r, 0.001, INFINITY, data)) {
        ray scattered;
        color attenuation;

        if (data.hitMaterial->scatter(r, data, attenuation, scattered))
            return attenuation * rayColor(scattered, depth - 1);

        return color(0, 0, 0);
    }

    vec3 unitDirection = unitVector(r.direction());
    auto t = 0.5 * (unitDirection.y() + 1.0);

    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

} // namespace shkyera
