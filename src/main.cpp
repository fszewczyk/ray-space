#include "core/color.hpp"
#include "core/utils.hpp"
#include "shapes/material.hpp"
#include "shapes/sphere.hpp"
#include "world/camera.hpp"
#include "world/hittableWorld.hpp"

#include <iostream>

using namespace fg;

color rayColor(const ray &r, const hittable &world, int depth) {
    if (depth <= 0)
        return color(0, 0, 0);

    hitData data;
    if (world.hit(r, 0.001, infinity, data)) {
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

int main(int argc, char *argv[]) {
    const auto aspectRatio = 16.0 / 9.0;
    const int imageWidth = 1500;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 200;
    const int maxDepth = 100;

    hittableWorld world;

    auto mat_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto mat_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    auto mat_left = make_shared<metal>(color(0.85, 0.85, 0.85), 0.5);
    auto mat_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0);

    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, mat_ground));
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, mat_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, mat_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1), 0.5, mat_right));

    camera cam;

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int y = imageHeight - 1; y >= 0; --y) {
        std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush;
        for (int x = 0; x < imageWidth; ++x) {
            color pixel(0, 0, 0);
            for (int s = 0; s < samplesPerPixel; ++s) {
                auto u = (x + randomDouble()) / (imageWidth - 1);
                auto v = (y + randomDouble()) / (imageHeight - 1);

                ray r = cam.getRay(u, v);
                pixel += rayColor(r, world, maxDepth);
            }

            pixel /= samplesPerPixel;
            pixel = color::sqrt(pixel);

            writeColor(std::cout, pixel);
        }
    }

    std::cerr << "\nDone\n";

    return 0;
}
