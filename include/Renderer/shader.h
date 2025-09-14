#ifndef SHADER_H
#define SHADER_H

#include <string>                   // Provides std::string
#include <glad/glad.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp> // Provides glm matrix utilities

// Encapsulates an OpenGL shader program and uniform helpers
class Shader {
public:
    unsigned int ID;                        // OpenGL shader program handle

    void load(const char* vertexPath, const char* fragmentPath); // Compiles and links vertex + fragment shaders
    void use();                              // Activates the shader program
    void terminate();                        // Deletes the shader program

    void setBool(const char* name, int value) const;             // Sets a boolean (int) uniform
    void setInt(std::string &name, int value) const;             // Sets an integer uniform
    void setFloat(std::string &name, float value) const;         // Sets a float uniform
    void setVec3(const char* name, const glm::vec3& vec3) const; // Sets a vec3 uniform
    void setMat4(const char* name, glm::mat4 mat) const;         // Sets a mat4 uniform

private:    
    void checkCompileErrors(unsigned int shader, const char* type); // Reports shader compile/link errors
};

#endif