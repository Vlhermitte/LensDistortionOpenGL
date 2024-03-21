# Implementation of lens distortion simulation in OpenGL

This repository is still in development. The project can be used, but some features are still being implemented.

## Approach
The scene is rendered to a custom framebuffer.
The image rendered in the framebuffer is then used as a texture to a quad.
The texture to the quad is then distorted using a lens distortion shader.

#### Radial distortion is model using the following equation:
```math
r = sqrt(x^2 + y^2)
x' = x * (1 + k1 * r^2 + k2 * r^4 + k3 * r^6) \\
y' = y * (1 + k1 * r^2 + k2 * r^4 + k3 * r^6) \\
```

Where k1, k2 and k3 are the radial distortion coefficients.

#### Tengential distortion is model using the following equation:
```math
x' = x + (2 * p1 * x * y + p2 * (r^2 + 2 * x^2)) \\
y' = y + (p1 * (r^2 + 2 * y^2) + 2 * p2 * x * y) \\
```
Where p1 and p2 are the tangential distortion coefficients.

### Pros and Cons
#### Pros
- Easy to implement
- Fast to compute
- Doesn't depend on the scene

#### Cons
- Since the distortion is applied to the texture, it leave unfilled areas in the corners of the image.

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

 You can install the dependencies using setup.bash script.

---

## Different approach (Not implemented yet)
Instead of applying the distortion to the texture, the position of each vertex can be moved to simulate the distortion.

The compute the new position of each vertex, we need to apply transformation to get to the clipping coordinates,
typically done using the projection matrix P:
```math
    P = 
    \begin{bmatrix}
        \frac{2n}{r-l} & 0 & \frac{r+l}{r-l} & 0 \\
        0 & \frac{2n}{t-b} & \frac{t+b}{t-b} & 0 \\
        0 & 0 & -\frac{f+n}{f-n} & -\frac{2fn}{f-n} \\
        0 & 0 & -1 & 0
    \end{bmatrix}
```

Then apply perspective division to normalize the coordinates.

```math
    x_{ndc} = \frac{x_{clip}}{w_{clip}} \\
    y_{ndc} = \frac{y_{clip}}{w_{clip}} \\
    z_{ndc} = \frac{z_{clip}}{w_{clip}} \\
```

Then we can apply the distortion to the normalized coordinates using the following equation:

```math
    x_{distorted} = x_{ndc} * (1 + k1 * r^2 + k2 * r^4 + k3 * r^6) \\
    y_{distorted} = y_{ndc} * (1 + k1 * r^2 + k2 * r^4 + k3 * r^6) \\
```

Where k1, k2 and k3 are the radial distortion coefficients and r is the distance from the center of the image to the vertex position.

Then we need to apply the inverse of the perspective division to get the distorted clip coordinates.

```math
    x_{clip} = x_{distorted} * w_{clip} \\
    y_{clip} = y_{distorted} * w_{clip} \\
    z_{clip} = z_{distorted} * w_{clip} \\
```

Finally, we can apply the inverse of the projection matrix to get the distorted vertex position.

```math
    P^{-1} = 
    \begin{bmatrix}
        \frac{r-l}{2n} & 0 & 0 & \frac{r+l}{2n} \\
        0 & \frac{t-b}{2n} & 0 & \frac{t+b}{2n} \\
        0 & 0 & 0 & -1 \\
        0 & 0 & \frac{f-n}{-2fn} & \frac{f+n}{2fn}
    \end{bmatrix}
```

```math
    x_{distorted} = \frac{r-l}{2n} * x_{clip} + \frac{r+l}{2n} * z_{clip} \\
    y_{distorted} = \frac{t-b}{2n} * y_{clip} + \frac{t+b}{2n} * z_{clip} \\
    z_{distorted} = -z_{clip}
```

Where l, r, t, b, n and f are the left, right, top, bottom, near and far values of the frustum.

### Pros and Cons
#### Pros
- Might be dependent on the polygon resolution of each models present in the scene.
- Should resolve the unfilled areas in the corners of the image.
- To be determined ...

#### Cons
- To be determined ...

### Note
This approach need further investigation to check its validity. And find its pros and cons.