#include "core/image.hpp"
#include "core/utils.hpp"
#include "shapes/material.hpp"
#include "shapes/sphere.hpp"
#include "ui/renderer.hpp"
#include "ui/ui.hpp"
#include "world/camera.hpp"
#include "world/hittableWorld.hpp"

#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

using namespace shkyera;

int main(int argc, char *argv[]) {
    const auto aspectRatio = 16.0 / 9.0;
    const int imageWidth = 600;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);

    hittableWorld world;

    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, lambertian::generateFromImage("resources/textures/earth.jpg")));
    world.add(make_shared<sphere>(point3(1, 1, 0), 1.0, lambertian::generateFromImage("resources/textures/moon.jpg")));
    world.add(make_shared<sphere>(point3(-2, 1, 0), 1.0, lambertian::generateFromImage("resources/textures/mars.jpg")));

    point3 lookfrom(6, 2, 8);
    point3 lookat(100, 100, 100);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;

    auto cam = std::make_shared<camera>(lookfrom, lookat, 40, aspectRatio, aperture, dist_to_focus);

    auto im = std::make_shared<image>(imageWidth, imageHeight);

    auto r = std::make_shared<renderer>(world, cam, im);
    r->startRendering();

    ui interface(im, r, cam);

    interface.init();

    while (interface.isOpen()) {
        interface.run();
    }

    return 0;
}
