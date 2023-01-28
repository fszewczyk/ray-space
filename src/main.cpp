#include "core/color.hpp"
#include "core/utils.hpp"
#include "shapes/sphere.hpp"
#include "world/camera.hpp"
#include "world/hittableWorld.hpp"

#include <iostream>

using namespace fg;

color rayColor(const ray &r, const hittable &world) {
    hitData data;
    if (world.hit(r, 0, infinity, data))
        return 0.5 * (data.normal + color(1, 1, 1));

    vec3 unitDirection = unitVector(r.direction());
    auto t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main(int argc, char *argv[]) {
    const auto aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 100;

    hittableWorld world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

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
                pixel += rayColor(r, world);
            }
            pixel /= samplesPerPixel;
            writeColor(std::cout, pixel);
        }
    }

    std::cerr << "\nDone\n";

    return 0;
}
