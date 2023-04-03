#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

#include "core/utils.hpp"

namespace shkyera {

class vec3 {
  public:
    vec3();
    vec3(double x, double y, double z);

    float *getCords();

    double x() const;
    double y() const;
    double z() const;

    vec3 absolute();

    vec3 operator-() const;
    double operator[](int i) const;
    double &operator[](int i);

    vec3 &operator+=(const vec3 &v);
    vec3 &operator*=(const double k);
    vec3 &operator/=(const double k);

    void rotateAroundY(double angle);
    void rotateUpAndDown(double angle);

    double length() const;
    double lengthSquared() const;

    static vec3 random();
    static vec3 random(double min, double max);
    static vec3 sqrt(const vec3 &v);

    static double max(const vec3 &v);

  private:
    double m_cords[3];
};

using point3 = vec3;
using color = vec3;

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z() << std::endl;
}

inline bool operator==(const vec3 &u, const vec3 &v) {
    return std::abs(u[0] - v[0]) < std::numeric_limits<float>::epsilon() &&
           std::abs(u[1] - v[1]) < std::numeric_limits<float>::epsilon() &&
           std::abs(u[2] - v[2]) < std::numeric_limits<float>::epsilon();
}

inline bool operator!=(const vec3 &u, const vec3 &v) { return !(u == v); }

inline vec3 operator+(const vec3 &u, const vec3 &v) { return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z()); }

inline vec3 operator-(const vec3 &u, const vec3 &v) { return vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z()); }

inline vec3 operator*(const vec3 &u, const vec3 &v) { return vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z()); }

inline vec3 operator*(double k, const vec3 &v) { return vec3(k * v.x(), k * v.y(), k * v.z()); }

inline vec3 operator*(const vec3 &v, double k) { return k * v; }

inline vec3 operator/(const vec3 &v, double k) { return (1 / k) * v; }

inline double dot(const vec3 &u, const vec3 &v) { return u.x() * v.x() + u.y() * v.y() + u.z() * v.z(); }

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(), u.x() * v.y() - u.y() * v.x());
}

inline vec3 clamp(const vec3 &u, double min, double max) {
    vec3 v;

    v[0] = clamp(u[0], 0, 1);
    v[1] = clamp(u[1], 0, 1);
    v[2] = clamp(u[2], 0, 1);

    return v;
}

inline vec3 unitVector(const vec3 &v) { return v / v.length(); }

inline vec3 randomInUnitSphere() {
    while (true) {
        vec3 v = vec3::random(-1, 1);
        if (v.lengthSquared() >= 1)
            continue;
        return v;
    }
}

inline vec3 randomInUnitHemisphere(const vec3 &normal) {
    vec3 v = randomInUnitSphere();
    if (dot(v, normal) > 0)
        return v;
    else
        return -v;
}

inline vec3 randomUnitVector() { return unitVector(randomInUnitSphere()); }

inline vec3 reflect(const vec3 &v, const vec3 &normal) { return v - 2 * dot(v, normal) * normal; }

inline vec3 refract(const vec3 &u, const vec3 &normal, double refractionRatio) {
    auto cosTheta = fmin(dot(-u, normal), 1.0);
    vec3 outParpendicular = refractionRatio * (u + cosTheta * normal);
    vec3 outParallel = -sqrt(fabs(1.0 - outParpendicular.lengthSquared())) * normal;

    return outParpendicular + outParallel;
}

} // namespace shkyera

#endif
