#include "vec3.hpp"

#include <cmath>

namespace fg {

vec3::vec3() : m_cords{0, 0, 0} {}
vec3::vec3(double x, double y, double z) : m_cords{x, y, z} {}

double vec3::x() const { return m_cords[0]; }
double vec3::y() const { return m_cords[1]; }
double vec3::z() const { return m_cords[2]; }

vec3 vec3::operator-() const {
    return vec3(-m_cords[0], -m_cords[1], -m_cords[2]);
}

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

double vec3::length() const { return std::sqrt(lengthSquared()); }
double vec3::lengthSquared() const {
    return m_cords[0] * m_cords[0] + m_cords[1] * m_cords[1] +
           m_cords[2] * m_cords[2];
}

} // namespace fg
