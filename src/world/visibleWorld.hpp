#ifndef HITTABLE_WORLD_H
#define HITTABLE_WORLD_H

#include "core/ray.hpp"
#include "core/utils.hpp"
#include "core/vec3.hpp"
#include "shapes/sphere.hpp"
#include "world/camera.hpp"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

namespace shkyera {

struct worldSettings {
    color ambientColor;
    std::vector<planetSettings> planets;
    std::vector<bool> updatedPlanets;
};

struct systemSettings {
    worldSettings world;
    cameraSettings cam;
};

class visibleWorld {
  public:
    visibleWorld();

    void clear();
    void add(shared_ptr<sphere> object);

    std::vector<shared_ptr<sphere>> getObjects() const;
    std::shared_ptr<sphere> getUniverse() const;
    std::shared_ptr<sphere> getObjectByIndex(size_t i) const;
    size_t size() const;

    bool hit(const ray &r, double minT, double maxT, hitData &data) const;

    worldSettings getSettings() const;
    void setSettings(worldSettings &settings);

    color getAmbientLightColor() const;
    void setAmbientLightColor(color c);

  private:
    std::vector<shared_ptr<sphere>> m_objects;
    std::shared_ptr<sphere> m_universe;

    color m_ambientLightColor;
};

} // namespace shkyera

#endif
