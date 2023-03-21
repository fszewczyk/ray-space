#ifndef PLOT_VIEW_TAB_H
#define PLOT_VIEW_TAB_H

#include "world/camera.hpp"
#include "world/visibleWorld.hpp"

namespace shkyera {

enum PLANE { XY, XZ, YZ };

class plotViewTab {
  public:
    plotViewTab(std::shared_ptr<visibleWorld> world, std::shared_ptr<camera> cam, PLANE plane);

    systemSettings render(bool &updated);

  private:
    void plotDummyPoints();
    void plotCamera();
    void plotPlanets();

    void labelAxes();

    std::pair<double, double> getPositionOnPlane(point3 point);

    std::shared_ptr<visibleWorld> m_world;
    std::shared_ptr<camera> m_camera;

    PLANE m_plane;
};

} // namespace shkyera

#endif
