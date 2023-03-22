#ifndef EXPORT_SETTINGS_WINDOW_H
#define EXPORT_SETTINGS_WINDOW_H

#include "world/camera.hpp"

namespace shkyera {

enum RENDER_FILE_EXTENSION { JPG, PNG };
enum RENDER_MODE { EDIT, PREVIEW, EXPORT };

struct exportSettings {
    int width;
    int height;
    int maximumRayDepth;
    int raysPerPixel;

    RENDER_FILE_EXTENSION extension;

    bool lockAspectRatio;
};

class exportSettingsWindow {
  public:
    exportSettingsWindow(std::shared_ptr<camera> cam);

    exportSettings render(RENDER_MODE &mode);

    inline static const std::string FILE_EXTENSIONS[2] = {"JPG", "PNG"};

  private:
    exportSettings getDefaultExportSettings();

    std::shared_ptr<camera> m_camera;
};

} // namespace shkyera

#endif
