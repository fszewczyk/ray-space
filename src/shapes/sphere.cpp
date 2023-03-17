#include "shapes/sphere.hpp"

#include <cmath>

namespace shkyera {

sphere::sphere(point3 center, double radius, shared_ptr<material> mat)
    : m_center(center), m_radius(radius), m_material(mat), m_name("Planet") {}

sphere::sphere(point3 center, double radius, shared_ptr<material> mat, std::string name)
    : m_center(center), m_radius(radius), m_material(mat), m_name(name) {}

bool sphere::hit(const ray &r, double minT, double maxT, hitData &data) const {
    vec3 displacement = r.origin() - m_center;

    auto a = r.direction().lengthSquared();
    auto b = dot(displacement, r.direction()); //< it's actually 0.5 * b
    auto c = displacement.lengthSquared() - m_radius * m_radius;

    double discriminant = b * b - a * c;

    if (discriminant < 0)
        return false;

    auto sqrtDiscriminant = std::sqrt(discriminant);

    auto closestHitPointDistance = (-b - sqrtDiscriminant) / a;
    if (closestHitPointDistance < minT || maxT < closestHitPointDistance) {
        closestHitPointDistance = (-b + sqrtDiscriminant) / a;
        if (closestHitPointDistance < minT || maxT < closestHitPointDistance)
            return false;
    }

    data.t = closestHitPointDistance;
    data.p = r.at(data.t);
    vec3 outwardNormal = (data.p - m_center) / m_radius;
    data.setFaceNormal(r, outwardNormal);
    getSphericalUV(outwardNormal, data.u, data.v);
    data.hitMaterial = m_material;

    return true;
}

shared_ptr<material> sphere::getMaterial() const { return m_material; }
void sphere::setMaterial(shared_ptr<material> material) { m_material = material; }

std::string sphere::getName() const { return m_name; }
void sphere::setName(std::string name) { m_name = name; }

point3 sphere::getOrigin() const { return m_center; }
void sphere::setOrigin(point3 p) { m_center = p; }

planetSettings sphere::getSettings() const {
    planetSettings settings;

    settings.remove = false;
    settings.origin = m_center;
    settings.radius = m_radius;
    settings.mat = m_material;
    settings.name = m_name;

    return settings;
}

void sphere::setSettings(planetSettings &settings) {
    m_center = settings.origin;
    m_radius = settings.radius;
    m_material = settings.mat;
    m_name = settings.name;
}

void sphere::getSphericalUV(const point3 &p, double &u, double &v) {
    auto theta = acos(-p.y());
    auto phi = atan2(-p.z(), p.x()) + PI;

    u = phi / (2 * PI);
    v = theta / PI;
}

} // namespace shkyera
