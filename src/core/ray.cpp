#include "ray.hpp"

namespace fg {

ray::ray(const point3 &origin, const vec3 &direction)
    : m_origin(origin), m_direction(direction) {}

point3 ray::origin() const { return m_origin; }
vec3 ray::direction() const { return m_direction; }

point3 ray::at(double t) const { return m_origin + t * m_direction; }

} // namespace fg
