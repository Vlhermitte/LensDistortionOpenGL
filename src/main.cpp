#include "main.h"

std::vector<Model> initModels() {
    std::vector<Model> models;
    // Floor
    Model floor("../Resources/Models/Floor/Floor.obj");
    floor.setPosition(glm::vec3(0.0f, -0.25f, 0.0f));
    floor.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
    models.emplace_back(floor);

    // RaceCar
    Model raceCar("../Resources/Models/RaceCar/RaceCar.obj");
    raceCar.setPosition(glm::vec3(0.5f, 0.0f, 0.5f));
    raceCar.setScale(glm::vec3(0.1f, 0.1f, 0.1f));
    models.emplace_back(raceCar);

    // Cadillac
    Model cadillac("../Resources/Models/Cadillac/Cadillac_CT4_V_2022.obj");
    cadillac.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    cadillac.setScale(glm::vec3(0.1f, 0.1f, 0.1f));
    models.emplace_back(cadillac);

    // Sun
    Model sun("../Resources/Models/Sun/Sun.obj");
    sun.setPosition(glm::vec3(0.5f, 1.0f, 0.5f));
    sun.setScale(glm::vec3(0.2f, 0.2f, 0.2f));
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
    Shader shadowMapShader("../Shaders/shadowMap.vert", "../Shaders/shadowMap.frag"); // Not used yet

    glm::vec4 sunColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 sunPos = glm::vec3(0.5f, 0.5f, 0.5f);

    defaultShader.Activate();
    glUniform3fv(glGetUniformLocation(defaultShader.ID, "lightPos"), 1, glm::value_ptr(sunPos));
    glUniform4fv(glGetUniformLocation(defaultShader.ID, "lightColor"), 1, glm::value_ptr(sunColor));
    glUniform1i(glGetUniformLocation(defaultShader.ID, "usePointLight"), true);
    glUniform1i(glGetUniformLocation(defaultShader.ID, "useDirectionalLight"), false);
    glUniform1i(glGetUniformLocation(defaultShader.ID, "useSpotLight"), true);
    defaultShader.Deactivate();

    lightShader.Activate();
    glUniform4fv(glGetUniformLocation(lightShader.ID, "lightColor"), 1, glm::value_ptr(sunColor));
    lightShader.Deactivate();

    // Skybox
    Skybox skybox = initSkybox();
    // Models
    std::vector<Model> models = initModels();

    // Camera
    Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.5f, 2.0f)); // Positive Z result in a backward movement because the camera is looking at the negative Z axis
    camera.AddRadialDistortion(defaultShader, glm::vec3(0.0f, 0.0f, 0.0f));
    camera.AddTangentialDistortion(defaultShader, glm::vec2(0.0f, 0.0f));

    // Time
    float previousTime = 0.0f;
    float currentTime = 0.0f;
    float deltaTime = 0.0f;
    unsigned int framesCounter = 0;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

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

        camera.updateMatrix(60.0f, 0.1f, 100.0f);
        camera.Matrix(defaultShader, "camMatrix");

        // Draw Skybox
        skybox.Draw(skyboxShader, camera);

        // Draw models
        for (auto & model : models) {
            model.Draw(defaultShader, camera);
        }

        // Swap buffers and GLFW poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    defaultShader.Delete();
    lightShader.Delete();
    skyboxShader.Delete();
    shadowMapShader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
