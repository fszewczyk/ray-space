#include "ui/render/renderWindow.hpp"

#include "imgui.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <glad/glad.h>

namespace shkyera {

renderWindow::renderWindow(std::shared_ptr<image> im, std::shared_ptr<camera> cam)
    : m_image(im), m_camera(cam), m_leftMouseHold(false) {
    m_loadedImage = false;
}

point3 renderWindow::render(bool sampleTexture, bool &updated, std::pair<int, int> &mouseMovement) {
    if (sampleTexture)
        updateImageTexture();
    {
        ImGui::Begin("Render");

        point3 cameraTranslation;

        if (ImGui::IsWindowFocused()) {
            if (ImGui::IsKeyPressed('W'))
                cameraTranslation += -0.4 * m_camera->getDirection();
            if (ImGui::IsKeyPressed('S'))
                cameraTranslation += 0.4 * m_camera->getDirection();
            if (ImGui::IsKeyPressed('A'))
                cameraTranslation += -0.4 * m_camera->getSidewaysDirection();
            if (ImGui::IsKeyPressed('D'))
                cameraTranslation += 0.4 * m_camera->getSidewaysDirection();
            if (ImGui::IsKeyPressed('Q'))
                cameraTranslation[1] = -0.4;
            if (ImGui::IsKeyPressed('E'))
                cameraTranslation[1] = 0.4;

            if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                ImVec2 mousePositionAbsolute = ImGui::GetMousePos();
                ImVec2 screenPositionAbsolute = ImGui::GetItemRectMin();
                ImVec2 mousePositionRelative = ImVec2(mousePositionAbsolute.x - screenPositionAbsolute.x,
                                                      mousePositionAbsolute.y - screenPositionAbsolute.y);

                if (m_leftMouseHold) {
                    mouseMovement.first = mousePositionRelative.x - m_lastMousePosition.first;
                    mouseMovement.second = mousePositionRelative.y - m_lastMousePosition.second;
                    updated = true;
                }

                m_lastMousePosition.first = mousePositionRelative.x;
                m_lastMousePosition.second = mousePositionRelative.y;

                m_leftMouseHold = true;
            } else {
                m_leftMouseHold = false;
            }
        }

        float renderAspectRatio = static_cast<float>(m_loadWidth) / m_loadHeight;

        float windowWidth = ImGui::GetWindowWidth() - 30;
        float windowHeight = ImGui::GetWindowHeight() - 40;
        float windowAspectRatio = windowWidth / windowHeight;

        float displayWidth = windowWidth;
        float displayHeight = windowHeight;

        if (renderAspectRatio > windowAspectRatio)
            displayHeight = displayWidth / renderAspectRatio;

        displayWidth = displayHeight * renderAspectRatio;

        float offsetWidth = (windowWidth - displayWidth) / 2;
        float offsetHeight = (windowHeight - displayHeight) / 2;

        ImGui::Dummy(ImVec2(0, offsetHeight));
        ImGui::Dummy(ImVec2(offsetWidth, 0));
        ImGui::SameLine();
        ImGui::Image((ImTextureID)m_loadTexId, ImVec2(displayWidth, displayHeight));

        ImGui::End();

        updated |= cameraTranslation.lengthSquared() != 0;

        return cameraTranslation;
    }
}

void renderWindow::updateImageTexture() {
    m_loadWidth = m_image->width();
    m_loadHeight = m_image->height();

    if (m_renderTexture.size() == 0)
        m_renderTexture.resize(4 * m_loadWidth * m_loadHeight);

    for (size_t y = 0; y < m_loadHeight; ++y) {
        for (size_t x = 0; x < m_loadWidth; ++x) {
            const color &c = m_image->at(x, y);
            m_renderTexture[(y * m_loadWidth + x) * 4 + 0] = uint8_t(fabs(c[0] * 255));
            m_renderTexture[(y * m_loadWidth + x) * 4 + 1] = uint8_t(fabs(c[1] * 255));
            m_renderTexture[(y * m_loadWidth + x) * 4 + 2] = uint8_t(fabs(c[2] * 255));
            m_renderTexture[(y * m_loadWidth + x) * 4 + 3] = 255;
        }
    }
    unsigned textureId = m_loadTexId;

    if (m_loadedImage)
        glDeleteTextures(1, &textureId);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_loadWidth, m_loadHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 m_renderTexture.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    m_loadedImage = true;
    m_loadTexId = textureId;
}

std::shared_ptr<image> renderWindow::getImage() const { return m_image; }

} // namespace shkyera
