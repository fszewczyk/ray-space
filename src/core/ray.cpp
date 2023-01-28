#include "ray.hpp"

namespace fg {

ray::ray(const point3 &origin, const vec3 &direction)
    : m_origin(origin), m_direction(direction) {}

point3 ray::origin() const { return m_origin; }
vec3 ray::direction() const { return m_direction; }

point3 ray::at(double t) const { return m_origin + t * m_direction; }

color ray::rayColor(const ray &r) {
    vec3 unitDirection = unitVector(r.direction());
    auto t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

} // namespace fg
