#ifndef HITTABLE_WORLD_H
#define HITTABLE_WORLD_H

#include "core/ray.hpp"
#include "core/utils.hpp"
#include "core/vec3.hpp"
#include "shapes/sphere.hpp"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

namespace shkyera {

class hittableWorld {
  public:
    hittableWorld();

    void clear();
    void add(shared_ptr<sphere> object);

    std::vector<shared_ptr<sphere>> getObjects();
    std::shared_ptr<sphere> getUniverse();
    size_t size() const;

    bool hit(const ray &r, double minT, double maxT, hitData &data) const;

    void setAmbientLightColor(color c);
    color getAmbientLightColor();

  private:
    std::vector<shared_ptr<sphere>> m_objects;
    std::shared_ptr<sphere> m_universe;

    color m_ambientLightColor;
};

} // namespace shkyera

#endif
