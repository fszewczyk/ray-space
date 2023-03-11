#include "ui.hpp"
#include "core/ray.hpp"
#include "core/utils.hpp"
#include "core/vec3.hpp"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <cmath>
#include <glad/glad.h>
#include <imgui_internal.h>
#include <iostream>
#include <stdio.h>
#include <string>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

namespace shkyera {

ui::ui(std::shared_ptr<image> im, std::shared_ptr<renderer> renderer, std::shared_ptr<camera> cam)
    : m_renderWindow(im, cam), m_renderer(renderer), m_camera(cam), m_cameraSettingsWindow(cam),
      m_mouseSensitivity(0.025) {}

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
    m_window = glfwCreateWindow(1280, 720, "SHKYERA Engine", NULL, NULL);
    if (m_window == NULL)
        return;

    m_open = true;

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable vsync
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
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
}

void ui::run() {
    m_open = !glfwWindowShouldClose(m_window);
    // Main loop
    if (m_open) {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard
        // flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input
        // data to your main application, or clear/overwrite your copy of
        // the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard
        // input data to your main application, or clear/overwrite your copy
        // of the keyboard data. Generally you may always pass all inputs to
        // dear imgui, and hide them from your application based on those
        // two flags.

        // Start the Dear ImGui frame
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

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Load scene")) {
                }
                if (ImGui::MenuItem("Save scene")) {
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

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

                ImGuiID dock_id_left, dock_id_right, dock_id_top_right, dock_id_bottom_right;
                ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.70f, &dock_id_left, &dock_id_right);

                ImGui::DockBuilderDockWindow("Camera", dock_id_right);
                ImGui::DockBuilderDockWindow("Render", dock_id_left);
                ImGui::DockBuilderFinish(dockspace_id);
            }
        }

        ImGui::End();

        bool updatedSettings = false;
        point3 newCameraPosition = m_cameraSettingsWindow.render(updatedSettings);

        if (!updatedSettings)
            newCameraPosition = m_camera->getPosition();

        point3 cameraTranslation;
        std::pair<int, int> mouseMovement;

        if (m_renderer->renderedImage())
            cameraTranslation = m_renderWindow.render(true, updatedSettings, mouseMovement);
        else
            cameraTranslation = m_renderWindow.render(false, updatedSettings, mouseMovement);

        vec3 newCameraDirection = m_camera->getDirection();

        newCameraDirection.rotateAroundY(-m_mouseSensitivity * mouseMovement.first);
        newCameraDirection.rotateUpAndDown(m_mouseSensitivity * mouseMovement.second);
        newCameraDirection = unitVector(newCameraDirection);

        newCameraPosition += cameraTranslation;

        if (m_renderer->renderedImage() && updatedSettings) {
            m_renderer->stopRendering();
            m_renderer->renderingThread().join();

            m_camera->setPosition(newCameraPosition);
            m_camera->setDirection(newCameraDirection);

            m_renderer->startRendering();
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

bool ui::isOpen() const { return m_open; }

void ui::close() {
    m_renderer->stopRendering();
    m_renderer->renderingThread().join();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

} // namespace shkyera
