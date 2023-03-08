#ifndef IMAGE_H
#define IMAGE_H

#include "core/utils.hpp"

#include <vector>

namespace shkyera {

class image {
  public:
    image(int width, int height);

    int width() const;
    int height() const;

    std::vector<int> &verticalPixels();
    std::vector<int> &horizontalPixels();

    void clear();
    void writeImage(std::ostream &out) const;

    void scaleImage(std::shared_ptr<image> destinationImage);

    color &operator()(int x, int y);
    color &at(int x, int y);

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
