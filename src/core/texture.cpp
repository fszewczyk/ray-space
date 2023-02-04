#include "texture.hpp"

namespace shkyera {

solidColor::solidColor(color c) : m_color(c) {}
solidColor::solidColor(double r, double g, double b)
    : m_color(color(r, g, b)) {}

color solidColor::value(double u, double v, const point3 &p) const {
    return m_color;
}

checkerTexture::checkerTexture(color even, color odd)
    : checkerTexture(even, odd, 10) {}

checkerTexture::checkerTexture(shared_ptr<texture> even,
                               shared_ptr<texture> odd)
    : checkerTexture(even, odd, 10) {}

checkerTexture::checkerTexture(color even, color odd, double grain)
    : m_even(make_shared<solidColor>(even)),
      m_odd(make_shared<solidColor>(odd)), m_grain(grain) {}

checkerTexture::checkerTexture(shared_ptr<texture> even,
                               shared_ptr<texture> odd, double grain)
    : m_even(even), m_odd(odd), m_grain(grain) {}

color checkerTexture::value(double u, double v, const point3 &p) const {
    auto sins =
        sin(m_grain * p.x()) * sin(m_grain * p.y()) * sin(m_grain * p.z());
    if (sins < 0)
        return m_odd->value(u, v, p);
    else
        return m_even->value(u, v, p);
}

} // namespace shkyera
