// Class definitions for GUI

#include "GUI.h"


// Define the static variable
ImGuiIO* GUI::io = nullptr;

GUI::GUI(GLFWwindow* window) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // Setup Platform/Renderer bindings
    io = &ImGui::GetIO();  // Initialize the member variable
    (void)io;
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
}

void GUI::NewFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}


void GUI::Render() {
    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::DisableMouse(Camera& camera) {
    // Disable mouse when hovering over the GUI
    if (io->WantCaptureMouse) {
        camera.DisableMouse();
    } else {
        camera.EnableMouse();
    }
}

void GUI::Shutdown() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

bool GUI::DistortionModeSwitch() {
    // Set the next window size
    ImGui::Begin("Lens Distortion Parameters");
    ImVec2 windowSize = ImVec2(150, 300);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);

    static bool distortionMode = true;
    // Checked by default
    ImGui::Checkbox("Pre Processing Mode", &distortionMode);

    ImGui::End();

    return distortionMode;
}

std::pair<glm::vec3, glm::vec2> GUI::DistortionSlider() {
    // Set the next window size
    ImGui::Begin("Lens Distortion Parameters");

    static glm::vec3 radialDistortionParams = glm::vec3(0.0f, 0.0f, 0.0f);
    ImGui::SliderFloat("k1", &radialDistortionParams.x, -0.4f, 0.4f);
    ImGui::SliderFloat("k2", &radialDistortionParams.y, -0.4f, 0.4f);
    ImGui::SliderFloat("k3", &radialDistortionParams.z, -0.4f, 0.4f);

    static glm::vec2 tangentialDistortionParams = glm::vec2(0.0f, 0.0f);
    ImGui::SliderFloat("p1", &tangentialDistortionParams.x, -0.4f, 0.4f);
    ImGui::SliderFloat("p2", &tangentialDistortionParams.y, -0.4f, 0.4f);

    ImGui::End();

    return std::make_pair(radialDistortionParams, tangentialDistortionParams);
}


