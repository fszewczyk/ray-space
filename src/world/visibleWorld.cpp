#include "world/visibleWorld.hpp"
#include "core/image.hpp"
#include "shapes/material.hpp"

namespace shkyera {

visibleWorld::visibleWorld() : m_ambientLightColor(0.15, 0.15, 0.15) {
    auto starsMaterial = diffuseLight::generateFromImage(image::STARS_TEXTURE, m_ambientLightColor);
    m_universe = make_shared<sphere>(point3(0, 0, 0), 100000.0, starsMaterial);
    add(m_universe);
}

void visibleWorld::clear() { m_objects.clear(); }
void visibleWorld::add(shared_ptr<sphere> object) {
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

std::vector<shared_ptr<sphere>> visibleWorld::getObjects() const { return m_objects; }
std::shared_ptr<sphere> visibleWorld::getUniverse() const { return m_universe; }
std::shared_ptr<sphere> visibleWorld::getObjectByIndex(size_t i) const { return m_objects[i + 1]; }

size_t visibleWorld::size() const { return m_objects.size(); }

bool visibleWorld::hit(const ray &r, double minT, double maxT, hitData &data) const {
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

worldSettings visibleWorld::getSettings() const {
    worldSettings settings;
    settings.planets.resize(getObjects().size());
    settings.updatedPlanets.resize(getObjects().size());

    settings.ambientColor = getAmbientLightColor();

    return settings;
}

void visibleWorld::setSettings(worldSettings &settings) {
    setAmbientLightColor(settings.ambientColor);

    for (size_t i = 0; i < settings.planets.size(); ++i) {
        if (settings.updatedPlanets[i]) {
            if (settings.planets[i].remove) {
                m_objects.erase(m_objects.begin() + i + 1);
            } else if (i < m_objects.size() - 1) {
                getObjectByIndex(i)->setSettings(settings.planets[i]);
            } else {
                auto planet = make_shared<sphere>(settings.planets[i].origin, settings.planets[i].radius,
                                                  settings.planets[i].mat, settings.planets[i].name);
                add(planet);
            }
        }
    }
}

void visibleWorld::setAmbientLightColor(color c) {
    m_ambientLightColor = c;

    auto newAmbientLightMaterial = diffuseLight::generateFromImage(image::STARS_TEXTURE, m_ambientLightColor);
    m_universe->setMaterial(newAmbientLightMaterial);
}

color visibleWorld::getAmbientLightColor() const { return m_ambientLightColor; }

} // namespace shkyera
