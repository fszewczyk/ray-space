#include "ui.hpp"
#include "core/image.hpp"
#include "core/ray.hpp"
#include "core/utils.hpp"
#include "core/vec3.hpp"

#include "implot.h"
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <cmath>
#include <glad/glad.h>
#include <imgui_internal.h>
#include <iostream>
#include <stdio.h>
#include <string>

namespace shkyera {

ui::ui(std::shared_ptr<image> im, std::shared_ptr<renderer> renderer, std::shared_ptr<visibleWorld> world,
       std::shared_ptr<camera> cam)
    : m_renderWindow(im, cam), m_renderer(renderer), m_camera(cam), m_world(world), m_cameraSettingsWindow(cam),
      m_worldSettingsWindow(world), m_exportSettingsWindow(cam), m_plotWindow(world, cam),
      m_mouseSensitivity(MOUSE_SENSITIVITY), m_renderMode(EDIT), m_exported(false) {}

void glfw_error_callback(int error, const char *description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void ui::init() { // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return;

        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE,
                   GLFW_OPENGL_CORE_PROFILE);            // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // // 3.2+ only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // // 3.0+ only
#endif

    // Create window with graphics context
    m_window = glfwCreateWindow(1100, 800, "Shkyera Engine", NULL, NULL);
    if (m_window == NULL)
        return;

    m_open = true;

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable vsync
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    m_clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    glfwInit();

    NORMAL_FONT = io.Fonts->AddFontFromFileTTF("resources/fonts/OpenSansRegular.ttf", 18);
    BOLD_FONT = io.Fonts->AddFontFromFileTTF("resources/fonts/OpenSansBold.ttf", 18);

    image::EARTH_DAY_TEXTURE->scaleImage(image::ICON_EARTH_DAY_TEXTURE, true);
    image::EARTH_NIGHT_TEXTURE->scaleImage(image::ICON_EARTH_NIGHT_TEXTURE, true);
    image::MARS_TEXTURE->scaleImage(image::ICON_MARS_TEXTURE, true);
    image::SUN_TEXTURE->scaleImage(image::ICON_SUN_TEXTURE, true);
    image::MOON_TEXTURE->scaleImage(image::ICON_MOON_TEXTURE, true);
    image::CERES_TEXTURE->scaleImage(image::ICON_CERES_TEXTURE, true);
    image::CLOUDY_VENUS_TEXTURE->scaleImage(image::ICON_CLOUDY_VENUS_TEXTURE, true);
    image::ERIS_TEXTURE->scaleImage(image::ICON_ERIS_TEXTURE, true);
    image::HAUMEA_TEXTURE->scaleImage(image::ICON_HAUMEA_TEXTURE, true);
    image::JUPITER_TEXTURE->scaleImage(image::ICON_JUPITER_TEXTURE, true);
    image::MAKE_TEXTURE->scaleImage(image::ICON_MAKE_TEXTURE, true);
    image::MERCURY_TEXTURE->scaleImage(image::ICON_MERCURY_TEXTURE, true);
    image::NEPTUNE_TEXTURE->scaleImage(image::ICON_NEPTUNE_TEXTURE, true);
    image::SATURN_TEXTURE->scaleImage(image::ICON_SATURN_TEXTURE, true);
    image::URANUS_TEXTURE->scaleImage(image::ICON_URANUS_TEXTURE, true);
    image::VENUS_TEXTURE->scaleImage(image::ICON_VENUS_TEXTURE, true);

    image::ICON_EARTH_DAY_TEXTURE->updateTextureId();
    image::ICON_EARTH_NIGHT_TEXTURE->updateTextureId();
    image::ICON_MARS_TEXTURE->updateTextureId();
    image::ICON_SUN_TEXTURE->updateTextureId();
    image::ICON_MOON_TEXTURE->updateTextureId();
    image::ICON_CERES_TEXTURE->updateTextureId();
    image::ICON_CLOUDY_VENUS_TEXTURE->updateTextureId();
    image::ICON_ERIS_TEXTURE->updateTextureId();
    image::ICON_HAUMEA_TEXTURE->updateTextureId();
    image::ICON_JUPITER_TEXTURE->updateTextureId();
    image::ICON_MAKE_TEXTURE->updateTextureId();
    image::ICON_MERCURY_TEXTURE->updateTextureId();
    image::ICON_NEPTUNE_TEXTURE->updateTextureId();
    image::ICON_SATURN_TEXTURE->updateTextureId();
    image::ICON_URANUS_TEXTURE->updateTextureId();
    image::ICON_VENUS_TEXTURE->updateTextureId();
}

void ui::style() {
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowMinSize = ImVec2(320, 320);
    style.FramePadding = ImVec2(4, 2);
    style.ItemSpacing = ImVec2(6, 2);
    style.ItemInnerSpacing = ImVec2(6, 4);
    style.Alpha = 0.95f;
    style.WindowRounding = 8.0f;
    style.FrameRounding = 8.0f;
    style.IndentSpacing = 6.0f;
    style.ItemInnerSpacing = ImVec2(2, 4);
    style.ColumnsMinSpacing = 50.0f;
    style.GrabMinSize = 14.0f;
    style.GrabRounding = 16.0f;
    style.ScrollbarSize = 12.0f;
    style.ScrollbarRounding = 16.0f;

    ImVec4 BACKGROUND_COLOR(0.08f, 0.1f, 0.12f, 1.0f);
    ImVec4 TEXT_COLOR(0.86f, 0.93f, 0.89f, 0.78f);
    ImVec4 DISABLED_TEXT_COLOR(0.86f, 0.93f, 0.89f, 0.28f);
    ImVec4 ACCENT_COLOR(0.2f, 0.6f, 0.9f, 0.6f);
    ImVec4 STRONG_ACCENT_COLOR(0.2f, 0.6f, 0.9f, 1.0f);

    ImVec4 GREY(0.7f, 0.7f, 0.7f, 1.0f);
    ImVec4 LIGHT_GREY(0.8f, 0.8f, 0.8f, 1.0f);
    ImVec4 BLACK(0.0f, 0.0f, 0.0f, 0.0f);

    style.Colors[ImGuiCol_Text] = TEXT_COLOR;
    style.Colors[ImGuiCol_TextDisabled] = DISABLED_TEXT_COLOR;
    style.Colors[ImGuiCol_WindowBg] = BACKGROUND_COLOR;
    style.Colors[ImGuiCol_BorderShadow] = BLACK;
    style.Colors[ImGuiCol_FrameBg] = ACCENT_COLOR;
    style.Colors[ImGuiCol_ScrollbarBg] = ACCENT_COLOR;
    style.Colors[ImGuiCol_FrameBgHovered] = ACCENT_COLOR;
    style.Colors[ImGuiCol_FrameBgActive] = STRONG_ACCENT_COLOR;
    style.Colors[ImGuiCol_TitleBgCollapsed] = BACKGROUND_COLOR;
    style.Colors[ImGuiCol_TitleBg] = ACCENT_COLOR;
    style.Colors[ImGuiCol_TitleBgActive] = STRONG_ACCENT_COLOR;
    style.Colors[ImGuiCol_SliderGrab] = ACCENT_COLOR;
    style.Colors[ImGuiCol_SliderGrabActive] = STRONG_ACCENT_COLOR;
    style.Colors[ImGuiCol_Separator] = GREY;
    style.Colors[ImGuiCol_SeparatorHovered] = LIGHT_GREY;
    style.Colors[ImGuiCol_SeparatorActive] = LIGHT_GREY;
    style.WindowMinSize = ImVec2(380, 200);

    ImPlotStyle &plotStyle = ImPlot::GetStyle();
    plotStyle.Colors[ImPlotCol_FrameBg] = BACKGROUND_COLOR;
    plotStyle.Colors[ImPlotCol_PlotBg] = BACKGROUND_COLOR;
    plotStyle.Colors[ImPlotCol_PlotBorder] = BACKGROUND_COLOR;
}

void ui::run() {
    m_open = !glfwWindowShouldClose(m_window);
    // Main loop
    if (m_open) {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        style();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                                        ImGuiWindowFlags_NoBackground;

        bool open = true;
        ImGui::Begin("Shkyera Engine", &open, window_flags);
        ImGui::PopStyleVar(3);

        ImGuiIO &io = ImGui::GetIO();

        if (io.ConfigFlags) {
            ImGuiID dockspace_id = ImGui::GetID("Shkyera Engine");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f),
                             ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton);

            static auto firstTime = true;
            if (firstTime) {
                firstTime = false;

                ImGui::DockBuilderRemoveNode(dockspace_id);
                ImGui::DockBuilderAddNode(dockspace_id,
                                          ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

                ImGuiID dock_id_left, dock_id_right, dock_id_top_right, dock_id_bottom_right, dock_id_top_left,
                    dock_id_bottom_left;
                ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.6f, &dock_id_left, &dock_id_right);
                ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Up, 0.4f, &dock_id_top_right,
                                            &dock_id_bottom_right);
                ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Up, 0.5f, &dock_id_top_left, &dock_id_bottom_left);

