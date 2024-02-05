cmake_minimum_required(VERSION 3.27)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

link_directories(${CMAKE_SOURCE_DIR}/lib)

file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/HeaderFile/*.h
        ${CMAKE_SOURCE_DIR}/HeaderFile/*.hpp)

# Find DLLs
file(GLOB DLLS "${CMAKE_SOURCE_DIR}/dlls/*.dll")
file(COPY ${DLLS} DESTINATION ${CMAKE_BINARY_DIR})

# Include directories
include_directories(
        ${CMAKE_SOURCE_DIR}/HeaderFile
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/Shaders
        ${CMAKE_SOURCE_DIR}/include/GLFW
        ${CMAKE_SOURCE_DIR}/include/GLEW
        ${CMAKE_SOURCE_DIR}/include/GLM
        ${CMAKE_SOURCE_DIR}/include/KHR
        ${CMAKE_SOURCE_DIR}/include
)


# Add executable
add_executable(${PROJECT_NAME} src/main.cpp
        src/Shader.cpp
        HeaderFile/Shader.h
        src/VBO.cpp
        HeaderFile/VBO.h
        src/EBO.cpp
        HeaderFile/EBO.h
        src/VAO.cpp
        HeaderFile/VAO.h
        HeaderFile/data.h
        HeaderFile/Camera.h
        src/Camera.cpp
        src/stb.cpp
        src/Texture.cpp
        HeaderFile/Texture.h)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Find packages
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

# Find GLFW
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

# Find GLEW
find_package(GLEW REQUIRED)
message(STATUS "Found GLEW in ${GLEW_INCLUDE_DIR}")

# Find GLM
find_package(GLM REQUIRED)
message(STATUS "Found GLM in ${GLM_INCLUDE_DIR}")


# Link libraries
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLFW3_LIBRARY} ${GLEW_LIBRARY})

# Define GL_SILENCE_DEPRECATION to silence deprecation warnings
target_compile_definitions(${PROJECT_NAME} PRIVATE GL_SILENCE_DEPRECATION)



