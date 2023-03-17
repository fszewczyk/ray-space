#ifndef TEXTURE_H
#define TEXTURE_H

#include "core/image.hpp"
#include "core/ray.hpp"
#include "core/vec3.hpp"

namespace shkyera {

enum TEXTURE_TYPE { SOLID_COLOR, IMAGE };

class texture {
  public:
    virtual color value(double u, double v, const point3 &p) const = 0;
    virtual color getColor() const;
};

class solidColor : public texture {
  public:
    solidColor() = default;
    solidColor(color c);
    solidColor(double r, double g, double b);

    virtual color value(double u, double v, const point3 &p) const override;

    virtual color getColor() const override;
    void setColor(color c);

  private:
    color m_color;
};

class checkerTexture : public texture {
  public:
    checkerTexture() = default;
    checkerTexture(color even, color odd);
    checkerTexture(shared_ptr<texture> even, shared_ptr<texture> odd);
    checkerTexture(color even, color odd, double grain);
    checkerTexture(shared_ptr<texture> even, shared_ptr<texture> odd, double grain);

    virtual color value(double u, double v, const point3 &p) const override;
    virtual color getColor() const override;

  private:
    shared_ptr<texture> m_even;
    shared_ptr<texture> m_odd;

    double m_grain;
};

class imageTexture : public texture {
  public:
    imageTexture() = default;
    imageTexture(std::shared_ptr<image> image);

    ~imageTexture() = default;

    virtual color value(double u, double v, const point3 &p) const override;
    virtual color getColor() const override;

  private:
    std::shared_ptr<image> m_image;
};

} // namespace shkyera

#endif
