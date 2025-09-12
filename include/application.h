#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <string>
#include "Renderer/cubesphere.h"
#include "Renderer/renderer.h"
#include "settings.h"

struct Object {
    CubeSphere Sphere;
    glm::vec3 Color;
    glm::vec3 LightingVector;
    std::string Name;

    Object() : Sphere(1.0f) {}
    Object(std::string& name, float radius, glm::vec3 color) : Sphere(radius), Name(name), Color(color) {}
    Object(std::string& name, float radius, glm::vec3 color, glm::vec3 lighting) : Sphere(radius), Name(name), Color(color), LightingVector(lighting) {}
};

class App {
public:
    // Constructor to initiate the application with the necessary parameters.
    App(uint16_t width, uint16_t height, const char* name)
    {

        // initialize the renderer
        renderer.init();

        // define an object
        object.Name = "Coral";
        object.Color = glm::vec3(1.0f, 0.5f, 0.31f);
        object.Sphere.setRadius(0.7f);

        // draw a sphere (test)
        renderer.drawSphere(object.Sphere);
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
    
    // Sphere class instance and index data.
    Object object;

    // Render engine object.
    Renderer renderer = Renderer();
};

#endif
