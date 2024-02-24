# Implementation of lens distortion simulation in OpenGL (STILL IN DEVELOPMENT)

This repository is still in development. The code is not yet ready for use.

## Approach
The scene is rendered to a custom framebuffer. 
The image rendered in the framebuffer is then used as a texture to a quad.
The texture to the quad is then distorted using a lens distortion shader.

## Demo
The following image is a demo of the lens distortion simulation.

![Demo](Screenshots/demo_radial_distortion.png)


## Dependencies
- OpenGL
- GLFW
- GLEW
- GLM
- stb_image
- Assimp
