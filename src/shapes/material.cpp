#include "shapes/material.hpp"

namespace shkyera {

lambertian::lambertian(const color &c) : m_albedo(make_shared<solidColor>(c)) {}
lambertian::lambertian(shared_ptr<texture> tex) : m_albedo(tex) {}

metal::metal(const color &c, double f)
    : m_albedo(c), m_fuzz(f < 1 ? f : (f > 0 ? f : 0)) {}

refractor::refractor(double eta) : m_eta(eta) {}

bool lambertian::scatter(const ray &rayIn, const hitData &data,
                         color &attenuation, ray &rayOut) const {
    auto scatterDirection = randomInUnitHemisphere(data.normal);
    rayOut = ray(data.p, scatterDirection);
    attenuation = m_albedo->value(data.u, data.v, data.p);
    return true;
}

bool metal::scatter(const ray &rayIn, const hitData &data, color &attenuation,
                    ray &rayOut) const {
    vec3 reflected = reflect(unitVector(rayIn.direction()), data.normal);
    rayOut = ray(data.p, reflected + m_fuzz * randomInUnitSphere());
    attenuation = m_albedo;
    return dot(rayOut.direction(), data.normal) > 0;
}

bool refractor::scatter(const ray &rayIn, const hitData &data,
                        color &attenuation, ray &rayOut) const {
    attenuation = color(1.0, 1.0, 1.0);
    double refractionRatio = data.frontFace ? (1.0 / m_eta) : m_eta;

    vec3 unitDirection = unitVector(rayIn.direction());

    double cosTheta = fmin(dot(-unitDirection, data.normal), 1.0);
    double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    bool cannotRefract = refractionRatio * sinTheta > 1.0;
    vec3 direction;

    if (cannotRefract ||
        reflectance(cosTheta, refractionRatio) > randomDouble())
        direction = reflect(unitDirection, data.normal);
    else
        direction = refract(unitDirection, data.normal, refractionRatio);

    rayOut = ray(data.p, direction);

    return true;
}

double refractor::reflectance(double cosine, double eta) {
    auto r0 = (1 - eta) / (1 + eta);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

} // namespace shkyera
