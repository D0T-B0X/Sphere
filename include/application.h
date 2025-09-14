#ifndef APPLICATION_H
#define APPLICATION_H

#include "Renderer/renderer.h"

// High–level application wrapper that owns persistent Sphere instances
// and the Renderer. Constructs scene objects, registers them with the
// renderer, then hands control to the render loop.
class App {
public:
    App() {

        // Initialize rendering subsystem (GLFW, GLAD, shader, state)
        renderer.init();

        // Configure first sphere (Coral)
        coral.Name  = "Coral";
        coral.Color = {1.0f, 0.5f, 0.31f};
        coral.setRadius(0.3f);          // marks geometry dirty for upload

        // Configure second sphere (Lagoon)
        lagoon.Name  = "Lagoon";
        lagoon.Color = {0.7f, 0.32f, 0.6f};
        lagoon.setRadius(0.3f);

        // These are ordinary lit spheres (not light sources)
        coral.source  = false;
        lagoon.source = false;

        // Register visible spheres with the renderer at given positions
        renderer.drawSphere(coral,  { 0.7f,  0.0f,  0.0f});
        renderer.drawSphere(lagoon, {-0.7f,  0.0f,  0.0f});

        // Configure light marker sphere
        light.Name   = "Light";
        light.Color  = {1.0f, 1.0f, 1.0f};
        light.source = true;            // renderer will treat this as the light
        light.setRadius(0.3f);

        // Register light sphere (initial position in front)
        renderer.drawSphere(light, {0.0f, 0.0f, -1.0f});
    }

    // Returns the GLFW window pointer (read‑only)
    const GLFWwindow* getWindow() {
        return renderer.getWindow();
    }

    // Enters the main rendering / event loop (blocks until exit)
    void run() {
        renderer.runRenderLoop();
    }

private:
    // Persistent scene objects (must outlive renderer usage)
    Sphere coral;
    Sphere lagoon;
    Sphere light;

    // Rendering engine instance
    Renderer renderer;
};

#endif
