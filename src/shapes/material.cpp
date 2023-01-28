#include "shapes/material.hpp"

namespace fg {

lambertian::lambertian(const color &c) : m_albedo(c) {}

bool lambertian::scatter(const ray &rayIn, const hitData &data,
                         color &attenuation, ray &rayOut) const {
    auto scatterDirection = randomInUnitHemisphere(data.normal);
    rayOut = ray(data.p, scatterDirection);
    attenuation = m_albedo;
    return true;
}

metal::metal(const color &c, double f)
    : m_albedo(c), m_fuzz(f < 1 ? f : (f > 0 ? f : 0)) {}

bool metal::scatter(const ray &rayIn, const hitData &data, color &attenuation,
                    ray &rayOut) const {
    vec3 reflected = reflect(unitVector(rayIn.direction()), data.normal);
    rayOut = ray(data.p, reflected + m_fuzz * randomInUnitSphere());
    attenuation = m_albedo;
    return dot(rayOut.direction(), data.normal) > 0;
}

} // namespace fg
