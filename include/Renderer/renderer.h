#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <sstream>
#include "shader.h"
#include "camera.h"
#include "cubesphere.h"
#include "settings.h"
#include "config.h"

struct Mesh {
    unsigned int VBO = 0;
    unsigned int VAO = 0;
    unsigned int EBO = 0;
    int indexCount = 0;
};

struct Sphere {
    CubeSphere geometry;
    Mesh mesh;
    glm::vec3 Color;
    glm::vec3 Position;
    std::string Name;
    bool source = false;
    bool remake = true;

    Sphere() : geometry(1.0f) {}
    Sphere(std::string& name, float radius, glm::vec3 color) : geometry(radius), Name(name), Color(color) {}
    Sphere(std::string& name, float radius, glm::vec3 color, glm::vec3 lighting) : geometry(radius), Name(name), Color(color), Position(lighting) {}

    void setRadius(float radius) {
        geometry.setRadius(radius);
        remake = true;
    }

    void setSubdivisions(unsigned int subs) {
        geometry.setSubdivisions(subs);
        remake = true;
    }
};

class Renderer {
public:
    Renderer();

    void init();
    void drawFlatSurface();
    void drawSphere(Sphere& sphere, glm::vec3 position);
    void runRenderLoop();

    GLFWwindow* getWindow();

private:

    unsigned int sphereVAO;
    unsigned int sphereVBO;
    unsigned int sphereEBO;

    unsigned int lightVAO;
    unsigned int lightVBO;
    unsigned int lightEBO;

    GLFWwindow* window;
    Camera camera;
    Shader ourShader;

    std::vector<Sphere*> spheres;

    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;

    float deltaTime;
    float lastFrame = 0.0f;

    void initGlfwWindow();
    void createGlfwWindow(unsigned int width, unsigned int height, const char* name);
    void loadGLAD();
    void generateCameraView();
    void setupSphereVertexBuffer(Sphere& sphere);
    static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
    void processKeyboardInput(GLFWwindow* window);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    void handleMouse(double xpos, double ypos);
    void displayFrameRate(float deltaTime) const;
    void cleanup();
};

#endif