                ImGui::DockBuilderDockWindow("Render", dock_id_top_left);

                ImGui::DockBuilderDockWindow("Top view", dock_id_bottom_left);
                ImGui::DockBuilderDockWindow("Front view", dock_id_bottom_left);
                ImGui::DockBuilderDockWindow("Side view", dock_id_bottom_left);

                ImGui::DockBuilderDockWindow("Camera", dock_id_top_right);
                ImGui::DockBuilderDockWindow("Export", dock_id_top_right);
                ImGui::DockBuilderDockWindow("World", dock_id_bottom_right);

                ImGui::DockBuilderFinish(dockspace_id);
            }
        }

        ImGui::End();

        exportSettings settingsExport = m_exportSettingsWindow.render(m_renderMode);

        bool updatedSettings = false;
        systemSettings newSystemSettings = m_plotWindow.render(updatedSettings);

        cameraSettings newCameraSettings = newSystemSettings.cam;
        worldSettings newWorldSettings = newSystemSettings.world;

        if (updatedSettings) {
            m_cameraSettingsWindow.render(updatedSettings);
            m_worldSettingsWindow.render(updatedSettings);
        } else {
            newCameraSettings = m_cameraSettingsWindow.render(updatedSettings);
            newWorldSettings = m_worldSettingsWindow.render(updatedSettings);
        }

        point3 cameraTranslation;
        std::pair<int, int> mouseMovement;

        if (m_renderer->renderedImage())
            cameraTranslation = m_renderWindow.render(true, updatedSettings, mouseMovement);
        else
            cameraTranslation = m_renderWindow.render(false, updatedSettings, mouseMovement);

        newCameraSettings.direction.rotateAroundY(-m_mouseSensitivity * mouseMovement.first);
        newCameraSettings.direction.rotateUpAndDown(m_mouseSensitivity * mouseMovement.second);
        newCameraSettings.direction = unitVector(newCameraSettings.direction);

        newCameraSettings.origin += cameraTranslation;

        if (m_renderer->renderedImage() && updatedSettings) {
            m_renderer->stopRendering();
            m_renderer->renderingThread().join();

            m_camera->setSettings(newCameraSettings);
            m_world->setSettings(newWorldSettings);

            m_renderer->startRendering();
        }

        switch (m_renderMode) {
        case EXPORT:
            if (m_renderer->renderedImage() && !m_renderer->isExporting()) {
                m_renderer->stopRendering();
                m_renderer->renderingThread().join();

                m_camera->setAspectRatio(static_cast<float>(settingsExport.width) / settingsExport.height);

                auto previewImage = m_renderer->setupImageToExport(settingsExport);
                m_renderWindow.setImage(previewImage);

                m_renderer->startRendering();

                m_exported = false;
                m_startedExportTime = std::chrono::steady_clock::now();
                ImGui::OpenPopup("Exporting");
            }
            exportPopup(settingsExport);
            break;
        case PREVIEW:
            if (m_renderer->renderedImage() && !m_renderer->isExporting()) {
                m_renderer->stopRendering();
                m_renderer->renderingThread().join();

                m_camera->setAspectRatio(static_cast<float>(settingsExport.width) / settingsExport.height);

                auto previewImage = m_renderer->setupImageToExport(settingsExport);
                m_renderWindow.setImage(previewImage);

                m_renderer->startRendering();
            }
            break;
        case EDIT:
        default:
            if (m_renderer->renderedImage() && m_renderer->isExporting()) {
                m_renderer->stopRendering();
                m_renderer->renderingThread().join();

                auto editImage = m_renderer->stopExporting();
                m_renderWindow.setImage(editImage);

                m_renderer->startRendering();
            }
            break;
        }

        ImGui::Render();

        int display_w, display_h;

        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(m_clearColor.x * m_clearColor.w, m_clearColor.y * m_clearColor.w, m_clearColor.z * m_clearColor.w,
                     m_clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
    } else {
        close();
    }
}

