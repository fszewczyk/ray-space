#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"

namespace shkyera {

class ray {
  public:
    ray() = default;
    ray(const point3 &origin, const vec3 &direction);

    point3 origin() const;
    vec3 direction() const;

    point3 at(double t) const;

  private:
    point3 m_origin;
    point3 m_direction;
};

} // namespace shkyera

#endif
