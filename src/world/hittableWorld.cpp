#include "world/hittableWorld.hpp"
#include "core/image.hpp"
#include "shapes/material.hpp"

namespace shkyera {

hittableWorld::hittableWorld() : m_ambientLightColor(0.15, 0.15, 0.15) {
    auto starsMaterial = diffuseLight::generateFromImage(image::STARS_TEXTURE, m_ambientLightColor);
    m_universe = make_shared<sphere>(point3(0, 0, 0), 100000.0, starsMaterial);
    add(m_universe);
}

void hittableWorld::clear() { m_objects.clear(); }
void hittableWorld::add(shared_ptr<sphere> object) {
    std::string name = object->getName();

    bool uniqueName = true;
    int count = 0;
    do {
        uniqueName = true;
        for (auto existingObject : m_objects) {
            if (existingObject == m_universe)
                continue;

            if ((count == 0 && existingObject->getName() == name) ||
                existingObject->getName() == name + " " + std::to_string(count)) {
                uniqueName = false;
                count++;
            }
        }
    } while (!uniqueName);

    if (count)
        object->setName(name + " " + std::to_string(count));

    m_objects.push_back(object);
}

std::vector<shared_ptr<sphere>> hittableWorld::getObjects() { return m_objects; }
std::shared_ptr<sphere> hittableWorld::getUniverse() { return m_universe; }

size_t hittableWorld::size() const { return m_objects.size(); }

bool hittableWorld::hit(const ray &r, double minT, double maxT, hitData &data) const {
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

void hittableWorld::setAmbientLightColor(color c) {
    m_ambientLightColor = c;

    auto newAmbientLightMaterial = diffuseLight::generateFromImage(image::STARS_TEXTURE, m_ambientLightColor);
    m_universe->setMaterial(newAmbientLightMaterial);
}

color hittableWorld::getAmbientLightColor() { return m_ambientLightColor; }

} // namespace shkyera
