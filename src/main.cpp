#include "main.h"
#include "GameState.h"

std::vector<Model> initModels() {
    std::vector<Model> models;
    // Floor
    Model floor("../Resources/Models/Floor/Floor.obj");
    floor.setPosition(glm::vec3(0.0f, -0.25f, 0.0f));
    floor.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
    models.emplace_back(floor);

    Model Cottage("../Resources/Models/Cottage/Cottage_FREE.obj");
    Cottage.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    Cottage.setScale(glm::vec3(0.3f, 0.3f, 0.3f));
    models.emplace_back(Cottage);

    // RaceCar
    Model raceCar("../Resources/Models/RaceCar/RaceCar.obj");
    raceCar.setPosition(glm::vec3(1.5f, 0.0f, 3.2f));
    raceCar.setScale(glm::vec3(0.3f, 0.3f, 0.3f));
    raceCar.setRotation(glm::vec3(0.0f, 40.0f, 0.0f));
    models.emplace_back(raceCar);

    // Cadillac
    Model cadillac("../Resources/Models/Cadillac/Cadillac_CT4_V_2022.obj");
    cadillac.setPosition(glm::vec3(-0.8f, 0.0f, 4.0f));
    cadillac.setScale(glm::vec3(0.3f, 0.3f, 0.3f));
    models.emplace_back(cadillac);

    // Sun Model
    Model sun("../Resources/Models/Sun/Sun.obj");
    sun.setPosition(glm::vec3(8.0f, 8.0f, 8.0f));
    sun.setScale(glm::vec3(0.5f, 0.5f, 0.5f));
    models.emplace_back(sun);

    return models;
}

Skybox initSkybox() {
    std::vector<std::string> faces {
            "../Resources/Textures/Skybox/sh_posx.jpg",
            "../Resources/Textures/Skybox/sh_negx.jpg",
            "../Resources/Textures/Skybox/sh_posy.jpg",
            "../Resources/Textures/Skybox/sh_negy.jpg",
            "../Resources/Textures/Skybox/sh_posz.jpg",
            "../Resources/Textures/Skybox/sh_negz.jpg"
    };

    std::vector<Vertex> skyboxVerts(skyboxVertices, skyboxVertices + sizeof(skyboxVertices) / sizeof(Vertex));
    std::vector<GLuint> skyboxInds(skyboxIndices, skyboxIndices + sizeof(skyboxIndices) / sizeof(GLuint));
    return Skybox(faces, skyboxVerts, skyboxInds);
}

