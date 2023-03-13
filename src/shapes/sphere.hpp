#ifndef SPHERE_H
#define SPHERE_H

#include "shapes/hittable.hpp"

namespace shkyera {

class sphere : public hittable {
  public:
    sphere(point3 center, double radius, shared_ptr<material> mat);
    sphere(point3 center, double radius, shared_ptr<material> mat, std::string name);

    virtual bool hit(const ray &r, double minT, double maxT, hitData &data) const override;

    shared_ptr<material> getMaterial();
    void setMaterial(shared_ptr<material> material);

    std::string getName();
    void setName(std::string name);

  private:
    static void getSphericalUV(const point3 &p, double &u, double &v);

    point3 m_center;
    double m_radius;
    shared_ptr<material> m_material;

    std::string m_name;
};

} // namespace shkyera

#endif
