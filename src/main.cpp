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

using namespace fg;

hittableWorld randomScene() {
    hittableWorld world;

    world.add(
        make_shared<sphere>(point3(0, -1000, 0), 1000,
                            make_shared<lambertian>(color(0.5, 0.5, 0.5))));

    for (int a = -11; a < 11; a += 6) {
        for (int b = -11; b < 11; b += 6) {
            auto choose_mat = randomDouble();
            point3 center(a + 0.9 * randomDouble(), 0.2,
                          b + 0.9 * randomDouble());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(
                        make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(
                        make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<refractor>(1.5);
                    world.add(
                        make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<refractor>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int main(int argc, char *argv[]) {
    const auto aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 1;

    hittableWorld world = randomScene();

    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.3;

    camera cam(lookfrom, lookat, vup, 20, aspectRatio, aperture, dist_to_focus);
    image im(imageWidth, imageHeight);
    ui interface(im);

    renderer r(world, cam, im);

    interface.init();

    std::thread renderThread = r.startRendering();

    while (true) {
        interface.run();
    }

    interface.close();

    return 0;

    im.writeImage(std::cout);

    std::cerr << "\nDone\n";

    return 0;
}