int main(int argc, char** argv) {

    if (!glfwInit()) {
        std::cout << "Error initializing glfw" << std::endl;
        return -1;
    }

    // set up glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    // create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", NULL, NULL);
    if (!window) {
        std::cout << "Error creating window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // set up glew
    if (glewInit() != GLEW_OK) {
        std::cout << "Error initializing glew" << std::endl;
        return -1;
    }

    Shader defaultShader("../Shaders/default.vert", "../Shaders/default.frag");
    Shader lightShader("../Shaders/light.vert", "../Shaders/light.frag");
    Shader skyboxShader("../Shaders/skybox.vert", "../Shaders/skybox.frag");
    Shader shadowMapShader("../Shaders/shadowMap.vert", "../Shaders/shadowMap.frag");
    Shader postProcessShader("../Shaders/postProcessShader.vert", "../Shaders/postProcessShader.frag");

    // Skybox
    Skybox skybox = initSkybox();
    // Models
    std::vector<Model> models = initModels();

    glm::vec4 sunColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // Sun Position is the last model in the models vector
    glm::vec3 sunPos = models.back().getPosition();

    defaultShader.Activate();
    glUniform3fv(glGetUniformLocation(defaultShader.ID, "lightPos"), 1, glm::value_ptr(sunPos));
    glUniform4fv(glGetUniformLocation(defaultShader.ID, "lightColor"), 1, glm::value_ptr(sunColor));
    glUniform1i(glGetUniformLocation(defaultShader.ID, "usePointLight"), true);
    glUniform1i(glGetUniformLocation(defaultShader.ID, "useDirectionalLight"), true);
    glUniform1i(glGetUniformLocation(defaultShader.ID, "useSpotLight"), false);
    glUniform1i(glGetUniformLocation(defaultShader.ID, "usePreProcessDistortion"), gameState.preProcessingDistortion);
    defaultShader.Deactivate();
    CHECK_GL_ERROR();

    lightShader.Activate();
    glUniform4fv(glGetUniformLocation(lightShader.ID, "lightColor"), 1, glm::value_ptr(sunColor));
    lightShader.Deactivate();
    CHECK_GL_ERROR();

    // Camera
    Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 1.0f, 5.0f)); // Positive Z result in a backward movement because the camera is looking at the negative Z axis

    // Time
    float previousTime = 0.0f;
    float currentTime = 0.0f;
    float deltaTime = 0.0f;
    unsigned int framesCounter = 0;
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight); // We need to get the framebuffer size in case of retina displays

    // Shadow Map
    ShadowMap shadowMap;

    // Custom Framebuffer, we will use it to render the scene to a texture and then use this texture to render a quad on the screen
    Framebuffer framebuffer(windowWidth, windowHeight);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // GUI
    GUI gui(window);

    while (!glfwWindowShouldClose(window)) {
        currentTime = glfwGetTime();
        deltaTime = currentTime - previousTime;
        framesCounter++;
        if (deltaTime >= 1.0f) {
            std::string fps = std::to_string((1.0 / deltaTime) * framesCounter);
            std::string ms = std::to_string(deltaTime * 1000.0 / framesCounter);
            std::string title = "OpenGL Project | FPS: " + fps + " | " + ms + " (ms)";
            glfwSetWindowTitle(window, title.c_str());
            previousTime = currentTime;
            framesCounter = 0;
        }
        if (deltaTime >= 1.0f / 30.0f) {
            // Putting the Inputs handling here, to avoid inputs depending on the frame rate
            camera.Inputs(window);
        }

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Sending the camera distortion parameters to the shader
        if (gameState.preProcessingDistortion) {
            camera.RenderRadialDistortion(defaultShader);
            camera.RenderTangentialDistortion(defaultShader);
        } else {
            camera.RenderRadialDistortion(postProcessShader);
            camera.RenderTangentialDistortion(postProcessShader);
        }

        camera.updateMatrix(60.0f, 0.1f, 100.0f);

        // Draw the models to the shadow map
        shadowMap.ActivateShadows(shadowMapShader, sunPos);
        for (auto & model : models) {
            model.Draw(shadowMapShader, camera);
        }
        shadowMap.Unbind();

        // Set custom framebuffer
        framebuffer.Bind();
        glViewport(0, 0, windowWidth, windowHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        defaultShader.Activate();
        glUniformMatrix4fv(glGetUniformLocation(defaultShader.ID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(shadowMap.GetLightSpaceMatrix()));
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadowMap.GetDepthMap());
        glUniform1i(glGetUniformLocation(defaultShader.ID, "shadowMap"), 1);
        CHECK_GL_ERROR();
        defaultShader.Deactivate();

        // Draw Skybox
        skybox.Draw(skyboxShader, camera);

        // Draw models
        for (auto & model : models) {
            model.Draw(defaultShader, camera);
        }

        // Unbind custom framebuffer
        framebuffer.Unbind();

        // Draw framebuffer texture to a quad
        if (camera.IsWireframeMode()) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // Draw the quad without wireframe mode
        }

        framebuffer.Draw(postProcessShader);

        // GUI Rendering (We have to render it after the framebuffer has been drawn, otherwise the GUI will not be visible)
        gui.NewFrame();

        // Camera Distortion Parameters retrieval from GUI
        gameState.preProcessingDistortion = gui.DistortionModeSwitch();
        auto distortionParams = gui.DistortionSlider();
        camera.setRadialDistortionParams(distortionParams.first);
        camera.setTangentialDistortionParams(distortionParams.second);

        gui.Render();
        gui.DisableMouse(camera);

        if (camera.IsWireframeMode()) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Go back to camera wireframe mode
        }

        // Swap buffers and GLFW poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    gui.Shutdown();
    defaultShader.Delete();
    lightShader.Delete();
    skyboxShader.Delete();
    shadowMapShader.Delete();
    postProcessShader.Delete();

    for (auto & model : models) {
        model.Delete();
    }

    shadowMap.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
