#ifndef IMAGE_H
#define IMAGE_H

#include "core/ray.hpp"
#include "core/utils.hpp"
#include "core/vec3.hpp"

#include <vector>

namespace shkyera {

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

    void scaleImage(std::shared_ptr<image> destinationImage);

    color &operator()(int x, int y);
    color &at(int x, int y);

    static constexpr int TEXTURE_BYTES_PER_PIXEL = 3;

    static std::shared_ptr<image> EARTH_DAY_TEXTURE;
    static std::shared_ptr<image> EARTH_NIGHT_TEXTURE;
    static std::shared_ptr<image> MARS_TEXTURE;
    static std::shared_ptr<image> SUN_TEXTURE;
    static std::shared_ptr<image> MOON_TEXTURE;
    static std::shared_ptr<image> STARS_TEXTURE;

  private:
    void writeColor(std::ostream &out, color pixelColor) const;

    int m_width;
    int m_height;

    std::vector<std::vector<color>> m_data;

    std::vector<int> m_verticalPixels;
    std::vector<int> m_horizontalPixels;
};

} // namespace shkyera

#endif
