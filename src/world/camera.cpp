#include "world/camera.hpp"

namespace shkyera {

camera::camera(point3 lookFrom, vec3 direction, double verticalFov,
               double aspectRatio, double aperture, double focusDistance) {
    auto theta = degreesToRadians(verticalFov);
    auto h = tan(theta / 2);

    m_viewportHeight = 2.0 * h;
    m_viewportWidth = aspectRatio * m_viewportHeight;

    m_w = unitVector(direction);
    m_u = unitVector(cross(vec3(0, 1, 0), m_w));
    m_v = cross(m_w, m_u);

    m_focusDistance = focusDistance;
    m_horizontal = m_focusDistance * m_viewportWidth * m_u;
    m_vertical = m_focusDistance * m_viewportHeight * m_v;

    m_lensRadius = aperture / 2;

    setPosition(lookFrom);
}

ray camera::getRay(double s, double t) const {
    vec3 sampleFromLens = m_lensRadius * randomInUnitSphere();
    vec3 offset = m_u * sampleFromLens.x() + m_v * sampleFromLens.y();

    return ray(m_origin + offset, m_lowerLeftCorner + s * m_horizontal +
                                      t * m_vertical - m_origin - offset);
};

point3 camera::getPosition() const { return m_origin; }

void camera::setPosition(point3 p) {
    m_origin = p;

    m_lowerLeftCorner =
        m_origin - m_horizontal / 2 - m_vertical / 2 - m_focusDistance * m_w;
}

vec3 camera::getDirection() const { return m_w; }
vec3 camera::getSidewaysDirection() const { return m_u; }

void camera::setDirection(vec3 direction) {
    m_w = unitVector(direction);
    m_u = unitVector(cross(vec3(0, 1, 0), m_w));
    m_v = cross(m_w, m_u);

    m_horizontal = m_focusDistance * m_viewportWidth * m_u;
    m_vertical = m_focusDistance * m_viewportHeight * m_v;

    m_lowerLeftCorner =
        m_origin - m_horizontal / 2 - m_vertical / 2 - m_focusDistance * m_w;
}
} // namespace shkyera
