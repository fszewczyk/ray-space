#ifndef SPHERE_H
#define SPHERE_H

#include "shapes/hittable.hpp"

namespace shkyera {

class sphere : public hittable {
  public:
    sphere();
    sphere(point3 center, double radius, shared_ptr<material> mat);

    virtual bool hit(const ray &r, double minT, double maxT, hitData &data) const override;

    void setMaterial(shared_ptr<material> material);

  private:
    static void getSphericalUV(const point3 &p, double &u, double &v);

    point3 m_center;
    double m_radius;
    shared_ptr<material> m_material;
};

} // namespace shkyera

#endif
