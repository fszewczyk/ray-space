#include "shapes/sphere.hpp"

#include <cmath>

namespace shkyera {

sphere::sphere(point3 center, double radius, shared_ptr<material> mat)
    : m_center(center), m_radius(radius), m_material(mat) {}

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
    data.material = m_material;

    return true;
}

} // namespace shkyera
