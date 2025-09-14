#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>      // OpenGL function loader
#include <GLFW/glfw3.h>     // Window / input
#include <iostream>
#include <string>
#include <sstream>

#include "shader.h"         // Shader wrapper (compile / link / uniform helpers)
#include "camera.h"         // FPS style camera
#include "cubesphere.h"     // CPU sphere (cube → sphere) geometry generator
#include "settings.h"       // Global settings (screen size, FOV, etc.)
#include "config.h"         // CMake‑generated (paths, if any)

// Simple GPU mesh container (one VAO/VBO/EBO + index count)
struct Mesh {
    unsigned int VBO = 0;
    unsigned int VAO = 0;
    unsigned int EBO = 0;
    int          indexCount = 0;
};

// Sphere instance: owns CPU geometry + its GPU mesh + render properties
struct Sphere {
    CubeSphere   geometry;          // Procedural vertex/index data (CPU side)
    Mesh         mesh;              // Uploaded GPU buffers (lazy created)
    glm::vec3    Color{1.0f};       // Base albedo / emissive tint
    glm::vec3    Position{0.0f};    // World position (no rotation/scale here)
    std::string  Name;              // Debug name
    bool         source = false;    // True = treated as light/emissive
    bool         remake = true;     // True = geometry changed, needs re-upload

    // Default: unit radius sphere
    Sphere() : geometry(1.0f) {}

    Sphere(std::string& name, float radius, glm::vec3 color)
        : geometry(radius), Name(name), Color(color) {}

    Sphere(std::string& name, float radius, glm::vec3 color, glm::vec3 lighting)
        : geometry(radius), Name(name), Color(color), Position(lighting) {}

    // Mark geometry dirty when parameters change
    void setRadius(float radius) {
        geometry.setRadius(radius);
        remake = true;
    }
    void setSubdivisions(unsigned int subs) {
        geometry.setSubdivisions(subs);
        remake = true;
    }
};

// Renderer: owns window, GL context, shader, camera, and sphere registry
class Renderer {
public:
    Renderer();

    // Initialize context, load GL functions, compile shaders
    void init();

    // Register a sphere instance at a position (uploads mesh if needed)
    void drawSphere(Sphere& sphere, glm::vec3 position);

    // Main loop (poll events, render, swap buffers)
    void runRenderLoop();

    // Access underlying GLFW window (e.g., for additional user logic)
    GLFWwindow* getWindow();

private:
    // --- Core state ---
    GLFWwindow* window = nullptr;
    Camera      camera;
    Shader      ourShader;

    // All spheres submitted for rendering (stored as pointers; lifetime managed by caller)
    std::vector<Sphere*> spheres;

    // Pointer to the sphere acting as light source
    Sphere* lightSphere = nullptr;

    // Mouse state
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool  firstMouse = true;

    // Frame timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // --- Internal helpers ---
    void initGlfwWindow();                                        // Setup GLFW hints
    void createGlfwWindow(unsigned int width, unsigned int height,
                          const char* name);                      // Create + bind context + callbacks
    void loadGLAD();                                              // Load GL function pointers
    void generateCameraView();                                    // Upload view/projection matrices
    void setupSphereVertexBuffer(Sphere& sphere);                 // Lazy (re)upload sphere mesh
    static void frameBufferSizeCallback(GLFWwindow* window,
                                        int width, int height);   // Resize viewport
    void processKeyboardInput(GLFWwindow* window);                // WASD / vertical movement
    static void mouseCallback(GLFWwindow* window,
                              double xpos, double ypos);          // Static → instance redirect
    void handleMouse(double xpos, double ypos);                   // Apply mouse delta to camera
    void displayFrameRate(float deltaTime) const;                 // Title bar FPS update
    void cleanup();                                               // Release GL + GLFW resources
};

#endif