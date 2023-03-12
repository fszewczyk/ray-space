#include "core/image.hpp"
#include "core/ray.hpp"
#include "core/utils.hpp"
#include "core/vec3.hpp"
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

    auto earthMaterial = lambertian::generateFromImage("resources/textures/earthday.jpg");
    auto marsMaterial = lambertian::generateFromImage("resources/textures/mars.jpg");
    auto sunMaterial = diffuseLight::generateFromImage("resources/textures/sun.jpg", color(10, 10, 10));
    auto starsMaterial = diffuseLight::generateFromImage("resources/textures/stars.jpg", color(0.15, 0.15, 0.15));

    world.add(make_shared<sphere>(point3(-8, 0, 0.2), 2, earthMaterial));
    world.add(make_shared<sphere>(point3(0, 0, -17.5), 10.0, sunMaterial));
    world.add(make_shared<sphere>(point3(0, 0, 3), 1.0, marsMaterial));
    world.add(make_shared<sphere>(point3(0, 0, 0), 100000.0, starsMaterial));

    point3 lookfrom(6, 2, 8);
    point3 lookat(100, 100, 100);
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;

    auto cam = std::make_shared<camera>(lookfrom, lookat, 40, aspectRatio, aperture, dist_to_focus);

    auto im = std::make_shared<image>(imageWidth, imageHeight);

    auto r = std::make_shared<renderer>(world, cam, im, color(0, 0, 0));
    r->startRendering();

    ui interface(im, r, cam);

    interface.init();

    while (interface.isOpen()) {
        interface.run();
    }

    return 0;
}
