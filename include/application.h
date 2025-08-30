#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>
#include "shader.h"
#include "cubesphere.h"
#include "camera.h"
#include "config.h"

class App {
public:
    // Constructor to initiate the application with the necessary parameters.
    App(uint16_t width, uint16_t height, const char* name) 
        : window(nullptr), VBO(0), VAO(0), sphere(0.7f, 2), camera(glm::vec3(0.0f, 0.0f, 3.0f))
    {
        init(); // Initialize glfw window hints
        createWindow(width, height, name); // Create glfw Window
        loadGlad(); 

        // Load shader files and compile them
        ourShader.load(VSHADER_PATH, FSHADER_PATH);

        // Sphere Data accumulation and rendering
        const float* Vertices = sphere.getVertexData();
        const unsigned int* Indices = sphere.getIndexData();
        const size_t verticesSize = sphere.getVertexDataSize();
        const size_t indicesSize = sphere.getIndexDataSize();

        indexCount = sphere.getIndexCount();

        setupVertexBuffer(
            Vertices,
            verticesSize,
            Indices,
            indicesSize
        );
    }

    // Getter function to get a reference of the active window.
    GLFWwindow* getWindow() const {
        return window;
    }

    // Function that runs the program.
    void run() {
        
        while (!glfwWindowShouldClose(window)) {
            float currentFrame = (float)glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            displayFrameRate(deltaTime);

            processInput(window);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            ourShader.use();
            glBindVertexArray(VAO);

            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // Cleanup after the program is complete.
        cleanup();
    }

private:

    // Window object.
    GLFWwindow* window;

    // Vertex render buffers.
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    
    // Shader class instance.
    Shader ourShader;

    // Sphere class instance and index data.
    CubeSphere sphere;
    size_t indexCount;

    // frame time difference.
    float deltaTime;
    float lastFrame = 0.0f;

    // Camera class instance.
    Camera camera;

    // Initialize GLFW with all necessary parameters.
    void init() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    // Generate an OpenGL window and set it as context.
    void createWindow(uint16_t width, uint16_t height, const char* name) {
        window = glfwCreateWindow(
            width,
            height,
            name,
            NULL,
            NULL
        );

        if (window == NULL) {
            std::cout << "Failed to create window" << std::endl;
            glfwTerminate();
            std::exit(-1);
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    }

    // Load GLAD.
    void loadGlad() {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to load GLAD" << std::endl;
            std::exit(-1);
        }
    }

    void setupVertexBuffer(const float* vertices, const size_t verticesSize, const unsigned int* indices, const size_t indicesSize) {
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void updateVertexBuffer() {
        const float* vertices = sphere.getVertexData();
        const size_t verticesSize = sphere.getVertexDataSize();
        const unsigned int* indices = sphere.getIndexData();
        const size_t indicesSize = sphere.getIndexDataSize();

        indexCount = sphere.getIndexCount();

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    // Handle user input.
    void processInput(GLFWwindow* window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        // Control Sphere dimensions.
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            sphere.setSubdivisions(sphere.getSubdivisions() + 1);
            updateVertexBuffer();
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            sphere.setSubdivisions(sphere.getSubdivisions() - 1);
            updateVertexBuffer();
        }

        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
            sphere.setRadius(sphere.getRadius() - 0.01f);
            updateVertexBuffer();
        }

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            sphere.setRadius(sphere.getRadius() + 0.01f);
            updateVertexBuffer();
        }

        // Control camera movement.
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.processKeyboard(cameraMovement::FORWARD, deltaTime);
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.processKeyboard(cameraMovement::BACKWARD, deltaTime);
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.processKeyboard(cameraMovement::LEFT, deltaTime);
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.processKeyboard(cameraMovement::RIGHT, deltaTime);
        }
    }

    // Resize window.
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void displayFrameRate(float deltaTime) const {

        static bool first = true;
        std::ostringstream oss;
        std::string title;

        static float timeSinceLastDisplay = 0.0f;
        timeSinceLastDisplay += deltaTime;

        if (first) {
            unsigned int frameRate = 1 / deltaTime;
            
            oss << APP_NAME << " | FPS : " << frameRate;
            title = oss.str();

            glfwSetWindowTitle(window, title.c_str());

            first = false;
        }

        if (timeSinceLastDisplay > 0.1f) {
            unsigned int frameRate = 1 / deltaTime;

            oss.clear();
            oss << APP_NAME << " | FPS : " << frameRate;
            title = oss.str();

            glfwSetWindowTitle(window, title.c_str());

            timeSinceLastDisplay = 0.0f;
        }
    }

    void cleanup() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        ourShader.terminate();
        glfwTerminate();
    }
};

#endif
