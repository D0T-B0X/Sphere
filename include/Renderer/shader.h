#ifndef SHADER_H
#define SHADER_H

#include <glm/gtc/matrix_transform.hpp>

class Shader {
public:
    // Program ID.
    unsigned int ID;

    // Function to read and build the shader.
    void load(const char* vertexPath, const char* fragmentPath);

    // Functions to activate/deactivate the shader.
    void use();
    void terminate() ;

    // Setter functions to create uniforms.
    void setBool(const char* name, int value) const;
    void setInt(std::string &name, int value) const;
    void setFloat(std::string &name, float value) const;
    void setVec3(const char* name, const glm::vec3& vec3) const;
    void setMat4(const char* name, glm::mat4 mat) const;

private:    
    // Function to check for any errors generated during linking or compilation of shaders.
    void checkCompileErrors(unsigned int shader, const char* type);
};

#endif