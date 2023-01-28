#include "core/color.hpp"
#include "core/utils.hpp"
#include "shapes/hittableWorld.hpp"
#include "shapes/sphere.hpp"

#include <iostream>

using namespace fg;

color rayColor(const ray &r, const hittableWorld &world) {
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

    auto viewportHeight = 2.0;
    auto viewportWidth = aspectRatio * viewportHeight;
    auto focalLength = 1.0;

    hittableWorld world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    point3 origin(0, 0, 0);
    vec3 horizontal(viewportWidth, 0, 0);
    vec3 vertical(0, viewportHeight, 0);
    auto lowerLeftCorner =
        origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focalLength);

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int y = imageHeight - 1; y >= 0; --y) {
        std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush;
        for (int x = 0; x < imageWidth; ++x) {
            auto u = double(x) / (imageWidth - 1);
            auto v = double(y) / (imageHeight - 1);

            ray r(origin,
                  lowerLeftCorner + u * horizontal + v * vertical - origin);

            color pixel = rayColor(r, world);

            writeColor(std::cout, pixel);
        }
    }

    std::cerr << "\nDone\n";

    return 0;
}
