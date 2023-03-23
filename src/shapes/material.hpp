#ifndef MATERIAL_H
#define MATERIAL_H

#include "core/ray.hpp"
#include "core/texture.hpp"
#include "core/utils.hpp"
#include "core/vec3.hpp"
#include "shapes/sphere.hpp"

namespace shkyera {

typedef struct hitData hitData;

enum MATERIAL_TYPE { LAMBERTIAN, METAL, REFRACTOR, DIFFUSE_LIGHT };

class material {
public:
  virtual bool scatter(const ray &rayIn, const hitData &data,
                       color &attenuation, ray &rayOut) const = 0;
  virtual color emit(double u, double v, const point3 &p, bool firstHit) const;
  virtual std::shared_ptr<solidColor> getLightMaterial() const;
  virtual color getVisibleColor() const;
};

class lambertian : public material {
public:
  lambertian(const color &c);
  lambertian(shared_ptr<texture> c);

  static std::shared_ptr<lambertian>
  generateFromImage(std::shared_ptr<image> im);
  static std::shared_ptr<lambertian>
  generateFromImageTextureType(int imageTextureType);

  virtual bool scatter(const ray &rayIn, const hitData &data,
                       color &attenuation, ray &rayOut) const override;
  virtual color getVisibleColor() const override;

private:
  shared_ptr<texture> m_albedo;
};

class metal : public material {
public:
  metal(const color &c, double f);

  virtual bool scatter(const ray &rayIn, const hitData &data,
                       color &attenuation, ray &rayOut) const override;
  virtual color getVisibleColor() const override;

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

class diffuseLight : public material {
public:
  diffuseLight(shared_ptr<texture> text, color lightColor);
  diffuseLight(color even, color odd, color lightColor);
  diffuseLight(color displayColor, color lightColor);

  static std::shared_ptr<diffuseLight>
  generateFromImage(std::shared_ptr<image> im, color c);
  static std::shared_ptr<diffuseLight>
  generateFromImageTextureType(IMAGE_TEXTURE_TYPE imageTextureType, color c);

  virtual bool scatter(const ray &rayIn, const hitData &data,
                       color &attenuation, ray &rayOut) const override;
  virtual color emit(double u, double v, const point3 &p,
                     bool firstHit) const override;

  virtual std::shared_ptr<solidColor> getLightMaterial() const override;
  virtual color getVisibleColor() const override;

private:
  shared_ptr<texture> m_textureToDisplay;
  shared_ptr<solidColor> m_lightColor;
};

} // namespace shkyera

#endif
