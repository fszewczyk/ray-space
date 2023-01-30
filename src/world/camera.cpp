#include "world/camera.hpp"

namespace shkyera {

camera::camera(point3 lookFrom, point3 lookAt, vec3 up, double verticalFov,
               double aspectRatio, double aperture, double focusDistance) {
    auto theta = degreesToRadians(verticalFov);
    auto h = tan(theta / 2);

    auto viewportHeight = 2.0 * h;
    auto viewportWidth = aspectRatio * viewportHeight;

    m_w = unitVector(lookFrom - lookAt);
    m_u = unitVector(cross(up, m_w));
    m_v = cross(m_w, m_u);

    m_origin = lookFrom;
    m_horizontal = focusDistance * viewportWidth * m_u;
    m_vertical = focusDistance * viewportHeight * m_v;
    m_lowerLeftCorner =
        m_origin - m_horizontal / 2 - m_vertical / 2 - focusDistance * m_w;

    m_lensRadius = aperture / 2;
}

ray camera::getRay(double s, double t) const {
    vec3 sampleFromLens = m_lensRadius * randomInUnitSphere();
    vec3 offset = m_u * sampleFromLens.x() + m_v * sampleFromLens.y();

    return ray(m_origin + offset, m_lowerLeftCorner + s * m_horizontal +
                                      t * m_vertical - m_origin - offset);
};

point3 camera::getPosition() const { return m_origin; }

void camera::setPosition(point3 p) { m_origin = p; }

} // namespace shkyera
