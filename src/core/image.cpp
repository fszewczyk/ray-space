#include "core/image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <glad/glad.h>

#include <cmath>
#include <numeric>

namespace shkyera {

image::image() : m_width(0), m_height(0), m_dominantColor(randomDouble(0, 1), randomDouble(0, 1), randomDouble(0, 1)) {}

image::image(int width, int height)
    : m_width(width), m_height(height), m_data(height, std::vector<color>(width)), m_verticalPixels(height),
      m_horizontalPixels(width), m_dominantColor(0, 0, 0) {
    std::iota(m_verticalPixels.begin(), m_verticalPixels.end(), 0);
    std::iota(m_horizontalPixels.begin(), m_horizontalPixels.end(), 0);
}

image::image(const char *filename) {
    int bytesPerPixel = 3;
    uint8_t *raw_data = stbi_load(filename, &m_width, &m_height, &bytesPerPixel, bytesPerPixel);

    if (!raw_data) {
        std::cerr << "\033[0;31mCould not load texture from file '" << filename << "'\033[0m" << std::endl;
#ifdef __APPLE__
        std::cerr << "\tMake sure you are running the executive from its directory." << std::endl;
#endif
        m_width = 0;
        m_height = 0;
        return;
    }

    if (bytesPerPixel != TEXTURE_BYTES_PER_PIXEL) {
        std::cerr << "\033[0;31mInvalid bit-depth of the texture '" << filename << "'\033[0m" << std::endl;
        m_width = 0;
        m_height = 0;

        delete raw_data;
        return;
    }

    m_verticalPixels.resize(height());
    m_horizontalPixels.resize(width());

    m_data.resize(height());

    color averageColor;
    double imageArea = height() * width();

    for (int y = 0; y < height(); ++y) {
        std::vector<color> row(width());

        for (int x = 0; x < width(); ++x) {
            color pixel;
            uint8_t *pixelOffset = raw_data + (x + y * width()) * bytesPerPixel;

            for (int channel = 0; channel < bytesPerPixel; ++channel) {
                pixel[channel] = static_cast<double>(pixelOffset[channel]);
            }

            row[x] = pixel;

            averageColor += pixel / imageArea;
        }

        m_data[y] = row;
    }

    m_dominantColor = averageColor / 255;

    delete raw_data;
}

void image::writeImage(std::ostream &out) const {
    out << "P3\n" << m_width << ' ' << m_height << "\n255\n";

    for (const auto &row : m_data) {
        for (const auto &pixel : row) {
            writeColor(out, pixel);
        }
    }
}

color image::getColor() const { return m_dominantColor; }

void image::scaleImage(std::shared_ptr<image> destinationImage, bool uniformScaling) {
    float scaleHorizontal = static_cast<float>(width()) / destinationImage->width();
    float scaleVertical = static_cast<float>(height()) / destinationImage->height();

    if (uniformScaling) {
        scaleVertical = std::min(scaleHorizontal, scaleVertical);
        scaleHorizontal = std::min(scaleHorizontal, scaleVertical);
    }

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

void image::updateTextureId() {
    std::vector<uint8_t> data(4 * width() * height());

    for (size_t y = 0; y < height(); ++y) {
        for (size_t x = 0; x < width(); ++x) {
            const color &c = at(x, y);
            data[(y * width() + x) * 4 + 0] = uint8_t(fabs(c[0]));
            data[(y * width() + x) * 4 + 1] = uint8_t(fabs(c[1]));
            data[(y * width() + x) * 4 + 2] = uint8_t(fabs(c[2]));
            data[(y * width() + x) * 4 + 3] = 255;
        }
    }

    unsigned textureId = m_textureId;

    if (m_assignedTextureId)
        glDeleteTextures(1, &textureId);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width(), height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    m_assignedTextureId = true;
    m_textureId = textureId;
}

uint64_t image::getTextureId() const { return m_textureId; }

int image::width() const { return m_width; }
int image::height() const { return m_height; }
bool image::isEmpty() const { return m_width == 0 || m_height == 0; }

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

std::shared_ptr<image> image::getBlurred(int size) const {
    if (size < 3)
        throw std::invalid_argument("Image blur size needs to be at least 3.");

    if (size % 2 == 0)
        throw std::invalid_argument("Image blur size needs to be odd.");

    auto blurred = std::make_shared<image>(m_width, m_height);
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            int lowerBoundY = std::max(0, y - size / 2);
            int lowerBoundX = std::max(0, x - size / 2);
            int upperBoundY = std::min(m_height, y + size / 2 + 1);
            int upperBoundX = std::min(m_width, x + size / 2 + 1);

            color average(0, 0, 0);
            int n = 0;

            for (int cy = lowerBoundY; cy < upperBoundY; ++cy) {
                for (int cx = lowerBoundX; cx < upperBoundX; ++cx) {
                    average += m_data[cy][cx];
                    n++;
                }
            }

            average /= n * 255;
            blurred->at(x, y) = average;
        }
    }

