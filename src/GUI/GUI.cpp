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

glm::vec3 GUI::DistortionSlider() {
    // Set the next window size
    ImGui::Begin("Radial Distortion Parameters");
    ImVec2 windowSize = ImVec2(100, 300);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);

    static glm::vec3 radialDistortionParam = glm::vec3(0.0f, 0.0f, 0.0f);
    ImGui::SliderFloat("k1", &radialDistortionParam.x, 0.0f, 1.0f);
    ImGui::SliderFloat("k2", &radialDistortionParam.y, 0.0f, 1.0f);
    ImGui::SliderFloat("k3", &radialDistortionParam.z, 0.0f, 1.0f);

    ImGui::End();

    return radialDistortionParam;
}
