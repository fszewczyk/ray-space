#ifndef COLOR_H
#define COLOR_H

#include "vec3.hpp"

#include <iostream>

namespace fg {

void writeColor(std::ostream &out, color pixelColor) {
    out << static_cast<int>(255.999 * pixelColor.x()) << ' '
        << static_cast<int>(255.999 * pixelColor.y()) << ' '
        << static_cast<int>(255.999 * pixelColor.z()) << '\n';
}

} // namespace fg

#endif
