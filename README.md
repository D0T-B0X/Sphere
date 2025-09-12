# Sphere: OpenGL Cube-Based Sphere Renderer

## Overview

Sphere renders a cube-derived sphere mesh (cube faces uniformly subdivided and projected to a sphere) using modern OpenGL. It is a sandbox for extending into collision tests, particle / physics experiments, and rendering techniques.

## Features

- Parametric cube-sphere (adjustable radius & subdivisions)
- Modern OpenGL (core 4.3) + GLFW + GLAD + GLM
- Basic ambient fragment shading
- Camera (WASD + vertical + mouse look)
- FPS title update

## Build (Linux)

Dependencies (Ubuntu/Debian example):

```sh
sudo apt update
sudo apt install build-essential cmake pkg-config libglfw3-dev
```

Clone & build:

```sh
git clone https://github.com/D0T-B0X/Sphere.git
cd Sphere
mkdir build && cd build
cmake ..
make -j
./Sphere
```

## Controls

- Movement: W / A / S / D
- Vertical: SPACE (up), LEFT CTRL (down)
- Mouse: look (cursor locked)
- ESC: quit

## Project Layout

```
include/
  Renderer/ (camera, shader, cubesphere, renderer)
src/
  Renderer/ (implementations)
shaders/ (vObj.glsl, fObj.glsl)
config.h.in -> generates build/config.h
```

## Core Components

- **CubeSphere:** Generates vertices & indices per face, rebuilds on param change
- **Renderer:** Window/context, GLAD load, buffers, loop, input routing
- **Camera:** Euler Yaw/Pitch FPS-style
- **Shader:** Minimal utility wrapper (compile/link/use, uniform setters)

## Modifying Geometry

Call:

```cpp
sphere.setSubdivisions(n);
sphere.setRadius(r);
renderer.drawSphere(sphere); // reupload
```

## Extending Roadmap

Planned:

- Indexed normal & UV generation
- Lighting models (Phong / PBR)
- Wireframe overlay toggle
- Collision proxy (AABB / sphere)
- Particle system prototype
- ImGui debug overlay

## License

MIT

## Credits

- Cube-sphere concept inspiration: Song Ho Ahn
- Libraries: GLFW, GLAD, GLM

## Status

Early development; APIs subject to change.