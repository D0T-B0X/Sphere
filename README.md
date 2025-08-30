# Sphere: OpenGL Cube-Based Sphere Renderer

## Overview

**Sphere** is a modern C++ OpenGL application that renders a cube-based sphere mesh, subdividing the surface into six equal-area faces. The project is designed for extensibility and experimentation, making it ideal for graphics programming, collision detection, and particle simulation research. This project is still in its early stages and active developemnt. Stay tuned for updates.

## Features

- **Cube-based sphere mesh** with adjustable subdivisions and radius
- **Real-time rendering** using OpenGL 4.3+
- **GLFW** for window/context management and input handling
- **GLAD** for OpenGL function loading
- **GLSL shaders** for customizable vertex and fragment processing
- **Camera controls** for interactive navigation
- **Live FPS display** in the window title
- **Modular codebase** for easy extension (collision, particles, etc.)

## Getting Started

### Prerequisites

- C++17 or newer compiler
- [GLFW](https://www.glfw.org/) development libraries
- [GLM](https://github.com/g-truc/glm) math library
- [GLAD](https://glad.dav1d.de/) loader (included)
- CMake ≥ 3.19

### Build Instructions

```sh
git clone https://github.com/D0T-B0X/Sphere.git
cd Sphere
mkdir build && cd build
cmake ..
make
./Sphere
```

### Controls

| Key        | Action                        |
|------------|------------------------------|
| `W/A/S/D`  | Move camera (forward/left/back/right) |
| `E/Q`      | Increase/decrease subdivisions |
| `I/P`      | Decrease/increase sphere radius |
| `ESC`      | Exit application              |

## Project Structure

```
Sphere/
├── include/         # Header files (App, Camera, CubeSphere, Shader, etc.)
├── src/             # Source files
├── shaders/         # GLSL shader programs
├── build/           # Build output (ignored by git)
├── CMakeLists.txt   # Build configuration
├── README.md        # Project documentation
```

## Extending the Project

This codebase is designed for further development. You can:

- Implement **collision detection** between spheres and other objects
- Add **particle systems** and simulate interactions
- Experiment with **physics engines**
- Enhance rendering with **lighting**.

## License

This project is released under the MIT License. See LICENSE for details.

---

**Contact:**  
For questions, suggestions, or contributions, please open an issue or submit a pull request.

---

*Ready to build your own graphics and simulation experiments? Start with Sphere!*