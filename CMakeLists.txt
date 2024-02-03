cmake_minimum_required(VERSION 3.27)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

# Find packages
find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

# Include directories
include_directories(dependencies)
include_directories(HeaderFile)
include_directories(Shaders)

# Add subdirectories
add_subdirectory(dependencies/glad)

# Add executable
add_executable(${PROJECT_NAME} main.cpp
        dependencies/glad/src/glad.c
        Shader.cpp
        HeaderFile/Shader.h
        VBO.cpp
        HeaderFile/VBO.h
        EBO.cpp
        HeaderFile/EBO.h
        VAO.cpp
        HeaderFile/VAO.h)

# Include directories
include_directories(
        ${OPENGL_INCLUDE_DIR}
        ${GLUT_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIR}
        ${GLM_INCLUDE_DIR}
        ${GLFW_INCLUDE_DIR}
        dependencies/glad/include
)

# Target link libraries
target_link_libraries(
        ${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARY}
        ${GLM_LIBRARY}
        glfw
        glad
)

# Define GL_SILENCE_DEPRECATION to silence deprecation warnings
target_compile_definitions(${PROJECT_NAME} PRIVATE GL_SILENCE_DEPRECATION)



