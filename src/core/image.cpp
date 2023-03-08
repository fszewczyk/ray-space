#include "core/image.hpp"

#include <cmath>
#include <numeric>

namespace shkyera {

image::image(int width, int height)
    : m_width(width), m_height(height), m_data(height, std::vector<color>(width)), m_verticalPixels(height),
      m_horizontalPixels(width) {
    std::iota(m_verticalPixels.begin(), m_verticalPixels.end(), 0);
    std::iota(m_horizontalPixels.begin(), m_horizontalPixels.end(), 0);
}

void image::writeImage(std::ostream &out) const {
    out << "P3\n" << m_width << ' ' << m_height << "\n255\n";

    for (const auto &row : m_data) {
        for (const auto &pixel : row) {
            writeColor(out, pixel);
        }
    }
}

void image::scaleImage(std::shared_ptr<image> destinationImage) {
    float scaleHorizontal = static_cast<float>(width()) / destinationImage->width();
    float scaleVertical = static_cast<float>(height()) / destinationImage->height();

    for (int y = 0; y < destinationImage->height(); ++y) {
        for (int x = 0; x < destinationImage->width(); ++x) {
            int lx = floor(scaleHorizontal * x);
            int ly = floor(scaleVertical * y);
            int hx = ceil(scaleHorizontal * x);
            int hy = ceil(scaleVertical * y);

            lx = std::max(0, lx);
            ly = std::max(0, ly);
            hx = std::min(hx, width() - 1);
            hy = std::min(hy, height() - 1);

            float wx = scaleHorizontal * x - lx;
            float wy = scaleVertical * y - ly;

            color topLeft = this->at(lx, ly);
            color topRight = this->at(hx, ly);
            color bottomLeft = this->at(lx, hy);
            color bottomRight = this->at(hx, hy);

            destinationImage->at(x, y) = topLeft * (1 - wx) * (1 - wy) + topRight * wx * (1 - wy) +
                                         bottomLeft * wy * (1 - wx) + bottomRight * wx * wy;
        }
    }
}

int image::width() const { return m_width; }
int image::height() const { return m_height; }

void image::clear() { std::fill(m_data.begin(), m_data.end(), std::vector<color>(m_width, color(0, 0, 0))); }

std::vector<int> &image::verticalPixels() { return m_verticalPixels; }
std::vector<int> &image::horizontalPixels() { return m_horizontalPixels; }

void image::writeColor(std::ostream &out, color pixelColor) const {
    auto r = pixelColor.x();
    auto g = pixelColor.y();
    auto b = pixelColor.z();

    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' ' << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

color &image::operator()(int x, int y) { return m_data[y][x]; }
color &image::at(int x, int y) { return m_data[y][x]; }

} // namespace shkyera
