#ifndef VEC3_H
#define VEC3_H

#include <iostream>

namespace fg {

class vec3 {
  public:
    vec3();
    vec3(double x, double y, double z);

    double x() const;
    double y() const;
    double z() const;

    vec3 operator-() const;
    double operator[](int i) const;
    double &operator[](int i);

    vec3 &operator+=(const vec3 &v);
    vec3 &operator*=(const double k);
    vec3 &operator/=(const double k);

    double length() const;
    double lengthSquared() const;

  private:
    double m_cords[3];
};

using point3 = vec3;
using color = vec3;

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z() << "\n";
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline vec3 operator*(double k, const vec3 &v) {
    return vec3(k * v.x(), k * v.y(), k * v.z());
}

inline vec3 operator*(const vec3 &v, double k) { return k * v; }

inline vec3 operator/(const vec3 &v, double k) { return (1 / k) * v; }

inline double dot(const vec3 &u, const vec3 &v) {
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(),
                u.x() * v.y() - u.y() * v.x());
}

inline vec3 unitVector(const vec3 &v) { return v / v.length(); }

} // namespace fg

#endif
