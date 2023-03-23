#include <filesystem>
#include <stdio.h>
#include <unistd.h>

#include "ui/settings/exportSettingsWindow.hpp"
#include "ui/ui.hpp"

#include "imgui.h"
#include "imgui_internal.h"

namespace shkyera {

exportSettingsWindow::exportSettingsWindow(std::shared_ptr<camera> cam) : m_camera(cam) {}

exportSettings exportSettingsWindow::render(RENDER_MODE &mode) {
    ImGui::Begin("Export", nullptr, ImGuiWindowFlags_None);

    static exportSettings settings = getDefaultExportSettings();
    int setWidth = settings.width;
    int setHeight = settings.height;

    float aspectRatio = static_cast<float>(settings.width) / settings.height;

    if (ImGui::BeginCombo("Format", (" " + FILE_EXTENSIONS[settings.extension]).c_str())) {
        if (ImGui::Selectable("PNG", settings.extension == PNG))
            settings.extension = PNG;
        if (ImGui::Selectable("JPG", settings.extension == JPG))
            settings.extension = JPG;
        ImGui::EndCombo();
    }

    static char fileName[128] = "shkyera_render";
    ImGui::InputText("File Name", fileName, IM_ARRAYSIZE(fileName));

    static const char *localDirectory = settings.path.c_str();
    static char directory[1024];
    static bool loadedLocalDirectory = false;
    if (!loadedLocalDirectory) {
        strcpy(directory, localDirectory);
        loadedLocalDirectory = true;
    }

    ImGui::TextWrapped(directory);

#ifndef __APPLE__
    if (ImGui::Button("Choose Destination")) {
        FILE *f = popen("zenity  --file-selection --directory --title=\"Choose a directory\"", "r");
        fgets(directory, 1024, f);
        std::string stringifiedDirectory(directory);
        stringifiedDirectory.pop_back();
        strcpy(directory, stringifiedDirectory.c_str());
    }
#endif

    settings.path = directory;
    settings.path += "/";
    settings.path += fileName;

    switch (settings.extension) {
    case PNG:
        settings.path += ".png";
        break;
    case JPG:
    default:
        settings.path += ".jpg";
        break;
    }

    ImGui::Dummy(ImVec2(0.0f, 5.0f));

    ImGui::Checkbox("Lock Aspect Ratio", &settings.lockAspectRatio);

    ImGui::SliderInt("Width", &setWidth, 128, 4096);
    ImGui::SliderInt("Height", &setHeight, 128, 4096);

    if (settings.lockAspectRatio) {
        if (setWidth != settings.width)
            setHeight = setWidth / aspectRatio;
        else if (setHeight != settings.height)
            setWidth = setHeight * aspectRatio;
    }

    settings.width = std::max(128, setWidth);
    settings.height = std::max(128, setHeight);

    ImGui::SliderInt("Rays Per Pixel", &settings.raysPerPixel, 10, 200);
    if (ImGui::IsItemHovered() && !ImGui::IsItemActive()) {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted("Rays shot out for every pixel.\nThe higher, the "
                               "less noisy the image.\nUse high values "
                               "if you have relatively small light sources.");
        ImGui::EndTooltip();
    }

    ImGui::Dummy(ImVec2(0, 5.0f));

    if (mode == EDIT) {
        if (ImGui::Button("Start Preview", ImVec2(130, 0))) {
            mode = PREVIEW;
        }
        if (ImGui::IsItemHovered() && !ImGui::IsItemActive()) {
            ImGui::BeginTooltip();
            ImGui::TextUnformatted("The program might run slower after running preview.");
            ImGui::EndTooltip();
        }
    } else {
        if (ImGui::Button("Stop Preview", ImVec2(130, 0))) {
            mode = EDIT;
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Export", ImVec2(130, 0))) {
        mode = EXPORT;
    }
    if (ImGui::IsItemHovered() && !ImGui::IsItemActive()) {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted("The program might run slower after starting the export.");
        ImGui::EndTooltip();
    }

    ImGui::End();

    return settings;
} // namespace shkyera

exportSettings exportSettingsWindow::getDefaultExportSettings() {
    exportSettings settings;
    settings.width = 1920;
    settings.height = 1080;
    settings.raysPerPixel = 40;
    settings.extension = PNG;
    settings.lockAspectRatio = true;

#ifdef __APPLE__
    settings.path = std::filesystem::current_path();
#else
    settings.path = get_current_dir_name();
#endif

    return settings;
}

} // namespace shkyera
