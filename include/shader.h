#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

class Shader {
public:
    // Program ID.
    unsigned int ID;

    // Function to read and build the shader.
    void load(const char* vertexPath, const char* fragmentPath) {
        // String to store vertex and fragment shaders.
        std::string vertexCode;
        std::string fragmentCode;

        // Input Stream to read vertex and fragment shader files.
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // Add exceptions to catch any issues while reading the file.
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);        

        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);

            std::stringstream vertexStream, fragmentStream;

            // Read file data into memory
            vertexStream << vShaderFile.rdbuf();
            fragmentStream << fShaderFile.rdbuf();

            // Close files.
            vShaderFile.close();
            fShaderFile.close();

            // Store the stream data into the strings.
            vertexCode = vertexStream.str();
            fragmentCode = fragmentStream.str();
        } catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER_FILE::NOT_SUCCESSFULLY_READ" << std::endl;
        }

        // Store the string of shaders as C styled strings.
        const char* vCode = vertexCode.c_str();
        const char* fCode = fragmentCode.c_str();

        // Create shader IDs for vertex and fragment shaders.
        unsigned int vertex, fragment;

        // Generate and compile vertex shader in the GPU.
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        // Generate and compile fragment shader in the GPU.
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        // Creates a program object.
        ID = glCreateProgram();

        // Attach the compiled shader files to the program object.
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);

        // Link the attached shaders and make an executable for the GPU.
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        // Delete used shader objects.
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // Function to use/activate the shader.
    void use() {
        glUseProgram(ID);
    }

    void terminate() {
        glDeleteProgram(ID);
    }

    // Setter functions to create uniforms.
    void setBool(std::string &name, int value);
    void setInt(std::string &name, int value);
    void setFloat(std::string &name, float value);

private:    
    // Function to check for any errors generated during linking or compilation of shaders.
    void checkCompileErrors(unsigned int shader, const char* type) {
        int success;
        char infoLog[1024];

        // For all shaders.
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER::COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
            }
        } else {
            // For program.
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM::LINK_ERROR of type: " << type << "\n" << infoLog << std::endl;
            }
        }
    }
};

#endif