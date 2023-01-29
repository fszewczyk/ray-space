#include "ui.hpp"
#include "core/utils.hpp"

#include "imgui.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
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

namespace fg {

ui::ui(image &im) : m_image(im), m_loadedImage(false) {}

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
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
    // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#endif

    // Create window with graphics context
    m_window = glfwCreateWindow(1280, 720, "Freengine", NULL, NULL);
    if (m_window == NULL)
        return;

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable vsync
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = ImGui::GetIO();
    (void)m_io;

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
    // Main loop
    if (!glfwWindowShouldClose(m_window)) {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
        // tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data
        // to your main application, or clear/overwrite your copy of the mouse
        // data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
        // data to your main application, or clear/overwrite your copy of the
        // keyboard data. Generally you may always pass all inputs to dear
        // imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in
        // ImGui::ShowDemoWindow()! You can browse its code to learn more about
        // Dear ImGui!).

        // 2. Show a simple window that we create ourselves. We use a Begin/End
        // pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Camera Settings"); // Create a window called "Hello,
                                             // world!" and append into it.
            ImGui::End();
        }

        updateImageTexture();

        {
            ImGui::Begin("Render");

            if (m_loadedImage)
                ImGui::Image((void *)m_loadTexId,
                             ImVec2(m_loadWidth, m_loadHeight));

            ImGui::End();
        }

        // Rendering
        ImGui::Render();

        int display_w, display_h;

        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(m_clearColor.x * m_clearColor.w,
                     m_clearColor.y * m_clearColor.w,
                     m_clearColor.z * m_clearColor.w, m_clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
    } else {
        close();
    }
}

void ui::close() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void ui::updateImageTexture() {
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

} // namespace fg
