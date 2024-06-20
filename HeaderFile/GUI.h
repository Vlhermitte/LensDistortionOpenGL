//
// Created by Valentin Lhermitte on 20/06/2024.
//

#ifndef LENSDISTORTIONOPENGL_GUI_H
#define LENSDISTORTIONOPENGL_GUI_H

// Header for ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Camera.h"

class GUI {
private:
    static ImGuiIO* io;
public:
    explicit GUI(GLFWwindow* window);
    void NewFrame();
    void Render();
    void DisableMouse(Camera& camera);
    void Shutdown();

    glm::vec3 DistortionSlider();
};

#endif //LENSDISTORTIONOPENGL_GUI_H
