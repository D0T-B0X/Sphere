#version 430 core

layout (location = 0) in vec3 aPos;
out vec4 vertexColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    vertexColor = vec4(1.0f, 0.5f, 0.31f, 1.0f);
}