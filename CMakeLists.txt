cmake_minimum_required(VERSION 3.27)
project(LensDistortionOpenGL)

set(CMAKE_CXX_STANDARD 17)

link_directories(${CMAKE_SOURCE_DIR}/lib)

# Find DLLs
if (WIN32)
    file(GLOB DLLS "${CMAKE_SOURCE_DIR}/dlls/*.dll")
    file(COPY ${DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif ()
# Include directories
include_directories(
        ${CMAKE_SOURCE_DIR}/HeaderFile
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/Engine
        ${CMAKE_SOURCE_DIR}/Shaders
        ${CMAKE_SOURCE_DIR}/include/GLFW
        ${CMAKE_SOURCE_DIR}/include/GLEW
        ${CMAKE_SOURCE_DIR}/include/GLM
        ${CMAKE_SOURCE_DIR}/include
)

file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/HeaderFile/*.h
        ${CMAKE_SOURCE_DIR}/HeaderFile/*.hpp)

add_library(engine
        src/Engine/Shader.cpp
        src/Engine/VBO.cpp
        src/Engine/EBO.cpp
        src/Engine/VAO.cpp
        src/Engine/Texture.cpp
        src/Engine/Mesh.cpp
        src/Engine/Model.cpp
        src/Engine/Skybox.cpp
        src/Engine/ShadowMap.cpp
        src/Engine/Framebuffer.cpp
)


# Add executable
add_executable(${PROJECT_NAME} src/main.cpp HeaderFile/main.h
        src/Camera.cpp HeaderFile/Camera.h
        include/stb/stb.cpp include/stb/stb_image.h include/stb/stb_image_write.h
        ${HEADER_FILES})

# Link engine library
target_link_libraries(${PROJECT_NAME} engine)

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

# Find ASSIMP
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")


# Link libraries
if (WIN32)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLFW3_LIBRARY} ${GLEW_LIBRARY} assimp)
elseif (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} glfw GLEW GL assimp)
elseif (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo" glfw GLEW assimp)
endif ()

# Define GL_SILENCE_DEPRECATION to silence deprecation warnings
target_compile_definitions(${PROJECT_NAME} PRIVATE GL_SILENCE_DEPRECATION)



