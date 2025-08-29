#version 430 core

out vec4 FragColor;

in vec4 vertexColor;

void main() {
    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * vec3(1.0);

    FragColor = vec4(ambient, 1.0) * vertexColor;
}