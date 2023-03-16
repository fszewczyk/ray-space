#ifndef CAMERA_H
#define CAMERA_H

#include "core/ray.hpp"
#include "core/utils.hpp"
#include "core/vec3.hpp"

namespace shkyera {

struct cameraSettings {
    point3 origin;
    point3 direction;
    double depthOfField;
    double fieldOfView;
    double focusDistance;

    bool operator==(const cameraSettings &a) {
        return a.origin == origin && a.direction == direction && a.depthOfField == depthOfField &&
               a.fieldOfView == fieldOfView && a.focusDistance == focusDistance;
    }
};

class camera {
  public:
    camera(point3 lookFrom, vec3 direction, double verticalFov, double aspectRatio, double aperture,
           double focusDistance);

    ray getRay(double u, double v) const;

    point3 getPosition() const;
    void setPosition(point3 p);

    vec3 getDirection() const;
    vec3 getSidewaysDirection() const;
    void setDirection(vec3 direction);

    cameraSettings getSettings() const;
    void setSettings(const cameraSettings config);

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

    double m_fieldOfView;
    double m_aspectRatio;
};

} // namespace shkyera

#endif
