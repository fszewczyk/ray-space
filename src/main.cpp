#include "core/color.hpp"
#include "core/vec3.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    using namespace fg;

    const int imageWidth = 256;
    const int imageHeight = 256;

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int y = imageHeight - 1; y >= 0; --y) {
        std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush;
        for (int x = 0; x < imageWidth; ++x) {
            color pixel(double(x) / (imageWidth - 1),
                        double(y) / (imageHeight - 1), 0.3);
            writeColor(std::cout, pixel);
        }
    }

    std::cerr << "\nDone\n";

    return 0;
}
