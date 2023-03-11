#include "core/vec3.hpp"
#include "core/ray.hpp"
#include "core/utils.hpp"
#include "core/vec3.hpp"

#include <cmath>

namespace shkyera {

vec3::vec3() : m_cords{0, 0, 0} {}
vec3::vec3(double x, double y, double z) : m_cords{x, y, z} {}

double vec3::x() const { return m_cords[0]; }
double vec3::y() const { return m_cords[1]; }
double vec3::z() const { return m_cords[2]; }

vec3 vec3::operator-() const { return vec3(-m_cords[0], -m_cords[1], -m_cords[2]); }

double vec3::operator[](int i) const { return m_cords[i]; }
double &vec3::operator[](int i) { return m_cords[i]; }

vec3 &vec3::operator+=(const vec3 &v) {
    m_cords[0] += v.x();
    m_cords[1] += v.y();
    m_cords[2] += v.z();

    return *this;
}

vec3 &vec3::operator*=(const double k) {
    m_cords[0] *= k;
    m_cords[1] *= k;
    m_cords[2] *= k;

    return *this;
}
vec3 &vec3::operator/=(const double k) {
    m_cords[0] /= k;
    m_cords[1] /= k;
    m_cords[2] /= k;

    return *this;
}

void vec3::rotateAroundY(double angle) {
    double new_x = m_cords[0] * cos(angle) + m_cords[2] * sin(angle);
    double new_z = -m_cords[0] * sin(angle) + m_cords[2] * cos(angle);

    m_cords[0] = new_x;
    m_cords[2] = new_z;
}

void vec3::rotateUpAndDown(double angle) {
    float baseAngle = atan2(m_cords[0], m_cords[2]);
    rotateAroundY(baseAngle);

    double new_x = m_cords[0] * cos(angle) - m_cords[1] * sin(angle);
    double new_y = m_cords[0] * sin(angle) + m_cords[1] * cos(angle);

    m_cords[0] = new_x;
    m_cords[1] = new_y;

    rotateAroundY(-baseAngle);
}

double vec3::length() const { return std::sqrt(lengthSquared()); }

double vec3::max(const vec3 &v) {
    double maxValue = std::max(v[0], v[1]);
    return std::max(maxValue, v[2]);
}

double vec3::lengthSquared() const {
    return m_cords[0] * m_cords[0] + m_cords[1] * m_cords[1] + m_cords[2] * m_cords[2];
}

vec3 vec3::random() { return vec3(randomDouble(), randomDouble(), randomDouble()); }

vec3 vec3::random(double min, double max) {
    return vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
}

vec3 vec3::sqrt(const vec3 &v) { return vec3(std::sqrt(v.x()), std::sqrt(v.y()), std::sqrt(v.z())); }

} // namespace shkyera
