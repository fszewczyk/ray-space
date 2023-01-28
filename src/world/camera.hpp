#ifndef CAMERA_H
#define CAMERA_H

#include "core/utils.hpp"

namespace fg {

class camera {
  public:
    camera();

    ray getRay(double u, double v) const;

  private:
    point3 m_origin;
    point3 m_lowerLeftCorner;
    vec3 m_horizontal;
    vec3 m_vertical;
};

} // namespace fg

#endif
