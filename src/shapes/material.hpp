#ifndef MATERIAL_H
#define MATERIAL_H

#include "core/utils.hpp"
#include "shapes/hittable.hpp"

namespace fg {

class material {
  public:
    virtual bool scatter(const ray &rayIn, const hitData &data,
                         color &attenuation, ray &rayOut) const = 0;
};

class lambertian : public material {
  public:
    lambertian(const color &c);

    virtual bool scatter(const ray &rayIn, const hitData &data,
                         color &attenuation, ray &rayOut) const override;

  private:
    color m_albedo;
};

class metal : public material {
  public:
    metal(const color &c, double f);

    virtual bool scatter(const ray &rayIn, const hitData &data,
                         color &attenuation, ray &rayOut) const override;

  private:
    color m_albedo;
    double m_fuzz;
};

} // namespace fg

#endif
