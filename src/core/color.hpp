#ifndef COLOR_H
#define COLOR_H

#include "core/utils.hpp"

#include <iostream>

namespace fg {

void writeColor(std::ostream &out, color pixelColor) {
    auto r = pixelColor.x();
    auto g = pixelColor.y();
    auto b = pixelColor.z();

    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

} // namespace fg

#endif
