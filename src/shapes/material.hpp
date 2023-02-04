#ifndef MATERIAL_H
#define MATERIAL_H

#include "core/texture.hpp"
#include "core/utils.hpp"
#include "shapes/hittable.hpp"

namespace shkyera {

class material {
  public:
    virtual bool scatter(const ray &rayIn, const hitData &data,
                         color &attenuation, ray &rayOut) const = 0;
};

class lambertian : public material {
  public:
    lambertian(const color &c);
    lambertian(shared_ptr<texture> c);

    virtual bool scatter(const ray &rayIn, const hitData &data,
                         color &attenuation, ray &rayOut) const override;

  private:
    shared_ptr<texture> m_albedo;
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

class refractor : public material {
  public:
    refractor(double eta);

    virtual bool scatter(const ray &rayIn, const hitData &data,
                         color &attenuation, ray &rayOut) const override;

  private:
    static double reflectance(double cosine, double eta);

    double m_eta;
};

} // namespace shkyera

#endif
