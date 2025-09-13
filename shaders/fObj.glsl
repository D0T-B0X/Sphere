#version 430 core

out vec4 FragColor;

uniform bool source;
uniform vec3 inColor;

void main() {

    float ambientStrength = source ? 1.0 : 0.1;
    vec3 ambient = ambientStrength * vec3(1.0);

    FragColor = vec4(ambient, 1.0) * vec4(inColor, 1.0);
}