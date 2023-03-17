#ifndef SPHERE_H
#define SPHERE_H

#include "shapes/material.hpp"

namespace shkyera {

class material;

struct hitData {
    point3 p;
    vec3 normal;
    shared_ptr<material> hitMaterial;
    double t;
    double u;
    double v;
    bool frontFace;

    inline void setFaceNormal(const ray &r, const vec3 &outwardNormal) {
        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

struct planetSettings {
    bool remove;

    point3 origin;
    double radius;
    shared_ptr<material> mat;
    std::string name;

    bool operator==(const planetSettings &a) {
        return a.origin == origin && std::abs(a.radius - radius) < std::numeric_limits<float>::epsilon() &&
               a.remove == remove && a.mat == mat;
    }
};

class sphere {
  public:
    sphere(point3 center, double radius, shared_ptr<material> mat);
    sphere(point3 center, double radius, shared_ptr<material> mat, std::string name);

    bool hit(const ray &r, double minT, double maxT, hitData &data) const;

    std::string getName() const;
    void setName(std::string name);

    point3 getOrigin() const;
    void setOrigin(point3 p);

    shared_ptr<material> getMaterial() const;
    void setMaterial(shared_ptr<material> material);

    planetSettings getSettings() const;
    void setSettings(planetSettings &settings);

    static constexpr size_t MAX_NAME_LENGTH = 30;

  private:
    static void getSphericalUV(const point3 &p, double &u, double &v);

    point3 m_center;
    double m_radius;
    shared_ptr<material> m_material;

    std::string m_name;
};

} // namespace shkyera

#endif
