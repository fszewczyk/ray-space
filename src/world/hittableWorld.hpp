#ifndef HITTABLE_WORLD_H
#define HITTABLE_WORLD_H

#include "core/ray.hpp"
#include "core/utils.hpp"
#include "core/vec3.hpp"
#include "shapes/hittable.hpp"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

namespace shkyera {

class hittableWorld : public hittable {
  public:
    hittableWorld() = default;
    hittableWorld(shared_ptr<hittable> object);

    void clear();
    void add(shared_ptr<hittable> object);

    std::vector<shared_ptr<hittable>> objects();
    size_t size() const;

    virtual bool hit(const ray &r, double minT, double maxT, hitData &data) const override;

  private:
    std::vector<shared_ptr<hittable>> m_objects;
};

} // namespace shkyera

#endif
