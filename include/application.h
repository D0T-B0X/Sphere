#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "shader.h"
#include "sphere.h"
#include "config.h"

class App {
public:
    // Constructor to initiate the application with the necessary parameters.
    App(uint16_t width, uint16_t height, const char* name) 
        : window(nullptr), VBO(0), VAO(0) 
    {
        init(); // Initialize glfw window hints
        createWindow(width, height, name); // Create glfw Window
        loadGlad(); 

        // Load shader files and compile them
        ourShader.load(VSHADER_PATH, FSHADER_PATH);
    }

    // Getter function to get a reference of the active window.
    GLFWwindow* getWindow() const {
        return window;
    }

    // Function that runs the program.
    void run() {        
        while (!glfwWindowShouldClose(window)) {
            processInput(window);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            ourShader.use();
            glBindVertexArray(VAO);

            glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // Cleanup after the program is complete.
        cleanup();
    }

private:

    // Window object.
    GLFWwindow* window;
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    Shader ourShader;

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

    void setupVertexBuffer(float* vertices, size_t verticesSize, unsigned int* indices, size_t indicesSize) {
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

    // Handle user input.
    void processInput(GLFWwindow* window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    // Resize window.
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
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
