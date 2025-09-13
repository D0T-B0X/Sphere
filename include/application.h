#ifndef APPLICATION_H
#define APPLICATION_H

#include "Renderer/renderer.h"

class App {
public:
    // Constructor to initiate the application with the necessary parameters.
    App(uint16_t width, uint16_t height, const char* name)
    {

        // initialize the renderer
        renderer.init();

        // define an object
        coral.Name = "Coral";
        coral.Color = glm::vec3(1.0f, 0.5f, 0.31f);
        coral.setRadius(0.3f);

        lagoon.Name = "Lagoon";
        lagoon.Color = glm::vec3(0.7f, 0.32f, 0.6f);
        lagoon.setRadius(0.3f);
        
        coral.source = lagoon.source = false;

        // draw a sphere (test)
        renderer.drawSphere(coral, glm::vec3(0.7f, 0.0f, 0.0f));
        renderer.drawSphere(lagoon, glm::vec3(-0.7f, 0.0f, 0.0f));

        light.Name = "Light";
        light.Color = glm::vec3(1.0f, 1.0f, 1.0f);
        light.source = true;
        light.setRadius(0.3f);

        renderer.drawSphere(light, glm::vec3(0.0f, 0.0f, -3.0f));
    }

    // Getter function to get a reference of the active window.
    const GLFWwindow* getWindow() {
        return renderer.getWindow();
    }

    // Function that runs the program.
    void run() {
        
        // Start the render process
        renderer.runRenderLoop();
    }

private:
    
    // Sphere object instance.
    Sphere coral;
    Sphere lagoon;
    Sphere light;

    // Render engine object.
    Renderer renderer = Renderer();
};

#endif
