#include "core/image.hpp"
#include "core/utils.hpp"
#include "shapes/material.hpp"
#include "shapes/sphere.hpp"
#include "ui/ui.hpp"
#include "world/camera.hpp"
#include "world/hittableWorld.hpp"

#include <iostream>
#include <numeric>
#include <vector>

using namespace fg;

color rayColor(const ray &r, const hittable &world, int depth) {
    if (depth <= 0)
        return color(0, 0, 0);

    hitData data;
    if (world.hit(r, 0.001, INFINITY, data)) {
        ray scattered;
        color attenuation;

        if (data.material->scatter(r, data, attenuation, scattered))
            return attenuation * rayColor(scattered, world, depth - 1);
        return color(0, 0, 0);
    }

    vec3 unitDirection = unitVector(r.direction());
    auto t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

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
    ui interface;
    while (true) {
        interface.run();
    }
    const auto aspectRatio = 16.0 / 9.0;
    const int imageWidth = 500;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 20;
    const int maxDepth = 5;

    hittableWorld world = randomScene();

    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.2;

    camera cam(lookfrom, lookat, vup, 20, aspectRatio, aperture, dist_to_focus);

    image im(imageWidth, imageHeight);

    for (int s = 0; s < samplesPerPixel; ++s) {
        std::for_each(
            im.verticalPixels().begin(), im.verticalPixels().end(), [&](int y) {
                std::cerr << "scans remaining: " << samplesPerPixel - s << ' '
                          << "\rScanlines remaining: " << (imageHeight - y)
                          << ' ' << std::flush;
                std::for_each(im.horizontalPixels().begin(),
                              im.horizontalPixels().end(), [&](int x) {
                                  auto u =
                                      (x + randomDouble()) / (imageWidth - 1);
                                  auto v =
                                      ((imageHeight - y) + randomDouble()) /
                                      (imageHeight - 1);

                                  ray r = cam.getRay(u, v);
                                  color c = rayColor(r, world, maxDepth);

                                  im(x, y) = im(x, y) * (double(s) / (s + 1)) +
                                             (1.0 / (s + 1)) * c;
                              });
            });
    }

    im.writeImage(std::cout);

    std::cerr << "\nDone\n";

    return 0;
}
