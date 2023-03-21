#ifndef IMAGE_H
#define IMAGE_H

#define GLFW_INCLUDE_NONE

#include <vector>

#include "core/ray.hpp"
#include "core/utils.hpp"
#include "core/vec3.hpp"

namespace shkyera {

enum IMAGE_TEXTURE_TYPE {
    NONE,
    EARTH_DAY,
    EARTH_NIGHT,
    MARS,
    SUN,
    MOON,
    CERES,
    CLOUDY_VENUS,
    ERIS,
    HAUMEA,
    JUPITER,
    MAKE,
    MERCURY,
    NEPTUNE,
    SATURN,
    URANUS,
    VENUS
};

class image {
  public:
    image();
    image(int width, int height);
    image(const char *filename);

    ~image() = default;

    int width() const;
    int height() const;

    bool isEmpty() const;

    std::vector<int> &verticalPixels();
    std::vector<int> &horizontalPixels();

    void clear();
    void writeImage(std::ostream &out) const;

    color getColor() const;

    void scaleImage(std::shared_ptr<image> destinationImage, bool uniformScaling = false);

    void updateTextureId();
    uint64_t getTextureId() const;

    color &operator()(int x, int y);
    color &at(int x, int y);

    static constexpr int TEXTURE_BYTES_PER_PIXEL = 3;
    static constexpr int ICON_SIZE = 64;

    static std::shared_ptr<image> EARTH_DAY_TEXTURE;
    static std::shared_ptr<image> EARTH_NIGHT_TEXTURE;
    static std::shared_ptr<image> MARS_TEXTURE;
    static std::shared_ptr<image> SUN_TEXTURE;
    static std::shared_ptr<image> MOON_TEXTURE;
    static std::shared_ptr<image> STARS_TEXTURE;
    static std::shared_ptr<image> CERES_TEXTURE;
    static std::shared_ptr<image> CLOUDY_VENUS_TEXTURE;
    static std::shared_ptr<image> ERIS_TEXTURE;
    static std::shared_ptr<image> HAUMEA_TEXTURE;
    static std::shared_ptr<image> JUPITER_TEXTURE;
    static std::shared_ptr<image> MAKE_TEXTURE;
    static std::shared_ptr<image> MERCURY_TEXTURE;
    static std::shared_ptr<image> NEPTUNE_TEXTURE;
    static std::shared_ptr<image> SATURN_TEXTURE;
    static std::shared_ptr<image> URANUS_TEXTURE;
    static std::shared_ptr<image> VENUS_TEXTURE;

    inline static std::shared_ptr<image> ICON_EARTH_DAY_TEXTURE = std::make_shared<image>(ICON_SIZE, ICON_SIZE);
    inline static std::shared_ptr<image> ICON_EARTH_NIGHT_TEXTURE = std::make_shared<image>(ICON_SIZE, ICON_SIZE);
    inline static std::shared_ptr<image> ICON_MARS_TEXTURE = std::make_shared<image>(ICON_SIZE, ICON_SIZE);
    inline static std::shared_ptr<image> ICON_SUN_TEXTURE = std::make_shared<image>(ICON_SIZE, ICON_SIZE);
    inline static std::shared_ptr<image> ICON_MOON_TEXTURE = std::make_shared<image>(ICON_SIZE, ICON_SIZE);
    inline static std::shared_ptr<image> ICON_CERES_TEXTURE = std::make_shared<image>(ICON_SIZE, ICON_SIZE);
    inline static std::shared_ptr<image> ICON_CLOUDY_VENUS_TEXTURE = std::make_shared<image>(ICON_SIZE, ICON_SIZE);
    inline static std::shared_ptr<image> ICON_ERIS_TEXTURE = std::make_shared<image>(ICON_SIZE, ICON_SIZE);
    inline static std::shared_ptr<image> ICON_HAUMEA_TEXTURE = std::make_shared<image>(ICON_SIZE, ICON_SIZE);
    inline static std::shared_ptr<image> ICON_JUPITER_TEXTURE = std::make_shared<image>(ICON_SIZE, ICON_SIZE);
    inline static std::shared_ptr<image> ICON_MAKE_TEXTURE = std::make_shared<image>(ICON_SIZE, ICON_SIZE);
    inline static std::shared_ptr<image> ICON_MERCURY_TEXTURE = std::make_shared<image>(ICON_SIZE, ICON_SIZE);
    inline static std::shared_ptr<image> ICON_NEPTUNE_TEXTURE = std::make_shared<image>(ICON_SIZE, ICON_SIZE);
    inline static std::shared_ptr<image> ICON_SATURN_TEXTURE = std::make_shared<image>(ICON_SIZE, ICON_SIZE);
    inline static std::shared_ptr<image> ICON_URANUS_TEXTURE = std::make_shared<image>(ICON_SIZE, ICON_SIZE);
    inline static std::shared_ptr<image> ICON_VENUS_TEXTURE = std::make_shared<image>(ICON_SIZE, ICON_SIZE);

  private:
    void writeColor(std::ostream &out, color pixelColor) const;

    int m_width;
    int m_height;

    std::vector<std::vector<color>> m_data;

    std::vector<int> m_verticalPixels;
    std::vector<int> m_horizontalPixels;

    uint64_t m_textureId;
    bool m_assignedTextureId;

    color m_dominantColor;
};

} // namespace shkyera

#endif
