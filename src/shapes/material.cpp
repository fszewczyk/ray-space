#include "shapes/material.hpp"
#include "core/image.hpp"

namespace shkyera {

lambertian::lambertian(const color &c) : m_albedo(make_shared<solidColor>(c)) {}
lambertian::lambertian(shared_ptr<texture> tex) : m_albedo(tex) {}

metal::metal(const color &c, double f) : m_albedo(c), m_fuzz(f < 1 ? f : (f > 0 ? f : 0)) {}

refractor::refractor(double eta) : m_eta(eta) {}

diffuseLight::diffuseLight(shared_ptr<texture> text, color lightColor)
    : m_lightColor(make_shared<solidColor>(lightColor)) {
    m_textureToDisplay = text;
}

diffuseLight::diffuseLight(color even, color odd, color lightColor)
    : m_textureToDisplay(make_shared<checkerTexture>(even, odd)), m_lightColor(make_shared<solidColor>(lightColor)) {}

diffuseLight::diffuseLight(color displayColor, color lightColor)
    : m_textureToDisplay(make_shared<solidColor>(displayColor)), m_lightColor(make_shared<solidColor>(lightColor)) {}

color material::emit(double u, double v, const point3 &p, bool firstHit) const { return color(0, 0, 0); }

bool lambertian::scatter(const ray &rayIn, const hitData &data, color &attenuation, ray &rayOut) const {
    auto scatterDirection = randomInUnitHemisphere(data.normal);
    rayOut = ray(data.p, scatterDirection);
    attenuation = m_albedo->value(data.u, data.v, data.p);
    return true;
}

std::shared_ptr<lambertian> lambertian::generateFromImage(std::shared_ptr<image> im) {
    auto texture = std::make_shared<imageTexture>(im);
    auto material = std::make_shared<lambertian>(texture);

    return material;
}

std::shared_ptr<lambertian> lambertian::generateFromImageTextureType(int imageTextureType) {
    switch (imageTextureType) {
    case EARTH_DAY:
        return generateFromImage(image::EARTH_DAY_TEXTURE);
        break;
    case EARTH_NIGHT:
        return generateFromImage(image::EARTH_NIGHT_TEXTURE);
        break;
    case MARS:
        return generateFromImage(image::MARS_TEXTURE);
        break;
    case SUN:
        return generateFromImage(image::SUN_TEXTURE);
        break;
    case MOON:
        return generateFromImage(image::MOON_TEXTURE);
        break;
    case CERES:
        return generateFromImage(image::CERES_TEXTURE);
        break;
    case CLOUDY_VENUS:
        return generateFromImage(image::CLOUDY_VENUS_TEXTURE);
        break;
    case ERIS:
        return generateFromImage(image::ERIS_TEXTURE);
        break;
    case HAUMEA:
        return generateFromImage(image::HAUMEA_TEXTURE);
        break;
    case JUPITER:
        return generateFromImage(image::JUPITER_TEXTURE);
        break;
    case MAKE:
        return generateFromImage(image::MAKE_TEXTURE);
        break;
    case MERCURY:
        return generateFromImage(image::MERCURY_TEXTURE);
        break;
    case NEPTUNE:
        return generateFromImage(image::NEPTUNE_TEXTURE);
        break;
    case SATURN:
        return generateFromImage(image::SATURN_TEXTURE);
        break;
    case URANUS:
        return generateFromImage(image::URANUS_TEXTURE);
        break;
    case VENUS:
        return generateFromImage(image::VENUS_TEXTURE);
        break;
    case NONE:
    default:
        return std::make_shared<lambertian>(color(1, 1, 1));
        break;
    }
}

bool metal::scatter(const ray &rayIn, const hitData &data, color &attenuation, ray &rayOut) const {
    vec3 reflected = reflect(unitVector(rayIn.direction()), data.normal);
    rayOut = ray(data.p, reflected + m_fuzz * randomInUnitSphere());
    attenuation = m_albedo;
    return dot(rayOut.direction(), data.normal) > 0;
}

bool refractor::scatter(const ray &rayIn, const hitData &data, color &attenuation, ray &rayOut) const {
    attenuation = color(1.0, 1.0, 1.0);
    double refractionRatio = data.frontFace ? (1.0 / m_eta) : m_eta;

    vec3 unitDirection = unitVector(rayIn.direction());

    double cosTheta = fmin(dot(-unitDirection, data.normal), 1.0);
    double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    bool cannotRefract = refractionRatio * sinTheta > 1.0;
    vec3 direction;

    if (cannotRefract || reflectance(cosTheta, refractionRatio) > randomDouble())
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

std::shared_ptr<diffuseLight> diffuseLight::generateFromImage(std::shared_ptr<image> im, color c) {
    auto texture = std::make_shared<imageTexture>(im);
    auto material = std::make_shared<diffuseLight>(texture, c);

    return material;
}

bool diffuseLight::scatter(const ray &rayIn, const hitData &data, color &attenuation, ray &rayOut) const {
    return false;
}

color diffuseLight::emit(double u, double v, const point3 &p, bool firstHit) const {
    if (firstHit)
        return m_textureToDisplay->value(u, v, p);
    else
        return m_lightColor->value(u, v, p);
}

} // namespace shkyera
