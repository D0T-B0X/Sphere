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

class Renderer {
public:
    Renderer();

    void init();
    void drawElement();
    void drawSphere(CubeSphere& sphere);
    void runRenderLoop();

    GLFWwindow* getWindow();

private:

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    GLFWwindow* window;
    Camera camera;
    Shader ourShader;

    unsigned int indexCount;

    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;

    float deltaTime;
    float lastFrame = 0.0f;

    void initGlfwWindow();
    void createGlfwWindow(unsigned int width, unsigned int height, const char* name);
    void loadGLAD();
    void generateCameraView();
    void setupVertexBuffer(const float* Vertices, const unsigned int* Indices, const size_t verticesSize, const size_t indicesSize);
    void updateVertexBuffer(CubeSphere sphere);
    static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
    void processKeyboardInput(GLFWwindow* window);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    void handleMouse(double xpos, double ypos);
    void displayFrameRate(float deltaTime) const;
    void cleanup();
};

#endif