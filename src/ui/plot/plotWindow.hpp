#ifndef PLOT_WINDOW_H
#define PLOT_WINDOW_H

#include "ui/plot/plotViewTab.hpp"
#include "world/camera.hpp"
#include "world/visibleWorld.hpp"

namespace shkyera {

class plotWindow {
  public:
    plotWindow(std::shared_ptr<visibleWorld> world, std::shared_ptr<camera> cam);

    systemSettings render(bool &updated);

  private:
    plotViewTab m_topViewTab;
    plotViewTab m_sideViewTab;
    plotViewTab m_frontViewTab;
};

} // namespace shkyera

#endif
