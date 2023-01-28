#include "core/color.hpp"
#include "core/ray.hpp"
#include "core/vec3.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    using namespace fg;

    const auto aspectRatio = 16.0 / 9.0;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);

    auto viewportHeight = 2.0;
    auto viewportWidth = aspectRatio * viewportHeight;
    auto focalLength = 1.0;

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

            color pixel = ray::rayColor(r);

            writeColor(std::cout, pixel);
        }
    }

    std::cerr << "\nDone\n";

    return 0;
}
