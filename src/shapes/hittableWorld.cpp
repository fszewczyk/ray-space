#include "shapes/hittableWorld.hpp"

namespace fg {

hittableWorld::hittableWorld(shared_ptr<hittable> object) { add(object); }

void hittableWorld::clear() { m_objects.clear(); }
void hittableWorld::add(shared_ptr<hittable> object) {
    m_objects.push_back(object);
}

bool hittableWorld::hit(const ray &r, double minT, double maxT,
                        hitData &data) const {
    hitData tempData;
    bool hitAnything = false;
    auto closestObject = maxT;

    for (const auto &object : m_objects) {
        if (object->hit(r, minT, closestObject, tempData)) {
            hitAnything = true;
            closestObject = tempData.t;
            data = tempData;
        }
    }

    return hitAnything;
}

} // namespace fg
