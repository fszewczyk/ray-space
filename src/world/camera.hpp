#ifndef CAMERA_H
#define CAMERA_H

#include "core/utils.hpp"

namespace fg {

class camera {
  public:
    camera(point3 lookFrom, point3 lookAt, vec3 up, double verticalFov,
           double aspectRatio, double aperture, double focusDistance);

    ray getRay(double u, double v) const;

  private:
    point3 m_origin;
    point3 m_lowerLeftCorner;
    vec3 m_horizontal;
    vec3 m_vertical;
    vec3 m_u, m_v, m_w;
    double m_lensRadius;
};

} // namespace fg

#endif
