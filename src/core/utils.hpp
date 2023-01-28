#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

namespace fg {

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degreesToRadians(double degrees) { return degrees * pi / 180.0; }

} // namespace fg

#include "ray.hpp"
#include "vec3.hpp"

#endif
