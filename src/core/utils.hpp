#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

namespace shkyera {

constexpr double INIFINITY = std::numeric_limits<double>::infinity();
constexpr double PI = 3.1415926535897932385;

inline double degreesToRadians(double degrees) { return degrees * PI / 180.0; }

inline double randomDouble() { return rand() / (RAND_MAX + 1.0); }

inline double randomDouble(double min, double max) { return min + (max - min) * randomDouble(); }

inline int randomInt(int min, int max) { return static_cast<int>(randomDouble(min, max + 1)); }

inline double clamp(double x, double min, double max) {
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

} // namespace shkyera

#endif
