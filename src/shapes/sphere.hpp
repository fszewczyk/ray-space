#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.hpp"

namespace fg {

class sphere : public hittable {
  public:
    sphere();
    sphere(point3 center, double radius);

    virtual bool hit(const ray &r, double minT, double maxT,
                     hitData &data) const override;

  private:
    point3 m_center;
    double m_radius;
};

} // namespace fg

#endif
