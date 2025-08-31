#version 430 core

layout (location = 0) in vec3 aPos;
out vec4 vertexColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    gl_Position = model * projection * view * vec4(aPos, 1.0);
    vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
}