void ui::exportPopup(exportSettings settings) {
    ImGui::SetNextWindowSize(ImVec2(300, 0));
    if (ImGui::BeginPopupModal("Exporting", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
        int elapsedNanoseconds =
            std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_startedExportTime).count();

        int nanosecondsPerPass = elapsedNanoseconds / (m_renderer->getTakenSamples() + 0.01f);
        int nanosecondsToFinish = nanosecondsPerPass * (settings.raysPerPixel - m_renderer->getTakenSamples());
        int secondsToFinish = nanosecondsToFinish / 1e3;

        ImGui::Dummy(ImVec2(150.0f, 5.0f));
        ImGui::Text("Progress: %.0f%%",
                    std::min(100.0f, 100.0f * m_renderer->getTakenSamples() / settings.raysPerPixel));
        ImGui::SameLine();
        ImGui::Text("\tTime left: %d seconds", std::max(0, secondsToFinish));

        if (m_exported) {
            ImGui::TextWrapped(("Succesfully saved the image to:\n" + settings.path).c_str());
            if (ImGui::Button("OK")) {
                m_renderMode = EDIT;
                m_exported = false;
                ImGui::CloseCurrentPopup();
            }
        } else {
            if (ImGui::Button("Cancel")) {
                m_renderMode = EDIT;
                ImGui::CloseCurrentPopup();
            }
        }

        if (!m_renderer->isDenoising() && !m_exported && m_renderer->getTakenSamples() >= 20) {
            auto noise = m_renderWindow.getImage()->getNoiseMap();
            auto coordsToDenoise = noise->getPixelsAbove(0.1);
            m_renderer->setDenoiseCoordinates(coordsToDenoise);
        }

        if (!m_exported && m_renderer->getTakenSamples() >= settings.raysPerPixel) {
            switch (settings.extension) {
            case PNG:
                m_renderWindow.getImage()->saveToPng(settings.path);
                break;
            case JPG:
            default:
                m_renderWindow.getImage()->saveToJpg(settings.path);
            }
            m_exported = true;
        }

        ImGui::EndPopup();
    }
}

bool ui::isOpen() const { return m_open; }

void ui::close() {
    m_renderer->stopRendering();
    m_renderer->renderingThread().join();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    ImPlot::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

} // namespace shkyera
