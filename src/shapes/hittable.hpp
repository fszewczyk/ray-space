#ifndef HITTABLE_H
#define HITTABLE_H

#include "core/ray.hpp"
#include "core/utils.hpp"

namespace shkyera {

class material;

struct hitData {
    point3 p;
    vec3 normal;
    shared_ptr<material> hitMaterial;
    double t;
    bool frontFace;

    inline void setFaceNormal(const ray &r, const vec3 &outwardNormal) {
        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class hittable {
  public:
    virtual bool hit(const ray &r, double minT, double maxT,
                     hitData &data) const = 0;
};

} // namespace shkyera

#endif
