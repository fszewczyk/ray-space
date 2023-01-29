#include "core/image.hpp"

#include <numeric>

namespace fg {

image::image(int width, int height)
    : m_width(width), m_height(height),
      m_data(height, std::vector<color>(width)), m_verticalPixels(height),
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

int image::width() const { return m_width; }
int image::height() const { return m_width; }

std::vector<int> &image::verticalPixels() { return m_verticalPixels; }
std::vector<int> &image::horizontalPixels() { return m_horizontalPixels; }

void image::writeColor(std::ostream &out, color pixelColor) const {
    auto r = pixelColor.x();
    auto g = pixelColor.y();
    auto b = pixelColor.z();

    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

color &image::operator()(int x, int y) { return m_data[y][x]; }

} // namespace fg
