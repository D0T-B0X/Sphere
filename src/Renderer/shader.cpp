#include "Renderer/shader.h"

// Loads, compiles, and links a vertex + fragment shader into a program
void Shader::load(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Enable exception flags on the file streams
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);        

    try {
        // Open shader source files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vertexStream, fragmentStream;

        // Read entire contents into string streams
        vertexStream   << vShaderFile.rdbuf();
        fragmentStream << fShaderFile.rdbuf();

        // Close files
        vShaderFile.close();
        fShaderFile.close();

        // Convert stream data to std::string
        vertexCode   = vertexStream.str();
        fragmentCode = fragmentStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER_FILE::NOT_SUCCESSFULLY_READ" << std::endl;
    }

    // Raw C-string pointers for OpenGL
    const char* vCode = vertexCode.c_str();
    const char* fCode = fragmentCode.c_str();

    unsigned int vertex, fragment;

    // Create and compile vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // Create and compile fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // Create program and attach compiled shaders
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);

    // Link program
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // Delete individual shader objects (no longer needed after linking)
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

// Activates the shader program
void Shader::use() {
    glUseProgram(ID);
}

// Deletes the shader program
void Shader::terminate() {
    glDeleteProgram(ID);
}

// Sets a boolean (int) uniform
void Shader::setBool(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name), (int)value);
}

// Sets an integer uniform
void Shader::setInt(std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

// Sets a float uniform
void Shader::setFloat(std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

// Sets a vec3 uniform
void Shader::setVec3(const char* name, const glm::vec3& vec3) const {
    glUniform3fv(glGetUniformLocation(ID, name), 1, &vec3[0]);
}

// Sets a mat4 uniform
void Shader::setMat4(const char* name, glm::mat4 mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);
}

// Checks compile or link errors for a shader or program
void Shader::checkCompileErrors(unsigned int shader, const char* type) {
    int success;
    char infoLog[1024];

    // Shader object error path
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::COMPILATION_ERROR of type: " << type
                      << "\n" << infoLog << std::endl;
        }
    } else {
        // Program object error path
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM::LINK_ERROR of type: " << type
                      << "\n" << infoLog << std::endl;
        }
    }
}