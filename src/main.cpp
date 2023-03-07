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
    const int imageWidth = 260;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);

    hittableWorld world;
    auto material1 = make_shared<refractor>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto ground =
        make_shared<checkerTexture>(color(0.9, 0.3, 0.3), color(0.3, 0.9, 0.3));
    world.add(make_shared<sphere>(point3(0, -100, 0), 100.5,
                                  make_shared<lambertian>(ground)));

    auto material4 = make_shared<metal>(color(1, 0.6, 1), 0.0);
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material4));

    world.add(make_shared<sphere>(point3(4, -1, 0), 1.0, material4));

    point3 lookfrom(6, 2, 8);
    point3 lookat(10000, 10000, 10000);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;

    auto cam = std::make_shared<camera>(lookfrom, lookat, 40, aspectRatio,
                                        aperture, dist_to_focus);

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
