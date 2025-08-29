#ifndef SHADER_H
#define SHADER_H

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
    void setBool(std::string &name, int value);
    void setInt(std::string &name, int value);
    void setFloat(std::string &name, float value);

private:    
    // Function to check for any errors generated during linking or compilation of shaders.
    void checkCompileErrors(unsigned int shader, const char* type);
};

#endif