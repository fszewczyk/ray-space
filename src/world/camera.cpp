#include "world/camera.hpp"

namespace fg {

camera::camera() {
    auto aspectRatio = 16.0 / 9.0;
    auto viewportHeight = 2.0;
    auto viewportWidth = aspectRatio * viewportHeight;
    auto focalLength = 1.0;

    m_origin = point3(0, 0, 0);
    m_horizontal = vec3(viewportWidth, 0, 0);
    m_vertical = vec3(0, viewportHeight, 0);

    m_lowerLeftCorner =
        m_origin - m_horizontal / 2 - m_vertical / 2 - vec3(0, 0, focalLength);
}

ray camera::getRay(double u, double v) const {
    return ray(m_origin, m_lowerLeftCorner + u * m_horizontal + v * m_vertical -
                             m_origin);
};

} // namespace fg
