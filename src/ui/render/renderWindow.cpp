#include "ui/render/renderWindow.hpp"

#include "imgui.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <glad/glad.h>

namespace fg {

renderWindow::renderWindow(image &im) : m_image(im) { m_loadedImage = false; }

void renderWindow::render() {
    updateImageTexture();
    {
        ImGui::Begin("Render");

        if (m_loadedImage)
            ImGui::Image((void *)m_loadTexId,
                         ImVec2(m_loadWidth, m_loadHeight));

        ImGui::End();
    }
}

void renderWindow::updateImageTexture() {
    m_loadWidth = m_image.width();
    m_loadHeight = m_image.height();

    if (m_renderTexture.size() == 0)
        m_renderTexture.resize(4 * m_loadWidth * m_loadHeight);

    for (size_t y = 0; y < m_loadHeight; ++y) {
        for (size_t x = 0; x < m_loadWidth; ++x) {
            const color &c = m_image(x, y);
            m_renderTexture[(y * m_loadWidth + x) * 4 + 0] =
                uint8_t(fabs(c[0] * 255));
            m_renderTexture[(y * m_loadWidth + x) * 4 + 1] =
                uint8_t(fabs(c[1] * 255));
            m_renderTexture[(y * m_loadWidth + x) * 4 + 2] =
                uint8_t(fabs(c[2] * 255));
            m_renderTexture[(y * m_loadWidth + x) * 4 + 3] = 255;
        }
    }
    if (m_loadedImage)
        glDeleteTextures(1, &m_loadTexId);

    glGenTextures(1, &m_loadTexId);
    glBindTexture(GL_TEXTURE_2D, m_loadTexId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_loadWidth, m_loadHeight, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, m_renderTexture.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    m_loadedImage = true;
}

image &renderWindow::getImage() const { return m_image; }

} // namespace fg