    return blurred;
}

std::shared_ptr<image> image::getNoiseMap() const {
    auto blurred = getBlurred(3);
    auto noise = std::make_shared<image>(m_width, m_height);

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            double diff = (blurred->at(x, y) - m_data[y][x]).length();
            noise->at(x, y) = color(diff, diff, diff);
        }
    }

    return noise;
}

std::vector<std::pair<size_t, size_t>> image::getPixelsAbove(double threshold) const {
    std::vector<std::pair<size_t, size_t>> coords;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            if (m_data[y][x].length() > threshold)
                coords.push_back({x, y});
        }
    }

    return coords;
}

void image::saveToPng(std::string path) {
    char *data = new char[3 * width() * height()];

    for (int y = 0; y < height(); ++y) {
        for (int x = 0; x < width(); ++x) {
            color c = at(x, y);
            data[(y * width() + x) * 3 + 0] = static_cast<int>(c[0] * 255);
            data[(y * width() + x) * 3 + 1] = static_cast<int>(c[1] * 255);
            data[(y * width() + x) * 3 + 2] = static_cast<int>(c[2] * 255);
        }
    }

    stbi_write_png(path.c_str(), width(), height(), 3, data, 3 * width());
}

void image::saveToJpg(std::string path) {
    char *data = new char[3 * width() * height()];

    for (int y = 0; y < height(); ++y) {
        for (int x = 0; x < width(); ++x) {
            color c = at(x, y);
            data[(y * width() + x) * 3 + 0] = static_cast<int>(c[0] * 255);
            data[(y * width() + x) * 3 + 1] = static_cast<int>(c[1] * 255);
            data[(y * width() + x) * 3 + 2] = static_cast<int>(c[2] * 255);
        }
    }

    stbi_write_jpg(path.c_str(), width(), height(), 3, data, 100);
}

std::shared_ptr<image> image::EARTH_DAY_TEXTURE = std::make_shared<image>("resources/textures/earthday.jpg");
std::shared_ptr<image> image::EARTH_NIGHT_TEXTURE = std::make_shared<image>("resources/textures/earthnight.jpg");
std::shared_ptr<image> image::MARS_TEXTURE = std::make_shared<image>("resources/textures/mars.jpg");
std::shared_ptr<image> image::SUN_TEXTURE = std::make_shared<image>("resources/textures/sun.jpg");
std::shared_ptr<image> image::MOON_TEXTURE = std::make_shared<image>("resources/textures/moon.jpg");
std::shared_ptr<image> image::STARS_TEXTURE = std::make_shared<image>("resources/textures/stars.jpg");
std::shared_ptr<image> image::CERES_TEXTURE = std::make_shared<image>("resources/textures/ceres.jpg");
std::shared_ptr<image> image::CLOUDY_VENUS_TEXTURE = std::make_shared<image>("resources/textures/cloudyvenus.jpg");
std::shared_ptr<image> image::ERIS_TEXTURE = std::make_shared<image>("resources/textures/eris.jpg");
std::shared_ptr<image> image::HAUMEA_TEXTURE = std::make_shared<image>("resources/textures/haumea.jpg");
std::shared_ptr<image> image::JUPITER_TEXTURE = std::make_shared<image>("resources/textures/jupiter.jpg");
std::shared_ptr<image> image::MAKE_TEXTURE = std::make_shared<image>("resources/textures/makemake.jpg");
std::shared_ptr<image> image::MERCURY_TEXTURE = std::make_shared<image>("resources/textures/mercury.jpg");
std::shared_ptr<image> image::NEPTUNE_TEXTURE = std::make_shared<image>("resources/textures/neptune.jpg");
std::shared_ptr<image> image::SATURN_TEXTURE = std::make_shared<image>("resources/textures/saturn.jpg");
std::shared_ptr<image> image::URANUS_TEXTURE = std::make_shared<image>("resources/textures/uranus.jpg");
std::shared_ptr<image> image::VENUS_TEXTURE = std::make_shared<image>("resources/textures/venus.jpg");

} // namespace shkyera
