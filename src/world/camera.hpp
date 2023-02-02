#ifndef CAMERA_H
#define CAMERA_H

#include "core/utils.hpp"

namespace shkyera {

struct cameraConfig {
    point3 origin;
    point3 direction;
    double depthOfField;
    double fov;
    double focusDistance;
};

class camera {
  public:
    camera(point3 lookFrom, vec3 direction, double verticalFov,
           double aspectRatio, double aperture, double focusDistance);

    ray getRay(double u, double v) const;

    point3 getPosition() const;
    void setPosition(point3 p);

    vec3 getDirection() const;
    vec3 getSidewaysDirection() const;
    vec3 setDirection(vec3 direction);

  private:
    point3 m_origin;
    point3 m_lowerLeftCorner;
    vec3 m_horizontal;
    vec3 m_vertical;
    vec3 m_u, m_v, m_w;

    double m_lensRadius;
    double m_focusDistance;

    double m_viewportHeight;
    double m_viewportWidth;
};

} // namespace shkyera

#endif
